#include "subtractor.h"
#include "track/blob.h"


using namespace cv;
using namespace std;

/************************************************************************/
/*                            default const                             */
/************************************************************************/
// работаем только с трехканальным изображением
static const int defFrameChannels = 3;
// работаем только с uchar глубиной каналов
static const int defFrameDepth = CV_8U;
// типом входных изображений
static const int defFrameType = CV_MAKETYPE(defFrameDepth,defFrameChannels);
// глубина модели
static const int defModelDepth = CV_32F;


//определяет скорость обновления модели
//alpha = 1/defHistory
static const int defHistory = 300;
// коэффициент для определения находится цвет внутри смеси
static const float defVarThreshold = 3.5f*3.5f;
// максимальное число смесей
static const int defNMixtures = 4;
// какая доля в смеси является фоном
static const float defBackgroundRatio = 0.9f;
// коэффициент для обновления смеси меньше чем defVarThreshold
static const float defVarThresholdGen = 2.5f*2.5f;
static const float defVarInit = 4.0f*4.0f;
// максимальная дисперсия для моды
static const float defVarMax = 4.0f*defVarInit;
// минимальная дисперсия для моды
static const float defVarMin = 2.0f*2.0f;

// коэффициент уменьшения веса смесей, для уменьшения их количества
// расчитывается как prune = alpha*CT
static const float defCt = /*0.05f*/0.1f;
// каким цветом помечаем тень
static const uchar defShadowDetection = (uchar)Blob::PIX_SHADOW;
// каким цветом помечаем передний план
static const uchar defFgDetection = (uchar)Blob::PIX_FOREGROUND;
// каким цветом помечаем задний план
static const uchar defBgDetection = (uchar)Blob::PIX_BACKGROUND;

// порог для тени, чем меньше тем более темные области
// классифицируются как тень, а не темный объект
static const float defTau = 0.4f;
// порог на количество кадров для начала предсказания
static const float defPredictThreshold = 0.25f;

/*структура смеси*/
struct GMM
{
    float weight;
    float variance;
};

//детектор тени
//Prati,Mikic,Trivedi,Cucchiarra,"Detecting Moving Shadows...",IEEE PAMI,2003.
inline bool detectShadow(const uchar* data, int nchannels, int nmodes,
                const GMM* gmm, const float* mean,
                float Tb, float TB, float tau)
{
    float tWeight = 0;
    float mean_c;
    // check all the components  marked as background:
    for( int mode = 0; mode < nmodes; mode++, mean += nchannels )
    {
        GMM g = gmm[mode];

        float numerator = 0.0f;
        float denominator = 0.0f;
        for( int c = 0; c < nchannels; c++ )
        {
            mean_c = mean[c];
            denominator += mean_c * mean_c;
            numerator   += mean_c * data[c];
        }

        // no division by zero allowed
        if( denominator == 0 )
            return false;

        // if tau < a < 1 then also check the color distortion
        if( numerator <= denominator && numerator >= tau*denominator )
        {
            float a = numerator / denominator;
            float dist2a = 0.0f;

            for( int c = 0; c < nchannels; c++ )
            {
                float dD= a*mean[c] - data[c];
                dist2a += dD*dD;
            }

            if (dist2a < Tb*g.variance*a*a)
                return true;
        }

        tWeight += g.weight;
        if( tWeight > TB )
            return false;
    };
    return false;
}


/**
 * @brief The MOG2PInvoker struct
 */
struct MOG2PInvoker : ParallelLoopBody
{
    MOG2PInvoker(const cv::Mat& _src,const cv::Mat& _src_predict, cv::Mat& _dst,
                GMM* _gmm, float* _mean,
                cv::Mat& _modesUsed,
                int _nmixtures, float _alphaT,
                float _Tb, float _TB, float _Tg,
                float _varInit, float _varMin, float _varMax,
                float _prune, float _tau, bool _detectShadows)
    {
        src_ = &_src;
        src_predict_ = &_src_predict;
        dst_ = &_dst;
        gmm0_ = _gmm;
        mean0_ = _mean;
        modesUsed_ = &_modesUsed;
        nmixtures_ = _nmixtures;
        alphaT_ = _alphaT;
        Tb_ = _Tb;
        TB_ = _TB;
        Tg_ = _Tg;
        varInit_ = _varInit;
        varMin_ = MIN(_varMin, _varMax);
        varMax_ = MAX(_varMin, _varMax);
        prune_ = _prune;
        tau_ = _tau;
        detectShadows_ = _detectShadows;
    }

    void operator()(const Range& range) const
    {
        int y0 = range.start, y1 = range.end;
        int ncols = src_->cols, nchannels = src_->channels();
        float bettaT = 1.f - alphaT_;
        float dData[defFrameChannels];

        for( int y = y0; y < y1; y++ )
        {
            const uchar* data_ptr = src_->ptr(y);
            const uchar* predict_ptr = src_predict_->ptr(y);
            float* mean = mean0_ + ncols*nmixtures_*nchannels*y;
            GMM* gmm = gmm0_ + ncols*nmixtures_*y;
            uchar* modesUsed = modesUsed_->ptr(y);
            uchar* mask = dst_->ptr(y);

            for( int x = 0; x < ncols; x++, data_ptr += nchannels, gmm += nmixtures_, mean += nmixtures_*nchannels )
            {
                /*
                    если isPredict = true, запрещаем обновлять модель, запрещаем добалять новую компоненту
                    будем увеличивать вес только у сильных компонетн
                */
                /////////////////////////
                bool isPredict = (predict_ptr[x] > 0); // если > 0 -> пердсказан объект
                /////////////////////////


                bool background = false;//куда классифицировался пиксель

                //internal:
                bool fitsPDF = false;//если останется false, добавим новую компоненту
                int nmodes = modesUsed[x], nNewModes = nmodes;//число используемых мод
                float totalWeight = 0.f;

                float* mean_m = mean;

                // пошли по всем моделям
                for( int mode = 0; mode < nmodes; mode++, mean_m += nchannels )
                {
                    /////////////////////////
                    /*float weight = bettaT*gmm[mode].weight - prune_;*/
                    /////////////////////////

                    /////////////////////////
                    float weight = bettaT*gmm[mode].weight;
                    if(isPredict)
                    {
                        // в случе обнаружения на предыдущем кадре
                        // объекта или тени увеличиваем сильные компоненты
                        if(totalWeight < TB_)
                            weight += prune_;
                    }
                    else
                    {
                        weight -= prune_;
                    }
                    /////////////////////////

                    // если еще не нашли компоненту, ищем
                    if( !fitsPDF )
                    {

                        float var = gmm[mode].variance;

                        //считаем разницу и дистанцию
                        float dist2;

                        dData[0] = mean_m[0] - data_ptr[0];
                        dData[1] = mean_m[1] - data_ptr[1];
                        dData[2] = mean_m[2] - data_ptr[2];
                        dist2 = dData[0]*dData[0] + dData[1]*dData[1] + dData[2]*dData[2];

                        //проверка на преналежность фону
                        //растояние расчитано выше
                        if( totalWeight < TB_ && dist2 < Tb_*var )
                            background = true;

                        // если пиксель внутри смеси
                        // + не запрещено обновление isPredict == false
                        fitsPDF = dist2 < Tg_*var;
                        if( fitsPDF && !isPredict)
                        {
                            /////
                            // принадлежит смеси
                            // обновляем распределение

                            //обновление веса
                            weight += alphaT_;
                            float k = alphaT_/weight;

                            //обновление среднего
                            for( int c = 0; c < nchannels; c++ )
                                mean_m[c] -= k*dData[c];

                            //обновление дисперсии
                            float varnew = var + k*(dist2-var);
                            //limit the variance
                            varnew = MAX(varnew, varMin_);
                            varnew = MIN(varnew, varMax_);
                            gmm[mode].variance = varnew;

                            // сортируем, ищем новое меcто для моды
                            for( int i = mode; i > 0; i-- )
                            {
                                //check one up
                                if( weight < gmm[i-1].weight )
                                    break;
                                //swap one up
                                std::swap(gmm[i], gmm[i-1]);
                                for( int c = 0; c < nchannels; c++ )
                                    std::swap(mean[i*nchannels + c], mean[(i-1)*nchannels + c]);
                            }

                        } //if( fitsPDF && !isPredict)

                    }//if( !fitsPDF )

                    //проверяем, если вес меньше порога
                    if( weight <= prune_ )
                    {
                        weight = 0.0;
                        nmodes--;
                    }

                    gmm[mode].weight = weight;//обновляем вес
                    totalWeight += weight;
                }
                // прошли по всем моделям

                //перенормировка весов
                //сумма всегда = 1.0
                totalWeight = 1.f/totalWeight;
                for( int mode = 0; mode < nmodes; mode++ )
                {
                    gmm[mode].weight *= totalWeight;
                }

                /*nmodes = nNewModes;*/

                //делаем новое распределение если ничего не нашли ранее
                //и если не предсказан объект
                if( !fitsPDF && !isPredict)
                {
                    //определяем место для моды
                    //либо в конце существующих, если есть место
                    //либо заменяем последнюю, слабую
                    int mode = nmodes == nmixtures_ ? nmixtures_-1 : nmodes++;

                    // инициализируем вес
                    if (nmodes==1)
                        gmm[mode].weight = 1.f;
                    else
                    {
                        gmm[mode].weight = alphaT_;

                        // перенормировка других мод
                        for( int i = 0; i < nmodes-1; i++ )
                            gmm[i].weight *= bettaT;
                    }

                    // инициализируем среднее
                    for( int c = 0; c < nchannels; c++ )
                        mean[mode*nchannels + c] = data_ptr[c];
                    // инициализируем дисперсию
                    gmm[mode].variance = varInit_;

                    // сортировка
                    // поиск нового места для моды
                    for( int i = nmodes - 1; i > 0; i-- )
                    {
                        // check one up
                        if( alphaT_ < gmm[i-1].weight )
                            break;

                        // swap one up
                        std::swap(gmm[i], gmm[i-1]);
                        for( int c = 0; c < nchannels; c++ )
                            std::swap(mean[i*nchannels + c], mean[(i-1)*nchannels + c]);
                    }
                }

                //устанавливаем число используемых мод
                modesUsed[x] = uchar(nmodes);
                // инициализация маски переднего плана
                // с проверкой на тень
                mask[x] = background ? defBgDetection :
                    detectShadows_ && detectShadow(data_ptr, nchannels, nmodes, gmm, mean, Tb_, TB_, tau_) ?
                    defShadowDetection : defFgDetection;
            }
        }
    }

    const cv::Mat* src_;
    const cv::Mat* src_predict_;
    cv::Mat* dst_;
    GMM* gmm0_;
    float* mean0_;
    cv::Mat* modesUsed_;

    int nmixtures_;
    float alphaT_, Tb_, TB_, Tg_;
    float varInit_, varMin_, varMax_, prune_, tau_;

    bool detectShadows_;
};

/**
 * @brief The MOG2PBgCreator struct
 */
struct MOG2PBgCreator: ParallelLoopBody
{
    MOG2PBgCreator(const GMM* src_gmm,const float* src_mean,const cv::Mat& src_modesUsed,int nmixtures,float TB, cv::Mat& dst)
    {
        src_gmm_ = src_gmm;
        src_mean_ = src_mean;
        src_modesUsed_ = &src_modesUsed;
        dst_ = &dst;
        nmixtures_ = nmixtures;
        TB_ = TB;
    }

    void operator()(const Range& range) const
    {
        int y0 = range.start, y1 = range.end;
        int ncols = dst_->cols, nchannels = defFrameChannels;
        for( int y = y0; y < y1; y++ )
        {
            Vec3b* data = (Vec3b*)(dst_->ptr(y));
            const Vec3f* mean = (const Vec3f*)(src_mean_ + ncols*nmixtures_*nchannels*y);
            const GMM* gmm = src_gmm_ + ncols*nmixtures_*y;
            const uchar* modesUsed = src_modesUsed_->ptr(y);

            for( int x = 0; x < ncols; x++, gmm += nmixtures_, mean += nmixtures_)
            {
                int nmodes = modesUsed[x];
                Vec3f meanVal;
                float totalWeight = 0.f;
                for(int mode = 0; mode< nmodes; ++mode)
                {
                    GMM gaussian = gmm[mode];
                    meanVal += gaussian.weight * mean[mode];
                    totalWeight += gaussian.weight;

                    if(totalWeight > TB_)
                        break;
                }
                meanVal *= (1.f / totalWeight);
                data[x] = Vec3b(meanVal);
            }
        }
    }

    const GMM* src_gmm_;
    const float* src_mean_;
    const cv::Mat* src_modesUsed_;
    cv::Mat* dst_;
    int nmixtures_;
    float TB_;
};
/************************************************************************/
/*                       SubtractorMOG2P                                */
/************************************************************************/

SubtractorMOG2P::SubtractorMOG2P()
{
    bInitBgmodel_ = false;
    frameSize_ = Size(0,0);

    nframes_ = 0;
    history_ = defHistory;
    varThreshold_ = defVarThreshold;
    bShadowDetection_ = true;

    nmixtures_ = defNMixtures;
    backgroundRatio_ = defBackgroundRatio;
    fVarInit_ = defVarInit;
    fVarMax_  = defVarMax;
    fVarMin_ = defVarMin;

    varThresholdGen_ = defVarThresholdGen;
    CT_ = defCt;
    nShadowDetection_ =  defShadowDetection;
    Tau_ = defTau;
}

SubtractorMOG2P::SubtractorMOG2P(int _history,  float _varThreshold, bool _bShadowDetection)
{
    bInitBgmodel_ = false;
    frameSize_ = Size(0,0);

    nframes_ = 0;
    history_ = _history > 0 ? _history : defHistory;
    varThreshold_ = (_varThreshold>0)? _varThreshold : defVarThreshold;
    bShadowDetection_ = _bShadowDetection;

    nmixtures_ = defNMixtures;
    backgroundRatio_ = defBackgroundRatio;
    fVarInit_ = defVarInit;
    fVarMax_  = defVarMax;
    fVarMin_ = defVarMin;

    varThresholdGen_ = defVarThresholdGen;
    CT_ = defCt;
    nShadowDetection_ =  defShadowDetection;
    Tau_ = defTau;
}

SubtractorMOG2P::~SubtractorMOG2P()
{
}


void SubtractorMOG2P::initialize(Size size)
{
    // размер не может быть пустым
    CV_Assert( size != cv::Size());
    frameSize_ = size;
    nframes_ = 0;

    // модель
    // для каждой смеси вес, дисперсия, мат. ожидание для каждого канала
    bgmodel_.create( 1, frameSize_.height*frameSize_.width*nmixtures_*(2 + defFrameChannels), CV_MAKETYPE(defModelDepth,1) );
    bgmodel_ = Scalar::all(0);
    //количество смесей
    bgmodelUsedModes_.create(frameSize_,CV_MAKETYPE(defFrameDepth,1));
    bgmodelUsedModes_ = Scalar::all(0);
    // созать пустую маску предсказания
    emptyPredict_.create(frameSize_,CV_MAKETYPE(defFrameDepth,1));
    emptyPredict_ = Scalar::all(0);
    // флаг инициализации
    bInitBgmodel_ = true;
}

void SubtractorMOG2P::operator()(InputArray src, OutputArray dst_fgmask, InputArray src_predict /*= noArray()*/, float learningRate /*= -1*/)
{
    if(!bInitBgmodel_)
    {
        // инициализируем если еще не сделано
        initialize(src.size());
    }
    //исходное изображение
    cv::Mat image = src.getMat();
    CV_Assert(image.size() == frameSize_ && image.type() == defFrameType);

    //маска с предсказанием из предыдущего кадра
    cv::Mat predict = src_predict.empty() || nframes_ < history_*defPredictThreshold ? emptyPredict_ : src_predict.getMat();
    CV_Assert( predict.size() == frameSize_ && predict.type() == CV_MAKETYPE(defFrameDepth,1) );

    //выходной массив
    dst_fgmask.create( image.size(), CV_MAKETYPE(defFrameDepth,1) );
    cv::Mat fgmask = dst_fgmask.getMat();


    if(nframes_ < INT_MAX) ++nframes_; // чтоб при очередной сумме не стал отрицательным
    learningRate = learningRate >= 0 && nframes_ > 1 ? learningRate : 1.f/min( 2*nframes_, history_ );

    parallel_for_(Range(0, image.rows),
                  MOG2PInvoker(image,predict, fgmask,
                              (GMM*)bgmodel_.data,
                              (float*)(bgmodel_.data + sizeof(GMM)*nmixtures_*image.rows*image.cols),
                              bgmodelUsedModes_, nmixtures_, (float)learningRate,
                              (float)varThreshold_,
                              backgroundRatio_, varThresholdGen_,
                              fVarInit_, fVarMin_, fVarMax_, learningRate*CT_, Tau_,
                              bShadowDetection_));
}

void SubtractorMOG2P::getBackgroundImage(OutputArray backgroundImage) const
{
    CV_Assert( bInitBgmodel_ == true); // модель должна быть инициализирована
    backgroundImage.create( frameSize_, defFrameType );
    cv::Mat image = backgroundImage.getMat();
    // запуск параллельно (сирано долго работает)
    parallel_for_(Range(0, image.rows),
                    MOG2PBgCreator(	(GMM*)bgmodel_.data,
                                    (float*)(bgmodel_.data + sizeof(GMM)*nmixtures_*image.rows*image.cols),
                                    bgmodelUsedModes_, nmixtures_, backgroundRatio_,image) );
}

void SubtractorMOG2P::getUseMixturesImage(OutputArray dst)
{
    CV_Assert( bInitBgmodel_ == true); // модель должна быть инициализирована
    // скопировали
    cv::Mat tmp;
    bgmodelUsedModes_.copyTo(tmp);
    // маштабируем
    tmp.convertTo(dst,bgmodelUsedModes_.type(),255./nmixtures_);

}

void SubtractorMOG2P::getMeanVarImage(OutputArray dst)
{
    CV_Assert( bInitBgmodel_ == true); // модель должна быть инициализирована
    dst.create(frameSize_,defFrameDepth);
    cv::Mat var = dst.getMat();
    const GMM* gmm0 = (GMM*)bgmodel_.data;
    int ncols = frameSize_.width, nrows = frameSize_.height;
    for(int y = 0; y < nrows; y++)
    {
        const GMM* gmm = gmm0 + ncols*nmixtures_*y;
        uchar* data = var.ptr(y);
        const uchar* used = bgmodelUsedModes_.ptr(y);
        for(int x = 0; x < ncols; x++, gmm += nmixtures_)
        {
            float sumVar = 0.f;
            for(int i = 0; i< used[x]; ++i)
            {
                sumVar += gmm[i].variance;
            }
            sumVar /= nmixtures_;
            data[x] = (uchar)(sumVar*255.f/defVarMax);
        }
    }
}

