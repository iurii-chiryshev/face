#include "blob.h"

using namespace cv;
using namespace std;

namespace internal_bbox
{
    /*ограничивающий прямоугольник*/
    struct BBox
    {
        int minx;
        int miny;
        int maxx;
        int maxy;
        int cx;
        int cy;
    };
    inline static int _bb_distance(const BBox& box1, const BBox& box2)
    {
        int d1,d2;
        if (box1.cx < box2.minx)
        {
            if (box1.cy < box2.miny)
                d1 = MAX(box2.minx - box1.cx, box2.miny - box1.cy);
            else if (box1.cy>box2.maxy)
                d1 = MAX(box2.minx - box1.cx, box1.cy - box2.maxy);
            else
                d1 = box2.minx - box1.cx;
        }
        else if (box1.cx>box2.maxx)
        {
            if (box1.cy<box2.miny)
                d1 = MAX(box1.cx - box2.maxx, box2.miny - box1.cy);
            else if (box1.cy>box2.maxy)
                d1 = MAX(box1.cx - box2.maxx, box1.cy - box2.maxy);
            else
                d1 = box1.cx - box2.maxx;
        }
        else
        {
            if (box1.cy<box2.miny)
                d1 = box2.miny - box1.cy;
            else if (box1.cy>box2.maxy)
                d1 = box1.cy - box2.maxy;
            else
                return 0;
        }

        if (box2.cx < box1.minx)
        {
            if (box2.cy<box1.miny)
                d2 = MAX(box1.minx - box2.cx, box1.miny - box2.cy);
            else if (box2.cy>box1.maxy)
                d2 = MAX(box1.minx - box2.cx, box2.cy - box1.maxy);
            else
                d2 = box1.minx - box2.cx;
        }
        else if (box2.cx > box1.maxx)
        {
            if (box2.cy<box1.miny)
                d2 = MAX(box2.cx - box1.maxx, box1.miny - box2.cy);
            else if (box2.cy>box1.maxy)
                d2 = MAX(box2.cx - box1.maxx, box2.cy - box1.maxy);
            else
                d2 = box2.cx - box1.maxx;
        }
        else
        {
            if (box2.cy < box1.miny)
                d2 = box1.miny - box2.cy;
            else if (box2.cy>box1.maxy)
                d2 = box2.cy - box1.maxy;
            else
                return 0;
        }
        return MIN(d1, d2);
    }
    inline static void _rectToBox(const cv::Rect& rect,BBox& box)
    {
        box.minx = rect.x;
        box.miny = rect.y;
        box.maxx = rect.x + rect.width - 1;
        box.maxy = rect.y + rect.height - 1;
        box.cx = int( ( box.minx + box.maxx ) * 0.5 );
        box.cy = int( ( box.miny + box.maxy ) * 0.5 );
    }
    inline static void _blobToBox(const Blob& blob,BBox& box)
    {
        box.minx = blob.rect.x;
        box.miny = blob.rect.y;
        box.maxx = blob.rect.x + blob.rect.width - 1;
        box.maxy = blob.rect.y + blob.rect.height - 1;
        box.cx = blob.centroid.x;
        box.cy = blob.centroid.y;
    }

}
/************************************************************************/
/*
                                Blob
*/
/************************************************************************/

Blob::Blob(int _label /*= 0*/,
    int _pix /*= Blob::PIX_BACKGROUND*/,
    int _area /*= 0*/,
    const cv::Rect& _rect /*= cv::Rect()*/,
    const cv::Point& _centroid /*= cv::Point(0,0)*/)
    :label(_label),
    pix(_pix),
    area(_area),
    rect(_rect),
    centroid(_centroid),
    status(Blob::STATUS_NO)
{

}

bool Blob::isEmpty() const
{
    return *this == Blob();
}

inline bool Blob::operator == (const Blob& blob) const
{
    return (label == blob.label) && (pix == blob.pix) && (area == blob.area) && (rect == blob.rect) && (centroid == blob.centroid);
}
inline bool Blob::operator != (const Blob& blob) const
{
    return !(*this == blob);
}

void Blob::convertToVec(vector<int>& dst,int flags /*= Blob::ALL*/) const
{
    dst.clear();
    if(flags & Blob::BIT_AREA) dst.push_back(area);
    if(flags & Blob::BIT_CX) dst.push_back(centroid.x);
    if(flags & Blob::BIT_CY) dst.push_back(centroid.y);
    if(flags & Blob::BIT_MINX) dst.push_back(rect.x);
    if(flags & Blob::BIT_MAXX) dst.push_back(rect.x + rect.width - 1);
    if(flags & Blob::BIT_MINY) dst.push_back(rect.y);
    if(flags & Blob::BIT_MAXY) dst.push_back(rect.y + rect.height - 1);
    if(flags & Blob::BIT_WIDTH) dst.push_back(rect.width);
    if(flags & Blob::BIT_HEIGHT) dst.push_back(rect.height);

}

int Blob::distance(const shared_ptr<Blob>& src, int flags /*= Blob::ALL*/) const
{
    CV_Assert(src && "shared_ptr<Blob> is null");
    return distance(*src,flags);
}

int Blob::distance(const Blob* src, int flags /*= Blob::ALL*/) const
{
    return distance(*src,flags);
}

int Blob::distance(const Blob& src, int flags /*= Blob::ALL*/) const
{
    int dist = 0, val = 0;
    if(flags & Blob::BIT_AREA)
    {
        val = area - src.area;
        dist += val*val;
    }
    if(flags & Blob::BIT_CX)
    {
        val = centroid.x - src.centroid.x;
        dist += val*val;
    }
    if(flags & Blob::BIT_CY)
    {
        val = centroid.y - src.centroid.y;
        dist += val*val;
    }
    if(flags & Blob::BIT_MINX)
    {
        val = rect.x - src.rect.x;
        dist += val*val;
    }
    if(flags & Blob::BIT_MAXX)
    {
        val = rect.width + rect.x - src.rect.width - src.rect.x;
        dist += val*val;
    }
    if(flags & Blob::BIT_MINY)
    {
        val = rect.y - src.rect.y;
        dist += val*val;
    }
    if(flags & Blob::BIT_MAXY)
    {
        val = rect.height + rect.y - src.rect.height - src.rect.y;
        dist += val*val;
    }
    if(flags & Blob::BIT_WIDTH)
    {
        val = rect.width - src.rect.width;
        dist += val*val;
    }
    if(flags & Blob::BIT_HEIGHT)
    {
        val = rect.height - src.rect.height;
        dist += val*val;
    }
    return cvRound(sqrt(dist));
}

using namespace internal_bbox;

int Blob::boundingBoxDistance(const Blob& blob1,const Blob& blob2)
{
    BBox box1, box2;
    _blobToBox(blob1,box1);
    _blobToBox(blob2,box2);
    return _bb_distance(box1,box2);
}

int Blob::boundingBoxDistance(const Blob* blob1,const Blob* blob2)
{
    return Blob::boundingBoxDistance(*blob1,*blob2);
}

int Blob::boundingBoxDistance(const shared_ptr<Blob>& blob1,const shared_ptr<Blob>& blob2)
{
    return Blob::boundingBoxDistance(*blob1,*blob2);
}

int Blob::boundingBoxDistance(const Blob& blob,const cv::Rect& rectang)
{
    BBox box1, box2;
    _blobToBox(blob,box1);
    _rectToBox(rectang,box2);
    return _bb_distance(box1,box2);
}
int Blob::boundingBoxDistance(const Blob* blob,const cv::Rect& rectang)
{
    return Blob::boundingBoxDistance(*blob,rectang);
}
int Blob::boundingBoxDistance(const shared_ptr<Blob>& blob,const cv::Rect& rectang)
{
    CV_Assert(blob && "shared_ptr<Blob> is null");
    return Blob::boundingBoxDistance(*blob,rectang);
}



/************************************************************************/
/*
                    Any blob functions
*/
/************************************************************************/

void deletePtrBlobs(PtrBlobs& ptr_blobs)
{
    auto iter = ptr_blobs.begin();
    while(iter != ptr_blobs.end())
    {
        delete *(iter++);
    }
    ptr_blobs.clear();
}

void splitBlobsByPix(const Blobs& blobs,Blobs& fg_blobs, Blobs& shad_blobs)
{
    const size_t nblobs = blobs.size();
    fg_blobs.clear();
    shad_blobs.clear();
    for(size_t i = 0; i< nblobs;++i)
    {
        const Blob& blob = blobs[i];
        if(blob.pix == Blob::PIX_FOREGROUND){
            fg_blobs.push_back(blob);
        }
        else if(blob.pix == Blob::PIX_SHADOW){
            shad_blobs.push_back(blob);
        }
    }

}
void splitBlobsByPix(const SPtrBlobs& blobs,SPtrBlobs& fg_blobs, SPtrBlobs& shad_blobs)
{
    const size_t nblobs = blobs.size();
    fg_blobs.clear();
    shad_blobs.clear();
    for(size_t i = 0; i< nblobs;++i)
    {
        const SPtrBlob& blob = blobs[i];
        if(blob->pix == Blob::PIX_FOREGROUND){
            fg_blobs.push_back(blob);
        }
        else if(blob->pix == Blob::PIX_SHADOW){
            shad_blobs.push_back(blob);
        }
    }
}

template<typename _BTy>
inline void _mergeBlobs( vector<_BTy>& dst,const vector<_BTy>& src1,const vector<_BTy>& src2)
{
    const size_t n1 = src1.size(), n2 = src2.size();
    size_t i;
    dst.clear();
    dst.reserve(n1 + n2);
    for(i = 0; i< n1;i++)
        dst.push_back(src1[i]);
    for(;i < n1+n2;i++)
        dst.push_back(src2[i]);
}
void mergeBlobs(Blobs& dst,const Blobs& src1,const Blobs& src2)
{
    _mergeBlobs(dst,src1,src2);
}
void mergeBlobs(SPtrBlobs& dst,const SPtrBlobs& src1,const SPtrBlobs& src2)
{
    _mergeBlobs(dst,src1,src2);
}

void getBlobsByPix(const Blobs& src,Blobs& dst, int pix /*= Blob::PIX_FOREGROUND*/)
{
    const size_t nblobs = src.size();
    dst.clear();
    for(size_t i = 0; i< nblobs;++i)
    {
        const Blob& blob = src[i];
        if(blob.pix == pix){
            dst.push_back(blob);
        }
    }
}
void getBlobsByPix(const SPtrBlobs& src,SPtrBlobs& dst, int pix /*= Blob::PIX_FOREGROUND*/)
{
    const size_t nblobs = src.size();
    dst.clear();
    for(size_t i = 0; i< nblobs;++i)
    {
        const SPtrBlob& blob = src[i];
        if(blob->pix == pix){
            dst.push_back(blob);
        }
    }
}

namespace internal_assign
{
    /*задача о назначениях венгерским методом*/
    void assign_hung(Mat adj,int inf)
    {
        const int sz = adj.rows - 1;
        std::vector<int> u(sz+1);
        std::vector<int> v(sz+1);
        std::vector<int> p(sz+1); // вот здесь будут ответы
        std::vector<int> way(sz+1);
        for (int i=1; i <= sz; ++i)
        {
            p[0] = i;
            int j0 = 0;
            vector<int> minv(sz+1,inf);
            vector<bool> used (sz+1,false);
            do
            {
                used[j0] = true;
                int i0 = p[j0],  delta = inf,  j1;
                for (int j=1; j<=sz; ++j)
                {
                    if (used[j] == false)
                    {
                        int cur = adj.at<int>(i0,j)-u[i0]-v[j];
                        if (cur < minv[j])
                        {
                            minv[j] = cur,  way[j] = j0;
                        }
                        if (minv[j] < delta)
                        {
                            delta = minv[j],  j1 = j;
                        }
                    }
                }
                for (int j=0; j<=sz; ++j)
                {
                    if (used[j] == true)
                    {
                        u[p[j]] += delta,  v[j] -= delta;
                    }
                    else
                    {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while (p[j0] != 0);

            do
            {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
        /*
        заполнить вектор ответов, в привычной форме
        запись идет в нулевой столбец
        */
        for (int j=1; j<=sz; ++j)
        {
            adj.at<int>(p[j],0) = j;
        }
    }
    /*задача о назначениях жадным алгоритмом*/
    void assign_greegy(Mat adj,int inf)
    {
        CV_Error(CV_StsNotImplemented,"function not implemented");
    }

}

using namespace internal_assign;
void assignment(const SPtrBlobs& query,const SPtrBlobs& train,vector<DMatch>& matches,  int flags /*= Blob::ALL*/, int method /*= HUNGARIAN*/)
{

    CV_Assert(method == HUNGARIAN || method == GREEDY);
    CV_Assert(flags != 0);
    /*формируем матрицу расcтояний*/
    const int nquery = (int)query.size(), ntrain = (int)train.size();
    const int sz = max(nquery,ntrain);
    /*в матрице смежности должен быть фиктивный столбец и строка
      через них возвращается ответ
    */
    cv::Mat adj(sz+1,sz+1,CV_32SC1,cv::Scalar::all(-1));

    /*убнуляем фиктивные строку и столбец*/
    for( int i = 0 ; i <= sz ; ++i )
    {
        adj.at<int>(0,i) = 0;
        adj.at<int>(i,0) = 0;
    }

    int maxDist = 0;
    /*заполнить матрицу смежности, растояниями*/
    for(int i = 0; i< nquery; ++i)
    {
        for(int j = 0; j < ntrain; ++j )
        {
            int dist = query[i]->distance(train[j],flags);
            adj.at<int>(i+1,j+1) = dist;
            maxDist = max(maxDist,dist);
        }
    }

    /*заполнить матрицу смежности до конца, большими расстояниями*/
    ++maxDist;
    for(int i = 1; i <= sz; ++i)
    {
        for(int j = 1; j <= sz; ++j)
        {
            if(adj.at<int>(i,j) < 0)
            {
                adj.at<int>(i,j) = maxDist;
            }
        }
    }

    /*запуск алгоритма*/
    if(method == HUNGARIAN)
    {
        assign_hung(adj,++maxDist);
    }
    else // greedy
    {
        assign_greegy(adj,++maxDist);
    }


    /*заполнить matches*/
    matches.resize(sz);
    for(int qId = 0 ; qId < sz; ++qId )
    {
        int trId = adj.at<int>(qId+1,0) - 1;
        DMatch& m = matches[qId];
        if(qId < nquery && trId < ntrain)
        {
            /*оба индекса допустимы*/
            m.queryIdx = qId;
            m.trainIdx = trId;
            m.distance = (float)adj.at<int>(qId+1,trId+1);
            m.imgIdx = 0;
        }
        else
        {
            m.queryIdx = -1;
            m.trainIdx = -1;
            m.distance = -1;
            m.imgIdx = 0;
        }
    }
}
