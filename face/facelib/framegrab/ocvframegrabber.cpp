#include "ocvframegrabber.h"

OCVFrameGrabber::OCVFrameGrabber(int device):
  m_fileName(),
  m_device(device),
  m_videoCapture()
{

}

OCVFrameGrabber::OCVFrameGrabber(std::string fileName):
  m_fileName(fileName),
  m_device(-1),
  m_videoCapture()
{

}

OCVFrameGrabber::~OCVFrameGrabber()
{
    stop();
    uninit();
    close();
}

bool OCVFrameGrabber::open()
{
    if(!m_fileName.empty()){
        // если имя файла не пустое, открываем его
        return m_videoCapture.open(m_fileName);
    }else if(m_device >= 0){
        // открываем устройство
        return m_videoCapture.open(m_device);
    }
    return m_videoCapture.isOpened();
}

bool OCVFrameGrabber::init()
{
    return m_videoCapture.isOpened();
}

bool OCVFrameGrabber::start()
{
    return m_videoCapture.isOpened();
}

bool OCVFrameGrabber::read(cv::Mat &image)
{
    return m_videoCapture.read(image);
}

void OCVFrameGrabber::stop()
{

}

void OCVFrameGrabber::uninit()
{

}

void OCVFrameGrabber::close()
{
    m_videoCapture.release();
}
