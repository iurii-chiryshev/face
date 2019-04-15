#include "abstractframedetector.h"

#include <thread>
#include <chrono>

AbstractFrameDetector::AbstractFrameDetector()
{

}

AbstractFrameDetector::~AbstractFrameDetector()
{

}

AbstractFrameDetector::Ptr AbstractFrameDetector::Null()
{
    return AbstractFrameDetector::Ptr(new NullFrameDetector());
}




NullFrameDetector::NullFrameDetector()
{

}

void NullFrameDetector::apply(const cv::Mat &src, cv::Mat &dst)
{
    // никакого расчета здесь нет -> просто инициализировать dst
    // и заснуть
    if(src.empty()) return;
    if(!(src.type() == CV_8UC1 || src.type() == CV_8UC3)) return;
    src.copyTo(dst);
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

bool NullFrameDetector::init()
{
    return true;
}


void NullFrameDetector::uninit()
{

}
