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
// работаем только с таким типом вх. изображений
static const int defFrameType = CV_MAKETYPE(defFrameDepth,defFrameChannels);
// глубина модели
static const int defModelDepth = CV_32F;

// количество смесей модели фона
static const int defNbgmixture = 3;
// ско шума по умолчанию
static const float defNoizeSigma = 15.0f;
// дисперсия шума по умолчанию (присваивается если изображение одно)
static const float defNoizeVariance = defNoizeSigma*defNoizeSigma;
// множитель для порога по ско ()
static const float defDevThreshFactor = 2.5f;
// множитель для порога по дисперсии
static const float defVarThreshFactor = defDevThreshFactor*defDevThreshFactor;
// параметр обновления (какую долю данных брать из обновляемой модели)
// в модели одной гаусианы альфа другая
static const float defAlpha = 0.1f;
// чем инициализируется новая смесь
static const float defInitialWeight = 0.05f;
// параметр для детектирования тени
static const float defTau = 0.5f;
// чем помечается тень
static const uchar defShadowVal = (uchar)Blob::PIX_SHADOW;


/**
 * Cмесь
 */
template<typename Ty> struct Mixture
{
    // ключ сортировки weight/variance
    float sortkey;
    // вес смеси
    float weight;
    // вектор средних
    Ty mean;
    // вектор дисперсий
    Ty var;
};

/**
 * @brief shadowDetect
 * @param pix
 * @param mixptr
 * @param usemix
 * @param varThreshFactor
 * @param tau
 * @return
 * детектор тени
 */
inline bool shadowDetect(const Vec3f& pix,Mixture<Vec3f>* mixptr,int usemix,float varThreshFactor, float tau)
{
    int c, mix;
    int nchannels = defFrameChannels;
    for(mix = 0; mix< usemix; mix++)
    {
        float numerator = pix.dot(mixptr[mix].mean);
        float denominator = mixptr[mix].mean.dot(mixptr[mix].mean);
        // на 0 делить нельзя
        if( denominator < FLT_EPSILON )
            return false;

        if( numerator <= denominator && numerator >= tau*denominator )
        {
            float a = numerator / denominator;
            Vec3f dD = mixptr[mix].mean * a;
            dD -= pix;
            float dist2a = dD.dot(dD);
            float variance = 0.0f;
            for(c = 0; c < nchannels; c++ )
            {
                variance += mixptr[mix].var[c];
            }
            variance /= nchannels;
            if (dist2a < varThreshFactor*variance*a*a)
                return true;
        }
    }
    return false;
}

/**
 * @brief The MG1Updater struct
 * Структура для параллельного обновления
 */
struct MG1Updater: ParallelLoopBody
{
    MG1Updater(const cv::Mat& src_img,const cv::Mat& src_mask, cv::Mat& dst_bgmodel,Mat& dst_usemix,
                float alpha,int nMaxMixtures,float varThreshFactor,float noizeVariance,float initialWeight)
    {
        src_img_ = &src_img;
        src_mask_ = &src_mask;
        dst_bgmodel_ = &dst_bgmodel;
        dst_usemix_ = &dst_usemix;
        alpha_ = alpha;
        nMaxMixtures_ = nMaxMixtures;
        varThreshFactor_ = varThreshFactor;
        noizeVariance_ = noizeVariance;
        initialWeight_ = initialWeight;
    }

    void operator()(const Range& range) const
    {
        int k,k1,n;
        int y0 = range.start;
        int y1 = range.end;
        int ncols = src_img_->cols;
        const int nchannels = defFrameChannels;
        int K = nMaxMixtures_;

        const float w0 = initialWeight_;
        const float var0 = noizeVariance_;
        const float sk0 = w0/var0;
        float minVar = noizeVariance_;

        for(int y = y0; y < y1; y++)
        {
            //in
            const uchar* iptr = src_img_->ptr<uchar>(y);
            const uchar* maskptr = src_mask_->ptr<uchar>(y);
            //out
            Mixture<Vec3f>* mptr = (Mixture<Vec3f>*)dst_bgmodel_->ptr<float>(y);
            uchar* uptr = dst_usemix_->ptr<uchar>(y);

            for(int x  = 0; x < ncols; x++, mptr += K)
            {
                if(maskptr[x] == 0)
                {
                    // обновляем только нули
                    float wsum = 0;
                    Vec3f pix(iptr[x*nchannels], iptr[x*nchannels+1], iptr[x*nchannels+2]);
                    int kHit = -1;  // на какой смеси среагировал, что это фон
                    for( k = 0; k < uptr[x]; k++ )
                    {
                        float w = mptr[k].weight;
                        wsum += w;
                        if( w < FLT_EPSILON )
                        {
                            // нет смеси
                            break;
                        }
                        Vec3f mu = mptr[k].mean;
                        Vec3f diff = pix - mu;
                        Vec3f diff_sq = diff.mul(diff);
                        Vec3f thresh = mptr[k].var*varThreshFactor_;
                        for(n = 0; n< nchannels;n++)
                        {
                            if(diff_sq[n] > thresh[n])
                            {
                                //скорее всего объект, выходим
                                break;
                            }
                        }
                        if(n == nchannels)
                        {
                            // точно фон на k смеси
                            wsum -= w;
                            float dw = alpha_*(1.f - w);
                            mptr[k].weight = w + dw;
                            mptr[k].mean = mu + diff*alpha_;
                            Vec3f var = Vec3f(	max(var[0] + alpha_*(diff_sq[0] - var[0]), minVar),
                                                max(var[1] + alpha_*(diff_sq[1] - var[1]), minVar),
                                                max(var[2] + alpha_*(diff_sq[2] - var[2]), minVar));
                            mptr[k].var = var;
                            mptr[k].sortkey = (w*3)/(var[0] + var[1] + var[2]);

                            for( k1 = k-1; k1 >= 0; k1-- )
                            {
                                if( mptr[k1].sortkey >= mptr[k1+1].sortkey )
                                    break;
                                std::swap( mptr[k1], mptr[k1+1] );
                            }

                            kHit = k1+1;
                            break;
                        }
                    }
                    if(kHit < 0)// не нашел соответствующую смесь, значит создадим
                    {
                        k = min(k, K-1);
                        uptr[x] = k+1;
                        wsum += w0 - mptr[k].weight;
                        mptr[k].weight = w0;
                        mptr[k].mean = pix;
                        mptr[k].var = Vec3f(var0, var0, var0);
                        mptr[k].sortkey = sk0;
                    }
                    else
                    {
                        for( ; k < uptr[x]; k++ )
                        {
                            wsum += mptr[k].weight;
                        }
                    }

                    float wscale = 1.f/wsum;
                    wsum = 0;
                    for( k = 0; k < uptr[x]; k++ )
                    {
                        wsum += mptr[k].weight *= wscale;
                        mptr[k].sortkey *= wscale;
                    }
                }// if(mask == 0)
            }
        }
    }

    const cv::Mat* src_img_;
    const cv::Mat* src_mask_;
    cv::Mat* dst_bgmodel_;
    cv::Mat* dst_usemix_;
    float alpha_;
    float betta_;
    int nMaxMixtures_;
    float varThreshFactor_;
    float noizeVariance_;
    float initialWeight_;
};


/**
 * @brief The MG1Invoker struct
 */
struct MG1Invoker: ParallelLoopBody
{
    MG1Invoker(const cv::Mat& src_img,const cv::Mat& src_bgmodel,const cv::Mat& src_usemix,Mat& dst_fg,Mat& dst_shadow,
                int nMaxMixtures,float varThreshFactor, float tau)
    {
        src_img_ = &src_img;
        src_bgmodel_ = &src_bgmodel;
        src_usemix_ = &src_usemix;
        dst_fg_ = &dst_fg;
        dst_shadow_ = &dst_shadow;
        // пороговый коэффициент попадания в гаусиану
        varThreshFactor_ = varThreshFactor;
        // параметр для детектирования тени
        tau_ = tau;
        // максимальное число смесей
        nMaxMixtures_ = nMaxMixtures;
    }

    /**
     * @brief operator ()
     * @param range
     */
    void operator()(const Range& range) const
    {
        int k,n;
        int y0 = range.start;
        int y1 = range.end;
        int ncols = src_img_->cols;
        const int nchannels = defFrameChannels;
        int K = nMaxMixtures_;
        for(int y = y0; y < y1; y++)
        {
            //in
            const uchar* iptr = src_img_->ptr<uchar>(y);
            Mixture<Vec3f>* mptr = (Mixture<Vec3f>*)src_bgmodel_->ptr<float>(y);
            const uchar* uptr = src_usemix_->ptr<uchar>(y);
            //out
            uchar* fptr = dst_fg_->ptr<uchar>(y);
            uchar* sptr = dst_shadow_->ptr<uchar>(y);

            for(int x  = 0; x < ncols; x++, mptr += K)
            {
                fptr[x] = sptr[x] = 0;
                Vec3f pix(iptr[x*nchannels], iptr[x*nchannels+1], iptr[x*nchannels+2]);
                int kHit = -1;  // на какой смеси среагировал
                for( k = 0; k < uptr[x]; k++ )
                {
                    float w = mptr[k].weight;
                    if( w < FLT_EPSILON )
                    {
                        // нет смеси
                        break;
                    }
                    Vec3f diff = pix - mptr[k].mean;
                    diff = diff.mul(diff);
                    Vec3f thresh = mptr[k].var*varThreshFactor_;
                    for(n = 0; n< nchannels;n++)
                    {
                        if(diff[n] > thresh[n])
                        {
                            //скорее всего объект
                            break;
                        }
                    }
                    if(n < nchannels)
                    {
                        // чего-то нашел, запомни k и выходи
                        kHit = k;
                        break;
                    }
                }
                if(kHit >= 0)
                {
                    // x пиксель принадлежит объекту
                    // нужно проверить на тень
                    if(shadowDetect(pix,mptr,uptr[x],varThreshFactor_,tau_))
                    {
                        sptr[x] = defShadowVal;
                    }
                    else
                    {
                        fptr[x] = 255;
                    }
                }
            }
        }
    }

    const cv::Mat* src_img_;
    const cv::Mat* src_bgmodel_;
    const cv::Mat* src_usemix_;
    cv::Mat* dst_fg_;
    cv::Mat* dst_shadow_;
    int nMaxMixtures_;
    float varThreshFactor_;
    float tau_;

};


/************************************************************************/
/*                     SubtractorMG1                                    */
/************************************************************************/

SubtractorMG1::SubtractorMG1()
{
    fInitBgModel_ = 0;
    varThreshFactor_ = defVarThreshFactor;
    alpha_ = defAlpha;
}

SubtractorMG1::SubtractorMG1(float threshFactor, float alpha)
{
    fInitBgModel_ = 0;
    float devThreshFactor = MAX(0.0f,MIN(threshFactor,10.f));
    varThreshFactor_ = devThreshFactor*devThreshFactor;
    alpha_ = MAX(0.0f,MIN(alpha_,1.0f));
}

SubtractorMG1::~SubtractorMG1()
{

}

void SubtractorMG1::updateBackgroundModel(cv::InputArray src,cv::InputArray src_mask /*= cv::noArray()*/)
{
    if(fInitBgModel_ == 0)
    {
        // инициализируем если еще не сделано
        initialize(src.size());
    }
    cv::Mat image = src.getMat();
    CV_Assert(image.size() == frameSize_ && image.type() == defFrameType);

    cv::Mat fgmask = (!src_mask.empty() ? src_mask.getMat() : emptyFgmask_);
    CV_Assert( fgmask.size() == frameSize_ && fgmask.type() == CV_MAKETYPE(defFrameDepth,1) );

    //запуск параллельно
    cv::parallel_for_(Range(0, frameSize_.height),
        MG1Updater(image,fgmask,bgmodel_,useBgmixture_,defAlpha,defNbgmixture,defVarThreshFactor,defNoizeVariance,defInitialWeight));
}

void SubtractorMG1::getBackgroundImage(cv::OutputArray dst) const
{

}

void SubtractorMG1::operator()(InputArray src, OutputArray dst_fg, OutputArray dst_shadow)
{
    if(fInitBgModel_ == 0)
    {
        // инициализируем если еще не сделано
        initialize(src.size());
    }

    cv::Mat image = src.getMat();
    CV_Assert(image.size() == frameSize_ && image.type() == defFrameType);

    dst_fg.create( image.size(), CV_MAKETYPE(defFrameDepth,1) );
    cv::Mat fgmask = dst_fg.getMat();

    dst_shadow.create( image.size(), CV_MAKETYPE(defFrameDepth,1) );
    cv::Mat shadow = dst_shadow.getMat();

    // запуск параллельно
    cv::parallel_for_(Range(0, frameSize_.height),
        MG1Invoker(image,bgmodel_,useBgmixture_,fgmask,shadow,defNbgmixture,defVarThreshFactor,defTau));

}

inline void SubtractorMG1::initialize(const cv::Size& size)
{
    // размер не может быть пустым
    CV_Assert( size != cv::Size());
    frameSize_ = size;
    // создание модели
    bgmodel_.create(frameSize_,CV_MAKETYPE(defModelDepth,defNbgmixture*(2 + defFrameChannels + defFrameChannels)));
    bgmodel_ = Scalar::all(0);
    // число использованных смесей
    useBgmixture_.create(frameSize_,CV_MAKETYPE(defFrameDepth,1));
    useBgmixture_ = Scalar::all(0);
    // созать пустую маску forereground (все 0)
    emptyFgmask_.create(frameSize_,CV_MAKETYPE(defFrameDepth,1));
    emptyFgmask_ = Scalar::all(0);
    // выставить флаг инициализации
    fInitBgModel_ = 1;
}

