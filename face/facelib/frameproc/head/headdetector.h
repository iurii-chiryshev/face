#ifndef FLIB_HEADDETECTOR_H
#define FLIB_HEADDETECTOR_H

#include "frameproc/abstractframedetector.h"
#include "facelib_global.h"

/**
 * @brief The HeadDetector class
 * По ходу событий детектировать нужно не просто лица людей, а их головы целиком
 * Этот класс как раз этим и занимается
 * todo - реализовать
 */
class FACELIBSHARED_EXPORT HeadDetector: public AbstractFrameDetector
{
public:
    typedef std::shared_ptr<HeadDetector> Ptr;
    HeadDetector();

    // AbstractFrameDetector interface
public:
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    virtual bool init();
};

#endif // FLIB_HEADDETECTOR_H
