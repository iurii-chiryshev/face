#ifndef FLIB_ABSTRACTFRAMEDETECTOR_H
#define FLIB_ABSTRACTFRAMEDETECTOR_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "transmit/abstractfacetransmitter.h"
#include "facelib_global.h"

/**
 * @brief The AbstractFrameDetector class
 * Абстрактный класс обработчиков кадров по детектированию, трекингу лиц, голов людей, самих людей
 */
class FACELIBSHARED_EXPORT AbstractFrameDetector
{
public:
    /**
     * @brief Ptr
     * smart pointer type
     */
    typedef std::shared_ptr<AbstractFrameDetector> Ptr;
    /**
     * @brief AbstractFrameDetector
     * @param faceTransmitter
     */
    AbstractFrameDetector();
    /**
     * @brief ~AbstractFrameDetector
     * dtor
     */
    virtual ~AbstractFrameDetector();
    /**
     * @brief apply
     * @param[in] src входной кадр
     * @param[out] dst выходная картинка
     * Обработать картинку
     */
    virtual void apply(const cv::Mat &src, cv::Mat &dst) = 0;

    /**
     * @brief init
     * @return
     * Инициализация
     */
    virtual bool init() = 0;
    /**
     * @brief uninit
     * деинициализация
     */
    virtual void uninit() = 0;
    /**
     * @brief Null
     * @return заглушку
     * @see NullFrameDetector
     */
    static AbstractFrameDetector::Ptr Null();


};


/**
 * @brief The NullFrameDetector class
 * Заглушка обработчика кадров.
 * Что получил на вход, то и вернет на выходе без обработки.
 */
class FACELIBSHARED_EXPORT NullFrameDetector: public AbstractFrameDetector
{
public:
    /**
     * @brief Ptr
     * self smart pointer type
     */
    typedef std::shared_ptr<NullFrameDetector> Ptr;
    /**
     * @brief NullFrameDetector
     * ctor
     */
    NullFrameDetector();

    // AbstractFrameDetector interface
public:
    /**
     * @brief apply
     * @param[in] src
     * @param[out] dst
     * Stub w/o processing.
     */
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    /**
     * @brief init
     * @return true
     */
    virtual bool init();
    virtual void uninit();
};

#endif // FLIB_ABSTRACTFRAMEDETECTOR_H
