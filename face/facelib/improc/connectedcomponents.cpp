#include "connectedcomponents.h"
#include "_ccl_sauf.h"

    using namespace cv;
    using namespace std;

/************************************************************************/
/*               закрасить мелкие области в 0 и удалить метку           */
/************************************************************************/
inline void clean_labels(Mat& img, cv::Mat& labels,const int val, const cv::Rect& rect)
{
    const int ncols = img.cols;
    uchar* idata0 = img.ptr(rect.y);
    int* ldata0 = (int*)labels.ptr(rect.y);
    for(int y = 0; y<rect.height;++y)
    {
        uchar* idata = idata0 + ncols*y + rect.x;
        int* ldata = ldata0 + ncols*y + rect.x;
        for(int x = 0; x< rect.width; ++x)
        {
            if(ldata[x] == val)
            {
                ldata[x] = 0;
                idata[x] = 0;
            }
        }
    }
}

/************************************************************************/
/*               закрасить мелкие области в new_color не удалять метку  */
/************************************************************************/
inline void fill_labels(Mat& img, cv::Mat& labels,const int val, const cv::Rect& rect,const uchar new_color)
{
    if(new_color == 0)
    {
        // если new_color == 0, закрасить область в 0 и удалить метку
        clean_labels(img,labels,val,rect);
        return;
    }
    const int ncols = img.cols;
    uchar* idata0 = img.ptr(rect.y);
    int* ldata0 = (int*)labels.ptr(rect.y);
    for(int y = 0; y<rect.height;++y)
    {
        uchar* idata = idata0 + ncols*y + rect.x;
        int* ldata = ldata0 + ncols*y + rect.x;
        for(int x = 0; x< rect.width; ++x)
        {
            if(ldata[x] == val)
            {
                /*ldata[x] = 0;*/
                idata[x] = new_color;
            }
        }
    }
}


namespace internal_wrapblobs{
/************************************************************************/
/*
Обертки для использования различных векторов блобов
*/
/************************************************************************/
    struct WrapBlobs
    {
        Blobs blobs;
        void operator()(int label,int pix,int area,const cv::Rect& rect,const cv::Point& centroid)
        {
            blobs.push_back(Blob(label,pix,area,rect,centroid));
        }
        size_t max_size() const
        {
            return blobs.max_size();
        }
        size_t size() const
        {
            return blobs.size();
        }
    };
    struct WrapSPtrBlobs
    {
        SPtrBlobs blobs;
        void operator()(int label,int pix,int area,const cv::Rect& rect,const cv::Point& centroid)
        {
            blobs.push_back(SPtrBlob(new Blob(label,pix,area,rect,centroid)));
        }
        size_t max_size() const
        {
            return blobs.max_size();
        }
        size_t size() const
        {
            return blobs.size();
        }
    };
    struct WrapPtrBlobs
    {
        PtrBlobs blobs;
        void operator()(int label,int pix,int area,const cv::Rect& rect,const cv::Point& centroid)
        {
            blobs.push_back(new Blob(label,pix,area,rect,centroid));
        }
        size_t max_size() const
        {
            return blobs.max_size();
        }
        size_t size() const
        {
            return blobs.size();
        }
    };

} // end namespace internal_wrapblobs

/************************************************************************/
/*            маркировка с использованием FloodFill                     */
/************************************************************************/
template<typename WrapTy>
inline void ccl_ff(InputOutputArray src,OutputArray dst, WrapTy& wb,int connectivity, int minarea, int maxarea)
{
    /* входной массив */
    cv::Mat image = src.getMat();
    const int nrows = image.rows;
    const int ncols = image.cols;

    /* выходной массив */
    // заполним картинку меток отрицательными значениями
    image.convertTo(dst,CV_32SC1,-1);
    cv::Mat label_image = dst.getMat();

    CvMat cvlabel_image = label_image;
    CvScalar zero_scalar = cvScalarAll(0);
    int label_count = 0;

    int* data0 = (int*)label_image.ptr();
    uchar* img0 = image.ptr();


    const size_t maxBlobSize = wb.max_size();
    /*int sz = 0;*/
    for(int y = 0; y < nrows; ++y)
    {
        int* data = data0 + ncols*y;
        uchar* img = img0 + ncols*y;
        for(int x = 0; x < ncols; ++x)
        {
            if(data[x] >= 0)
            {
                //если уже помечен или 0 на следующий пиксель
                continue;
            }

            // начинаем заливку с текущего пикселя
            CvConnectedComp ccomp;
            /*
              для ускорения можно переписать cvFloodFill
              чтоб статистику считала внутри, а не доп. проходом
            */
            cvFloodFill(&cvlabel_image,cvPoint(x,y), cvScalarAll(++label_count), zero_scalar, zero_scalar, &ccomp, connectivity, 0);
            int area = (int)ccomp.area;
            cv::Rect rect = ccomp.rect;
            if(area < minarea || area > maxarea || wb.size() >= maxBlobSize)
            {
                // закрасить в 0
                clean_labels(image,label_image,label_count,rect);
            }
            else
            {
                // считаем статистику + заполняем нашу структуру
                const int rh = rect.height , rw = rect.width;
                /*расчет статистики*/
                int* roi0 = data0 + rect.y*ncols + rect.x;
                __int64 m10 = 0, m01 = 0;
                for(int i = 0; i < rh; ++i)
                {
                    int* roi = roi0 + i*ncols;
                    for(int j = 0; j < rw; ++j)
                    {
                        if(roi[j] == label_count)
                        {
                            m10 += (j+x);
                            m01 += (i+y);
                        }
                    }
                }
                // расчитать остальное
                cv::Point centroid( (int)(m10 / area), (int)(m01 / area) );
                // добавить
                wb(label_count,(int)img[x],area,rect,centroid);
            }
        }
    }
}

/************************************************************************/
/*						    SAUF						                */
/************************************************************************/
template<typename WrapTy>
inline void ccl_sauf(InputOutputArray src,OutputArray dst, WrapTy& wb,int connectivity, int minarea, int maxarea)
{
    const size_t blob_size = sizeof(Blob);
    cv::Mat stats_32,stats_64;
    const size_t maxBlobSize = wb.max_size();
    /*выполняем маркировку*/
    const int nlabels = connectedComponentsWithStats_sauf(src,dst,stats_32,stats_64,connectivity,CV_32S);
    /*записываем данные в нашу структуру*/
    int* row0 = (int*)stats_32.ptr();
    /*0 лабел, как бы соответствует фону, не нужен
     начинаем с 1*/
    for(int i = 1; i< nlabels; ++i)
    {

        int* row = (int*)stats_32.ptr(i);
        cv::Rect rect = cv::Rect(row[CC_32_STAT_LEFT],row[CC_32_STAT_TOP],row[CC_32_STAT_WIDTH],row[CC_32_STAT_HEIGHT]);
        int area = row[CC_32_STAT_AREA];
        int pix = row[CC_32_STAT_PIX];// пиксель может быть либо 255 либо тень
        int label = i;

        if(area < minarea || area > maxarea || wb.size() >= maxBlobSize)
        {
            //не проходим по размерам или вектор переполнен
            //закрасить labels и src 0
            clean_labels(src.getMat(),dst.getMat(),label,rect);
        }
        else
        {
            __int64* row64 = (__int64*)(stats_64.ptr(i));
            cv::Point centroid(int(row64[CC_64_STAT_M10] / area), int(row64[CC_64_STAT_M01] / area));
            //заполнить
            wb(label,pix,area,rect,centroid);
        }

    }
}

/************************************************************************/
/*                       маркировка связных компонент                   */
/************************************************************************/

template<typename WrapTy>
inline void _connectedComponents(InputOutputArray src_img,OutputArray dst_labels, WrapTy& wb,int connectivity,int algorithm, int minarea, int maxarea)
{
    /*входной массив толко CV_8UC1*/
    CV_Assert( !src_img.empty() && src_img.type() == CV_8UC1);
    /*связность может быть либо 4 либо 8*/
    CV_Assert(connectivity == 4 || connectivity == 8);
    if(algorithm == CCL_FLOODFILL)
    {
        ccl_ff(src_img,dst_labels,wb,connectivity,minarea,maxarea);
    }
    else //CC_SAUF
    {
        ccl_sauf(src_img,dst_labels,wb,connectivity,minarea,maxarea);
    }
}

using namespace internal_wrapblobs;

void connectedComponents(InputOutputArray src_img,OutputArray dst_labels, Blobs& blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    WrapBlobs wb;
    _connectedComponents(src_img,dst_labels,wb,connectivity,algorithm,minarea,maxarea);
    blobs = wb.blobs;
}

void connectedComponents(InputOutputArray src_img,OutputArray dst_labels, SPtrBlobs& sp_blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    WrapSPtrBlobs wb;
    _connectedComponents(src_img,dst_labels,wb,connectivity,algorithm,minarea,maxarea);
    sp_blobs = wb.blobs;
}

void connectedComponents(InputOutputArray src_img,OutputArray dst_labels, PtrBlobs& p_blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    WrapPtrBlobs wb;
    _connectedComponents(src_img,dst_labels,wb,connectivity,algorithm,minarea,maxarea);
    p_blobs = wb.blobs;
}

template<typename WrapTy>
struct CCLInvoker : ParallelLoopBody
{
    CCLInvoker(vector<Mat>& imgs,vector<Mat>& labels, vector<WrapTy>& wbs,int connectivity,int algorithm, int minarea, int maxarea)
        : _imgs(imgs), _labels(labels), _wbs(wbs)
    {
        _connectivity = connectivity;
        _algorithm = algorithm;
        _minarea = minarea;
        _maxarea = maxarea;

    }
    void operator()(const Range& range) const
    {
        int y0 = range.start, y1 = range.end;

        for( int y = y0; y < y1; y++ )
        {
            _connectedComponents(_imgs[y],_labels[y],_wbs[y],_connectivity,_algorithm,_minarea,_maxarea);
        }
    }
    vector<Mat>& _imgs;
    vector<Mat>& _labels;
    vector<WrapTy>& _wbs;
    int _connectivity;
    int _algorithm;
    int _minarea;
    int _maxarea;

};

template<typename WrapTy>
inline void _parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<WrapTy>& wbs,int connectivity,int algorithm, int minarea, int maxarea)
{
    const int nimgs = (int)src_imgs.size();
    CV_Assert( nimgs > 0);
    dst_labels.resize(nimgs);
    wbs.resize(nimgs);
    parallel_for_(Range(0, nimgs),
        CCLInvoker<WrapTy>(src_imgs,dst_labels,wbs,connectivity,algorithm,minarea,maxarea));
}

void parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<Blobs>& blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    vector<WrapBlobs> wbs;
    _parallelConnectedComponents(src_imgs,dst_labels,wbs,connectivity,algorithm,minarea,maxarea);
    const int nblobs = (int)wbs.size();
    blobs.resize(nblobs);
    for(int i = 0; i< nblobs; ++i)
    {
        blobs[i] = wbs[i].blobs;
    }

}

void parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<SPtrBlobs>& blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    vector<WrapSPtrBlobs> wbs;
    _parallelConnectedComponents(src_imgs,dst_labels,wbs,connectivity,algorithm,minarea,maxarea);
    const int nblobs = (int)wbs.size();
    blobs.resize(nblobs);
    for(int i = 0; i< nblobs; ++i)
    {
        blobs[i] = wbs[i].blobs;
    }
}

void parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<PtrBlobs>& blobs,int connectivity /*= 4*/,int algorithm /*= CCL_SAUF*/, int minarea /*= 0*/, int maxarea /*= INT_MAX*/)
{
    vector<WrapPtrBlobs> wbs;
    _parallelConnectedComponents(src_imgs,dst_labels,wbs,connectivity,algorithm,minarea,maxarea);
    const int nblobs = (int)wbs.size();
    blobs.resize(nblobs);
    for(int i = 0; i< nblobs; ++i)
    {
        blobs[i] = wbs[i].blobs;
    }
}


