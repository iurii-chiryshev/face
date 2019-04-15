#include "abstractframegrabber.h"

AbstractFrameGrabber::AbstractFrameGrabber()
{

}

AbstractFrameGrabber::~AbstractFrameGrabber()
{

}

AbstractFrameGrabber::Ptr AbstractFrameGrabber::Null()
{
    return AbstractFrameGrabber::Ptr(new NullFrameGrabber());
}


NullFrameGrabber::NullFrameGrabber()
{

}

bool NullFrameGrabber::open()
{
    return false;
}

bool NullFrameGrabber::init()
{
    return false;
}

bool NullFrameGrabber::start()
{
    return false;
}

bool NullFrameGrabber::read(cv::Mat &image)
{
    return false;
}

void NullFrameGrabber::stop()
{
}

void NullFrameGrabber::uninit()
{
}

void NullFrameGrabber::close()
{
}
