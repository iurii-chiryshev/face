#include "mainwindow.h"

#include "viewer.h"
#include "framegrab/ocvframegrabber.h"
#include "util/io.h"
#include "frameproc/face/facedetector.h"

static bool firstDialog = true;
static void initFileDialog(QFileDialog &dialog,const QStringList &filters)
{
    if (firstDialog) {
        firstDialog = false;
        const QStringList moviesLocations = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
        dialog.setDirectory(moviesLocations.isEmpty() ? QDir::currentPath() : moviesLocations.last());
    }
    dialog.setNameFilters(filters);

}

static QString selectExistingDirectory(QWidget *parent){
    QString caption(QObject::tr("Open Directory"));
    QFileDialog::Options options = QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks;
    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        return QFileDialog::getExistingDirectory(parent,
                                   caption,
                                   picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last(),
                                   options);
    }
    return QFileDialog::getExistingDirectory(parent,
                               caption,
                               QString(),
                               options);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon("://icons/FatCow_Icons32x32/xfn.png"));
    setWindowTitle(tr("FaceViewer"));

    setMinimumSize(800,600);

    createMdiArea();

    createMenuAndToolBar();

    createStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenuAndToolBar()
{


    //Menu->File
    QMenu *pmnuFile   = new QMenu(tr("File"));
    //Menu->File->Open
    QMenu *pmnuOpen = new QMenu(tr("Open"), pmnuFile);
    pmnuOpen->setIcon(QIcon("://icons/FatCow_Icons32x32/folder.png"));
    pmnuFile->addMenu(pmnuOpen);
    QAction *pOpenFromVideoFile = pmnuOpen->addAction(QIcon("://icons/FatCow_Icons32x32/film.png"), tr("Video file"), this, SLOT(slotOpenVideoFile()));
    QAction *pOpenFromPictures = pmnuOpen->addAction(QIcon("://icons/FatCow_Icons32x32/pictures.png"), tr("Pictures"), this, SLOT(slotOpenPictures()));
    //Menu->File->Attach
    QMenu *pmnuAttach = new QMenu(tr("Attach"), pmnuFile);
    pmnuAttach->setIcon(QIcon("://icons/FatCow_Icons32x32/connect.png"));
    pmnuFile->addMenu(pmnuAttach);
    QAction *pAttachFromWebcam = pmnuAttach->addAction(QIcon(":/icons/FatCow_Icons32x32/webcam.png"), tr("Webcam"), this, SLOT(slotNoImpl()));
    QAction *pAttachFromCCTVcam = pmnuAttach->addAction(QIcon(":/icons/FatCow_Icons32x32/cctv_camera.png"), tr("CCTV camera"), this, SLOT(slotNoImpl()));
    //Menu->File->Exit
    pmnuFile->addSeparator();
    pmnuFile->addAction(tr("Exit"), this, SLOT(close()));
    // Menu->Windows
    QMenu *pmnuWindows   = new QMenu(tr("Windows"));
    QAction *pWindowsCascade = pmnuWindows->addAction(QIcon("://icons/FatCow_Icons32x32/application_cascade.png"), tr("Cascade"), this->m_mdiArea, SLOT(cascadeSubWindows()));
    QAction *pWindowsTile = pmnuWindows->addAction(QIcon("://icons/FatCow_Icons32x32/application_tile_horizontal.png"), tr("Tile"), this->m_mdiArea, SLOT(tileSubWindows()));

    // Add
    this->menuBar()->addMenu(pmnuFile);
    this->menuBar()->addMenu(pmnuWindows);

    //ToolBar
    QToolBar *ptb = new QToolBar(tr("Toolbar"));
    //Open
    QToolButton * ptbOpen = new QToolButton(ptb);
    ptbOpen->setIcon(pmnuOpen->icon());
    ptb->addWidget(ptbOpen);
    ptbOpen->setMenu(pmnuOpen);
    ptbOpen->setPopupMode(QToolButton::MenuButtonPopup);
    //Attach
    QToolButton * ptbAttach = new QToolButton(ptb);
    ptbAttach->setIcon(pmnuAttach->icon());
    ptb->addWidget(ptbAttach);
    ptbAttach->setMenu(pmnuAttach);
    ptbAttach->setPopupMode(QToolButton::MenuButtonPopup);
    ptb->addSeparator();
    // Cascade/Tile
    ptb->addAction(pWindowsCascade);
    ptb->addAction(pWindowsTile);
    ptb->addSeparator();
    this->addToolBar(Qt::TopToolBarArea, ptb);

}

void MainWindow::createStatusBar()
{
    QStatusBar *statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);
}

void MainWindow::createMdiArea()
{
    m_mdiArea = new QMdiArea();
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setCentralWidget(m_mdiArea);

}

void MainWindow::slotNoImpl()
{
    QMessageBox::information(0, "Message", "Not implemented");
}

void MainWindow::slotOpenVideoFile(){
    QFileDialog dialog(this, tr("Open Video"));
    initFileDialog(dialog,QStringList("Video files (*.avi *.wmv)"));

    if(dialog.exec() == QDialog::Accepted){
        QString fileName = dialog.selectedFiles().first();
        AbstractFrameGrabber::Ptr fg(new OCVFrameGrabber(fileName.toStdString()));
        AbstractFrameDetector::Ptr fd(new FaceDetector());
        VideoProcessor::Ptr vp(new VideoProcessor());
        vp->setFrameDetector(fd);
        vp->setFrameGrabber(fg);

        Viewer * viewer = new Viewer(vp);
        this->m_mdiArea->addSubWindow(viewer);
        viewer->show();
    }
}

void MainWindow::slotOpenPictures()
{
    QString dir = selectExistingDirectory(this);
    if(!dir.isNull() && !dir.isEmpty()){
        qDebug(dir.toStdString().c_str());
        std::string fileName = flib::getCVCaptureFile(dir.toStdString() + "/");
        qDebug(fileName.c_str());
        AbstractFrameGrabber::Ptr fg(new OCVFrameGrabber(fileName));
        AbstractFrameDetector::Ptr fd(new FaceDetector());
        VideoProcessor::Ptr vp(new VideoProcessor());
        vp->setFrameDetector(fd);
        vp->setFrameGrabber(fg);

        Viewer * viewer = new Viewer(vp);
        this->m_mdiArea->addSubWindow(viewer);
        viewer->show();
    }
}

