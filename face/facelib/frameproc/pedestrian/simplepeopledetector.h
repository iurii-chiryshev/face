#ifndef FLIB_SIMPLEPEOPLEDETECTOR_H
#define FLIB_SIMPLEPEOPLEDETECTOR_H

#include "frameproc/abstractframedetector.h"
#include "facelib_global.h"

using namespace cv;
using namespace std;


/**
 * @brief The SimplePeopleDetector class
 * Простой детектор людей на основе HOG
 */
class FACELIBSHARED_EXPORT SimplePeopleDetector: public AbstractFrameDetector
{
public:
    typedef std::shared_ptr<SimplePeopleDetector> Ptr;
    SimplePeopleDetector();

    // AbstractFrameDetector interface
public:
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    virtual bool init();
    virtual void uninit();
protected:
    HOGDescriptor m_hog;
    cv::Mat m_gray;
    std::vector<float> m_defaultDescriptor;
    std::vector<float> m_daimlerDescriptor;
};

#endif // FLIB_SIMPLEPEOPLEDETECTOR_H
