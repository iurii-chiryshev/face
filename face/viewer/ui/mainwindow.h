#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QMdiArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createMenuAndToolBar();
    void createStatusBar();
    void createMdiArea();

public slots:
    void slotNoImpl();
    void slotOpenVideoFile();
    void slotOpenPictures();

private:
    QMdiArea *m_mdiArea;
};

#endif // MAINWINDOW_H
