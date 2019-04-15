#ifndef FLIB_FASTFACEDETECTOR_H
#define FLIB_FASTFACEDETECTOR_H

#include "abstractfacedetector.h"

/**
 * @brief The FastFaceDetector class
 * Хочу поэкспериментировать c быстрой реализацией детектора лиц
 */
class FACELIBSHARED_EXPORT FastFaceDetector: public AbstractFaceDetector
{
public:
    typedef std::shared_ptr<FastFaceDetector> Ptr;
    FastFaceDetector();

    // AbstractFrameDetector interface
public:
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    virtual bool init();
    virtual void uninit();
};


#endif // FLIB_FASTFACEDETECTOR_H
