#include "viewer.h"

Viewer::Viewer(const VideoProcessor::Ptr &videoProcessor,
               QWidget *parent,
               Qt::WindowFlags flags): QMdiSubWindow(parent,flags)
  ,m_videoProcessor(videoProcessor)
  ,m_videoProcessorTask(new VideoProcessorTask(videoProcessor))
{
    setWindowIcon(QIcon("://icons/FatCow_Icons32x32/cctv_camera.png"));
    setMinimumSize(400,300);
    this->layout()->setMenuBar(createToolBar());
    this->layout()->addWidget(createCentralWidget());
    this->layout()->addWidget(createStatusBar());

    m_videoProcessor->setTransmitter(createSignalVideoTransmitter());

    connect(&m_backgroundWorker,
            SIGNAL(started(const AbstractTask::Ptr&)),
            this,
            SLOT(slotVideoStarted(const AbstractTask::Ptr&)));
    connect(&m_backgroundWorker,
            SIGNAL(finished(const AbstractTask::Ptr&)),
            this,
            SLOT(slotVideoStoped(const AbstractTask::Ptr&)));

    slotWantStartVideo();
}

Viewer::~Viewer()
{

}



QToolBar *Viewer::createToolBar(){
    QToolBar *toolBar = new QToolBar(this);
    m_startAction = toolBar->addAction(QIcon(":/icons/FatCow_Icons32x32/control_play_blue.png"), tr("Play"), this, SLOT(slotWantStartVideo()));
    m_stopAction = toolBar->addAction(QIcon(":/icons/FatCow_Icons32x32/control_stop_blue.png"), tr("Stop"), this, SLOT(slotWantStopVideo()));
    toolBar->addSeparator();
    toolBar->addAction(QIcon("://icons/FatCow_Icons32x32/resultset_first.png"), tr("First frame"), this, SLOT(slotNoImpl()));
    toolBar->addAction(QIcon("://icons/FatCow_Icons32x32/resultset_previous.png"), tr("Previous frame"), this, SLOT(slotNoImpl()));
    toolBar->addAction(QIcon("://icons/FatCow_Icons32x32/resultset_next.png"), tr("Next frame"), this, SLOT(slotNoImpl()));
    toolBar->addAction(QIcon("://icons/FatCow_Icons32x32/resultset_last.png"), tr("Last frame"), this, SLOT(slotNoImpl()));
    toolBar->addSeparator();
    m_repeatAction = toolBar->addAction(QIcon("://icons/FatCow_Icons32x32/control_repeat_blue.png"), tr("Repeat"),this,SLOT(slotWantRepeatVideo(bool)));
    m_repeatAction->setCheckable(true);
    m_repeatAction->setChecked(true);
    return toolBar;
}

QStatusBar *Viewer::createStatusBar()
{
    QStatusBar *statusBar = new QStatusBar();
    statusBar->setMaximumHeight(21);

    m_fpsLabel = new QLabel("FPS: ",this);
    m_fpsLabel->setMinimumWidth(50);
    statusBar->addWidget(m_fpsLabel);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setMinimumWidth(50);
    statusBar->addWidget(m_statusLabel);

    return statusBar;
}

QWidget *Viewer::createCentralWidget()
{
    m_imageLabel = new QLabel(this);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(new QHBoxLayout());
    centralWidget->layout()->addWidget(m_imageLabel);
    centralWidget->layout()->setMargin(10);
    centralWidget->setStyleSheet("background-color: black");
    return centralWidget;
}

SignalVideoTransmitter::Ptr Viewer::createSignalVideoTransmitter()
{
    m_signalVideoTransmitter = SignalVideoTransmitter::Ptr(new SignalVideoTransmitter());
    connect(m_signalVideoTransmitter.get(),
                SIGNAL(image(QImage)),
                this,
                SLOT(slotImageUpdated(QImage)));
    connect(m_signalVideoTransmitter.get(),
            SIGNAL(message(QString)),
            this,
            SLOT(slotMessageUpdated(QString)));
    connect(m_signalVideoTransmitter.get(),
            SIGNAL(fps(double)),
            this,
            SLOT(slotFpsUpdated(double)));
    return m_signalVideoTransmitter;
}

void Viewer::slotNoImpl()
{
    QMessageBox::information(0, "Message", "Not implemented");
}

void Viewer::slotImageUpdated(QImage img){
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setPixmap(QPixmap::fromImage(img).scaled( m_imageLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}

void Viewer::slotMessageUpdated(QString msg)
{
    m_statusLabel->setText(msg);
}

void Viewer::slotFpsUpdated(double value)
{
    m_fpsLabel->setText("FPS: "  + QString::number(value,'f',1));
}

void Viewer::slotWantStartVideo()
{
    m_startAction->setEnabled(false);
    m_videoProcessor->setIsRepeat(m_repeatAction->isChecked());
    m_backgroundWorker.start(m_videoProcessorTask);
}

void Viewer::slotWantStopVideo()
{
    m_videoProcessor->setIsRun(false);
}

void Viewer::slotWantRepeatVideo(bool isRepeat)
{
    m_videoProcessor->setIsRepeat(isRepeat);
}

void Viewer::slotVideoStarted(const AbstractTask::Ptr &task)
{

}

void Viewer::slotVideoStoped(const AbstractTask::Ptr &task)
{
    m_startAction->setEnabled(true);
}


