#ifndef FLIB_VIDEOPROCESSORTASK_H
#define FLIB_VIDEOPROCESSORTASK_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDebug>

#include "videoprocessor.h"
#include "async/abstracttask.h"
#include "frameproc/abstractframedetector.h"
#include "util/mat2qimage.h"
#include "facelib_global.h"

/**
 * @brief The VideoProcessorTask class
 * Класс, позволяет создать и запустить процесс видео как фоновую задачу
 */
class FACELIBSHARED_EXPORT VideoProcessorTask : public AbstractTask
{
    Q_OBJECT
public:
    typedef QSharedPointer<VideoProcessorTask> Ptr;

    /**
     * @brief VideoProcessorTask
     * @param videoProcessor - процессор видео
     * @param parent
     * ctor
     */
    explicit VideoProcessorTask(const VideoProcessor::Ptr &videoProcessor, QObject *parent = 0);

    // AbstractTask interface
public:
    virtual void run();
    virtual QString getName();

private:
    /**
     * @brief m_videoProcessor
     * Обработчик видео
     */
    VideoProcessor::Ptr m_videoProcessor;

};

#endif // VIDEOPROCESSORTASK_H
