#include "videoprocessor.h"
#include <iostream>

VideoProcessor::VideoProcessor()
{
    m_frameGrabber = AbstractFrameGrabber::Null();
    m_frameDetector = AbstractFrameDetector::Null();
    m_transmitter = AbstractVideoTransmitter::Null();
    m_isRun = false;
    m_isRepeat = false;
}

void VideoProcessor::loop()
{
    cv::Mat grab_img, dysplay_img;
    setIsRun(true);

    m_transmitter->sendMessage("detector initialization..");
    if(!m_frameDetector->init()){
        m_transmitter->sendMessage("detector initialization error");
        return;
    }

    do{// цикл повторений

        m_transmitter->sendMessage("video initialization..");
        if(!(   m_frameGrabber->open() &&
                m_frameGrabber->init() &&
                m_frameGrabber->start())){
            m_transmitter->sendMessage("video initialization error");
            m_frameGrabber->stop();
            m_frameGrabber->uninit();
            m_frameGrabber->close();
            return;
        }

        double fps = 10.;
        int64 frame_count = 0; // счетчик кадров
        const int fps_stride = 10; // шаг расчета fps в кадрах
        int64 t1,t0 = cv::getTickCount(); // замеры frame rate


        while(isRun() && m_frameGrabber->read(grab_img)){ // главный цикл обработки
           if(!(grab_img.type() == CV_8UC3 || grab_img.type() == CV_8UC1)){
               m_transmitter->sendMessage("format not supported");
               break;
           }

           if(frame_count == 0) {
               m_transmitter->sendMessage("playing video..");
           }

           m_frameDetector->apply(grab_img,dysplay_img);

           if(frame_count++ % fps_stride == 0)
           {
               t1 = cv::getTickCount();
               fps = fps_stride / (double(t1-t0)/cv::getTickFrequency());
               t0 = t1;
               m_transmitter->sendFPS(fps);
               std::cout << "fps = " << fps << std::endl;
           }


           m_transmitter->sendImage(dysplay_img);
        }

        m_transmitter->sendMessage("video uninitialization..");
        m_frameGrabber->stop();
        m_frameGrabber->uninit();
        m_frameGrabber->close();

    }while(isRepeat() && isRun());


    m_frameDetector->uninit();
}



AbstractFrameGrabber::Ptr VideoProcessor::frameGrabber() const
{
    return m_frameGrabber;
}

void VideoProcessor::setFrameGrabber(const AbstractFrameGrabber::Ptr &frameGrabber)
{
    m_frameGrabber = frameGrabber;
}


AbstractVideoTransmitter::Ptr VideoProcessor::transmitter() const
{
    return m_transmitter;
}

void VideoProcessor::setTransmitter(const AbstractVideoTransmitter::Ptr &videoTransmitter)
{
    m_transmitter = videoTransmitter;
}

AbstractFrameDetector::Ptr VideoProcessor::frameDetector() const
{
    return m_frameDetector;
}

void VideoProcessor::setFrameDetector(const AbstractFrameDetector::Ptr &faceDetector)
{
    m_frameDetector = faceDetector;
}

bool VideoProcessor::isRun() const
{
    return m_isRun;
}

void VideoProcessor::setIsRun(bool isRun)
{
    m_isRun = isRun;
}

bool VideoProcessor::isRepeat() const
{
    return m_isRepeat;
}

void VideoProcessor::setIsRepeat(bool isRepeat)
{
    m_isRepeat = isRepeat;
}



