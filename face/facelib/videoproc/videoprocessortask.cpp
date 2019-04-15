#include "videoprocessortask.h"
#include <algorithm>


VideoProcessorTask::VideoProcessorTask(const VideoProcessor::Ptr &videoProcessor, QObject *parent) : AbstractTask(parent)
 ,m_videoProcessor(videoProcessor)
{

}

void VideoProcessorTask::run()
{
    if(m_videoProcessor){
        m_videoProcessor->loop();
    }
}

QString VideoProcessorTask::getName()
{
    return tr("Video Processor");
}
