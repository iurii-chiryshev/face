#include "subtractor.h"

using namespace cv;
using namespace std;

// работаем только с трехканальным изображением
static const int defFrameChannels = 3;
// работаем только с uchar глубиной каналов
static const int defFrameDepth = CV_8U;
// работаем только с таким типом вх. изображений
static const int defFrameType = CV_MAKETYPE(defFrameDepth,defFrameChannels);
// глубина модели
static const int defModelDepth = CV_32F;


// ско шума по умолчанию
static const float defNoizeSigma = 4.0f;
// дисперсия шума по умолчанию (присваивается если изображение одно)
static const float defNoizeVariance = defNoizeSigma*defNoizeSigma;
// множитель для порога по ско ()
static const float defDevThreshFactor = 2.5f;
// множитель для порога по дисперсии
static const float defVarThreshFactor = defDevThreshFactor*defDevThreshFactor;
// параметр обновления (какую долю данных брать из обновляемой модели)
static const float defAlpha = 0.95f;



/**
 * @brief The OG struct
 * Гаусиан
 */
struct OG
{
    //среднее
    float mean;
    //дисперсия
    float var;
};

/**
 * @brief The OG1Creator struct
 * OG1Creator
 */
struct OG1Creator: ParallelLoopBody
{
    OG1Creator(const cv::Mat& srcAcc,const cv::Mat& srsAccSq,int nAccum,Mat& dstBgmodel)
    {
        srcAcc_ = &srcAcc;
        srsAccSq_ = &srsAccSq;
        nAccum_ = nAccum;
        dstBgmodel_ = &dstBgmodel;
    }

    void operator()(const Range& range) const
    {
        int y0 = range.start;
        int y1 = range.end;
        int ncols = srcAcc_->cols;
        const int nchannels = defFrameChannels;
        float sf = 1.0f/nAccum_;
        float mean[nchannels];
        float var[nchannels];
        int dstStrige = ncols*nchannels*2;
        for(int y = y0; y < y1; y++)
        {
            const float* data = srcAcc_->ptr<float>(y);
            const float* dataSq = srsAccSq_->ptr<float>(y);
            float* dataBgmodel = (float*)( dstBgmodel_->ptr<float>() + y*dstStrige );
            for(int x = 0; x< ncols; x++, data += nchannels, dataSq += nchannels, dataBgmodel += nchannels*2)
            {
                // расчитать среднее и дисперсию
                // VAR = 1/N*SUM(X^2)- (1/N*SUM(X))^2
                mean[0] = data[0]*sf; mean[1] = data[1]*sf; mean[2] = data[2]*sf;
                var[0] = dataSq[0]*sf; var[1] = dataSq[1]*sf; var[2] = dataSq[2]*sf;
                var[0] -= mean[0]*mean[0]; if(var[0] < 1.0f) var[0] = defNoizeVariance;
                var[1] -= mean[1]*mean[1]; if(var[1] < 1.0f) var[1] = defNoizeVariance;
                var[2] -= mean[2]*mean[2]; if(var[2] < 1.0f) var[2] = defNoizeVariance;
                // заполнить модель по схеме
                // [MEAN_b,VAR_b,MEAN_g,VAR_g,MEAN_r,VAR_r]
                dataBgmodel[0] = mean[0]; dataBgmodel[1] = var[0];
                dataBgmodel[2] = mean[1]; dataBgmodel[3] = var[1];
                dataBgmodel[4] = mean[2]; dataBgmodel[5] = var[2];
            }
        }
    }

    const cv::Mat* srcAcc_;
    const cv::Mat* srsAccSq_;
    cv::Mat* dstBgmodel_;
    int nAccum_;
};


/**
 * @brief The OG1Invoker struct
 */
struct OG1Invoker: ParallelLoopBody
{
    OG1Invoker(const cv::Mat& image,const float* bgmodel, cv::Mat& fgmask,float varFactor)
    {
        image_ = &image;
        bgmodel_ = bgmodel;
        fgmask_ = &fgmask;
        varFactor_ = varFactor;
    }

    void operator()(const Range& range) const
    {
        int i;
        int y0 = range.start;
        int y1 = range.end;
        int ncols = image_->cols;
        const int nchannels = defFrameChannels;
        OG* mptr0 = (OG*)bgmodel_;
        for(int y = y0; y < y1; y++)
        {
            const uchar* iptr = image_->ptr<uchar>(y);
            uchar* fgptr = fgmask_->ptr<uchar>(y);
            OG* mptr = mptr0 + y*ncols*nchannels;
            for(int x  = 0; x < ncols; x++, mptr += nchannels)
            {
                int pix = x*nchannels;
                for(i = 0; i< nchannels;i++)
                {
                    float diff = mptr[i].mean - iptr[pix + i];
                    diff *= diff;
                    if(diff > mptr[i].var*varFactor_)
                        break;
                }
                fgptr[x] = (i == nchannels ? 0 : 255);
            }
        }
    }

    const cv::Mat* image_;
    const float* bgmodel_;
    cv::Mat* fgmask_;
    float varFactor_;
};


/**
 * @brief The OG1Updater struct
 */
struct OG1Updater: ParallelLoopBody
{
    OG1Updater(const cv::Mat& srcImg, const cv::Mat& srcFgmask, float alpha,float * dstBgmodel)
    {
        srcImg_ = &srcImg;
        srcFgmask_ = &srcFgmask;
        alpha_ = alpha;
        betta_ = 1.0f - alpha_;
        dstBgmodel_ = dstBgmodel;
    }

    void operator()(const Range& range) const
    {
        int i;
        int y0 = range.start;
        int y1 = range.end;
        int ncols = srcImg_->cols;
        const int nchannels = defFrameChannels;
        OG* mptr0 = (OG*)dstBgmodel_;
        for(int y = y0; y < y1; y++)
        {
            const uchar* iptr = srcImg_->ptr<uchar>(y);
            const uchar* fgptr = srcFgmask_->ptr<uchar>(y);
            OG* mptr = mptr0 + y*ncols*nchannels;
            for(int x  = 0; x < ncols; x++, mptr += nchannels)
            {
                if(fgptr[x] == 0)
                {
                    // обновляем только нулевые пикселы маски
                    int pix = x*nchannels;
                    float val, old_mean, new_mean, mean_diff, diff;
                    for(i = 0; i< nchannels;++i)
                    {
                        val = (float)iptr[pix+i];
                        old_mean = mptr[i].mean;
                        new_mean = alpha_*old_mean + betta_*val;
                        mean_diff = new_mean - old_mean;
                        mean_diff *= mean_diff;
                        diff = new_mean - val;
                        diff *= diff;
                        mptr[i].mean = new_mean;
                        mptr[i].var = alpha_*( mptr[i].var + mean_diff ) + betta_*diff;
                    }
                }
            }
        }
    }

    const cv::Mat* srcImg_;
    const cv::Mat* srcFgmask_;
    float alpha_;
    float betta_;
    float * dstBgmodel_;

};



SubtractorOG1::SubtractorOG1(): nAccum_(0), fInitBgModel_(0)
{
    varThreshFactor_ = defVarThreshFactor;
    alpha_ = defAlpha;
}

SubtractorOG1::SubtractorOG1(float threshFactor, float alpha): nAccum_(0), fInitBgModel_(0)
{
    float devThreshFactor = MAX(0.0f,MIN(threshFactor,10.f));
    varThreshFactor_ = devThreshFactor*devThreshFactor;
    alpha_ = MAX(0.0f,MIN(alpha_,1.0f));
}

SubtractorOG1::~SubtractorOG1()
{

}

void SubtractorOG1::operator()(InputArray src, OutputArray dst)
{
    if(fInitBgModel_ == 0)
    {
        //если еще не инициализировали сделаем это
        accumulateBackground(src);
        createBackgroundModel();
    }
    cv::Mat image = src.getMat();
    CV_Assert(image.size() == frameSize_ && image.type() == defFrameType);
    dst.create( image.size(), CV_MAKETYPE(defFrameDepth,1) );
    cv::Mat fgmask = dst.getMat();
    // запуск параллельно
    cv::parallel_for_(Range(0, frameSize_.height),
        OG1Invoker(image,bgmodel_.ptr<float>(),fgmask,varThreshFactor_));
}

inline void SubtractorOG1::createBackgroundModel()
{
    CV_Assert(nAccum_ != 0);
    // создание памяти под модель
    // структура модели [MEAN_b,VAR_b,MEAN_g,VAR_g,MEAN_r,VAR_r][...][...][...]
    // MEAN_b - средняя в синем канале
    // VAR_r - дисперсия в красном канале
    bgmodel_.create( 1, frameSize_.height*frameSize_.width*defFrameChannels*2, CV_MAKETYPE(defModelDepth,1) );
    // заполнить параллельно
    cv::parallel_for_(Range(0, frameSize_.height),
                    OG1Creator(accum_,accumSq_,nAccum_,bgmodel_));
    // освободить память под аккумы
    accum_.release();
    accumSq_.release();
    // созать пустую маску forereground (все 0)
    emptyFgmask_.create(frameSize_,CV_MAKETYPE(defFrameDepth,1));
    emptyFgmask_ = Scalar::all(0);
    // выставить флаг инициализации
    fInitBgModel_ = 1;
}

inline void SubtractorOG1::accumulateBackground(cv::InputArray src)
{
    // если изображение пустое получи ошибку
    CV_Assert(!src.empty());
    // получить изображение
    cv::Mat image = src.getMat();
    cv::Size size = image.size();
    int nchannels = image.channels(), depth = image.depth();
     // работаем только с трехканальными uchar
    CV_Assert(image.type() == defFrameType);
     // тип изображений аккумуляторов
    int accType = CV_MAKETYPE(defModelDepth,nchannels);
    bool needInitialize = (nAccum_ == 0 || frameSize_ != size);
    if(needInitialize)
    {
        // инициализируем размер, тип и аккумуляторы
        frameSize_ = size;
        nAccum_ = 0;
        initializeAccum(size,accType);
    }
    cv::Mat fimage;
    //uchar to float
    image.convertTo(fimage,accType,1.0,0.0);
    // накапливаем изображение
    cv::accumulate(fimage,accum_);
    // накапливам квадрат изображения
    cv::accumulateSquare(fimage,accumSq_);
    // увеличиваем счетчик
    ++nAccum_;
}

void SubtractorOG1::updateBackgroundModel(cv::InputArray src,cv::InputArray src_mask /*= cv::noArray()*/)
{
    if(fInitBgModel_ == 0)
    {
        //если еще не инициализировали сделаем это
        accumulateBackground(src);
        createBackgroundModel();
    }
    cv::Mat image = src.getMat();
    CV_Assert(image.size() == frameSize_ && image.type() == defFrameType);

    cv::Mat fgmask = (!src_mask.empty() ? src_mask.getMat() : emptyFgmask_);
    CV_Assert( fgmask.size() == frameSize_ && fgmask.type() == CV_MAKETYPE(defFrameDepth,1) );

    // вызываем папаллельное обновление
    cv::parallel_for_(Range(0, frameSize_.height),
        OG1Updater(image,fgmask,alpha_,bgmodel_.ptr<float>()));
}

void SubtractorOG1::getBackgroundImage(cv::OutputArray dst) const
{

}

inline void SubtractorOG1::initializeAccum(const cv::Size& size, int type)
{
    accum_.create(size,type);
    accum_ = Scalar::all(0);
    accumSq_.create(size,type);
    accumSq_ = Scalar::all(0);
}

