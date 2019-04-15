#ifndef VIEWER_H
#define VIEWER_H

#include <QMdiSubWindow>
#include <QtWidgets>
#include <QBoxLayout>
#include <QImage>

#include "async/backgroundworker.h"
#include "videoproc/videoprocessortask.h"
#include "videoproc/transmit/signalvideotransmitter.h"


class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

/**
 * @brief The ImageViewer class
 * Упрощенный класс вывода видео в qt
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 */
class Viewer : public QMdiSubWindow
{
    Q_OBJECT

public:
    Viewer(const VideoProcessor::Ptr &videoProcessor,
           QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    ~Viewer();

public slots:



private:
    /**
     * @brief m_videoProcessor
     * Видеопроцессор
     */
    VideoProcessor::Ptr m_videoProcessor;
    /**
     * @brief m_videoProcessorTask
     * Асинхронная задача для видеопроцессора
     */
    VideoProcessorTask::Ptr m_videoProcessorTask;

    /**
     * @brief m_backgroundWorker
     * Исполнитель фоновых задач
     */
    BackgroundWorker m_backgroundWorker;

    /**
     * @brief m_signalVideoTransmitter
     */
    SignalVideoTransmitter::Ptr m_signalVideoTransmitter;
    /**
     * @brief m_imageLabel
     * В центральной части располагается label с картинкой (видео)
     */
    QLabel *m_imageLabel;

    QAction *m_startAction;
    QAction *m_stopAction;
    QAction *m_repeatAction;
    /**
     * @brief m_statusLabel
     * строка состояния проигрывателя
     */
    QLabel *m_statusLabel;
    /**
     * @brief m_fpsLabel
     * строка с частотой обработки кадров
     */
    QLabel *m_fpsLabel;

private:
    QToolBar *createToolBar();
    QStatusBar *createStatusBar();
    QWidget *createCentralWidget();
    SignalVideoTransmitter::Ptr createSignalVideoTransmitter();
private slots:
    void slotNoImpl();
    void slotImageUpdated(QImage img);
    void slotMessageUpdated(QString msg);
    void slotFpsUpdated(double value);
    void slotWantStartVideo();
    void slotWantStopVideo();
    void slotWantRepeatVideo(bool isRepeat);
    void slotVideoStarted(const AbstractTask::Ptr &task);
    void slotVideoStoped(const AbstractTask::Ptr &task);




};

#endif //VIEWER_H
