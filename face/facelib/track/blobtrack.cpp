#include "blobtrack.h"
#include "_blobmap.h"



using namespace cv;
using namespace std;


/**
 * @brief checkZone
 * @param zone
 * @param blobs
 * @param positive
 * проверка, находится объекты в зоне rect - типа зона интереса
 */
void checkZone(const cv::Rect& zone,const SPtrBlobs& blobs, vector<bool>& positive)
{
    const int nblobs = (int)blobs.size();
    // по умолчанию, блобы прошли проверку
    positive.resize(nblobs,true);
    if(zone == cv::Rect())
    {
        //	если прямоугольник пустой выходи
        return;
    }
    for(int i = 0; i < nblobs; ++i)
    {
        // в качестве меры растояния используется
        // растояние до ограничивающего прямоугольника
        // если == 0 - внутри, > 0 снаружи
        if(Blob::boundingBoxDistance(blobs[i],zone) > 0)
        {
            positive[i] = false;
        }
    }
}

// todo потом убрать отсюда
const int def_capacity = 10;

/************************************************************************/
/*                        BlobTrack                                     */
/************************************************************************/

BlobTrack::BlobTrack(ID id /*= 0*/, LifeTime lifetime /*= 1*/)
    :_id(id),_lifeTime(lifetime), _capacity(def_capacity), _blobHistory(_capacity)
{

}
bool BlobTrack::tryGetBlob(SPtrBlob& outBlob,int lavel /*= 0*/)
{

    if((int)_blobHistory.size() > lavel){
        //если размер буфера больше запрошенного уровня
        //верни его
        outBlob = _blobHistory[lavel];
        return true;
    }else{
        //если нет верни умный указалель на nullptr
        outBlob.reset();
        return false;
    }
}

BlobTrack::ID BlobTrack::getId() const
{
    return _id;
}

BlobTrack::LifeTime BlobTrack::getLifeTime() const
{
    return _lifeTime;
}

int BlobTrack::getCapacity() const
{
    return _capacity;
}

/************************************************************************/
/*                        BlobTracker                                    */
/************************************************************************/

BlobTracker::BlobTracker()
    : _last_id(0)
{


}

void BlobTracker::update(const SPtrBlobs& curr)
{
    SPtrBlobs prev;
    vector<Key> keys;
    // получить предыдущие блобы
    getLastBlobs(prev,keys);
    // получить соответствия
    BlobMapsInt maps;
    blobMapping(maps,prev,curr);
    const size_t nprev = prev.size(),ncurr = curr.size(), nmap = maps.size();
    if(nmap == 0) return;
    vector<bool> notused(nprev,false);
    /*в конце списка (> nprev) могут быть только либо новые, либо слияние 2-х и более
        для таких создаем новый трек, а старые треки удаляем
    */
    for(size_t i = nprev; i< nmap;++i)
    {
        const BlobMapInt& map = maps[i];
        int status = map.getStatus();
        if(status == BMS_MERGE)
        {
            // два и более объектов предыдущего кадра слились в один
            // удалить старые треки?
            const size_t nprev_val = map.prev_val.size();
            for(size_t j = 0;j < nprev_val; ++j)
            {
                //найти ключ для трека
                int key_id = map.prev_val[j];
                Key key = keys[key_id];
                // удалить этот трек, по его ключу
                _tracks.erase(key);
                // пометить, чтоб больше не использовал
                notused[key_id] = true;
            }
            // создать новый трек
            _last_id++;
            SPtrBlobTrack track(new BlobTrack(_last_id,1) );
            track->_blobHistory.push_front(curr[map.curr_val[0]]);
            _tracks.insert(make_pair( _last_id, track));
        }
        else if(status == BMS_NEW) // BMS_NEW
        {
            _last_id++;
            SPtrBlobTrack track(new BlobTrack(_last_id,1) );
            track->_blobHistory.push_front(curr[map.curr_val[0]]);
            _tracks.insert(make_pair( _last_id, track));
        }
        else
        {
            //других сотояний быть не должно
            //поэтому смело генерируем ошибку
            CV_Error(CV_StsBadArg,"Bad status");
        }
    }
    /*пройдемся по началу списка и отработаем остальные состояния*/
    for(size_t i = 0; i< nprev;++i)
    {
        if(notused[i] == true)
            continue;
        const BlobMapInt& map = maps[i];
        int status = map.getStatus();
        if(status == BMS_LOSE) // потерянный
        {
            //найти ключ для трека
            int key_id = map.prev_val[0];
            Key key = keys[key_id];
            // удалить этот трек, по его ключу
            _tracks.erase(key);
        }
        else if(status == BMS_ONE_TO_ONE) // однозначное соответствие
        {
            //найти ключ для трека
            int key_id = map.prev_val[0];
            Key key = keys[key_id];
            // получить трек по ключу
            SPtrBlobTrack& track = _tracks[key];
            // добавить историю, увеличить lifetime
            track->_lifeTime++;
            track->_blobHistory.push_front(curr[map.curr_val[0]]);
        }
        else if(status == BMS_SPLIT) // развалился на несколько частей
        {
            // удалить старый трек
            // для каждого нового создать новый трек
            //найти ключ для трека
            int key_id = map.prev_val[0];
            Key key = keys[key_id];
            // удалить этот трек, по его ключу
            _tracks.erase(key);
            const size_t ncurr_val = map.curr_val.size();
            for(size_t j = 0; j < ncurr_val; ++j)
            {
                _last_id++;
                SPtrBlobTrack track(new BlobTrack(_last_id,1) );
                track->_blobHistory.push_front(curr[map.curr_val[j]]);
                _tracks.insert(make_pair( _last_id, track));
            }
        }
        else
        {
            //других сотояний быть не должно
            //поэтому смело генерируем ошибку
            CV_Error(CV_StsBadArg,"Bad status");
        }
    }
    /*после того*/
}

void BlobTracker::getLastBlobs(SPtrBlobs& blobs,vector<Key>& keys,BlobTrack::LifeTime lefeTimeThreshold /*= 0*/)
{
    const size_t sz = _tracks.size();
    blobs.clear(); blobs.reserve(sz);
    keys.clear(); keys.reserve(sz);
    for(auto iter = _tracks.begin();iter != _tracks.end();++iter)
    {
        if(iter->second->_lifeTime >= lefeTimeThreshold)
        {
            /*
                если время жизни > порога
                получи выгребай последний блоб
            */
            SPtrBlob sptrBlob;
            if(iter->second->tryGetBlob(sptrBlob) == true)
            {
                blobs.push_back(sptrBlob);
                keys.push_back(iter->first);
            }
        }

    }
}

bool BlobTracker::tryGetLastBlob(Key key,SPtrBlob& outBlob)
{
    auto iter = _tracks.find(key);
    if(iter != _tracks.end())
    {
        /*чего-то нашел, извлекаем*/
        return iter->second->tryGetBlob(outBlob);
    }
    else
    {
        /*пустой указатель*/
        outBlob.reset();
        return false;
    }
}

void BlobTracker::getTracks(vector<SPtrBlobTrack> tracks,vector<Key>& keys,BlobTrack::LifeTime lefeTimeThreshold /*= 0*/)
{
    const size_t sz = _tracks.size();
    tracks.clear(); tracks.reserve(sz);
    keys.clear(); keys.reserve(sz);

    for(auto iter = _tracks.begin();iter != _tracks.end();++iter)
    {
        if(iter->second->_lifeTime >= lefeTimeThreshold)
        {
            /*если время существования трека больше порога заполняем*/
            keys.push_back(iter->first);
            tracks.push_back(iter->second);
        }
    }
}

bool BlobTracker::tryGetTrack(Key key,SPtrBlobTrack& outTrack)
{
    auto iter = _tracks.find(key);
    if(iter != _tracks.end())
    {
        /*чего-то нашел, извлекаем*/
        outTrack = iter->second;
        return true;
    }
    else
    {
        /*пустой указатель*/
        outTrack.reset();
        return false;
    }
}



