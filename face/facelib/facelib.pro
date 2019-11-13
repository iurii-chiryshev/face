#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T11:03:45
#
#-------------------------------------------------

QT       += core gui

CONFIG(debug, debug|release) {
    TARGET = facelibd
} else {
    TARGET = facelib
}



TEMPLATE = lib
CONFIG += c++11
DEFINES += FACELIB_LIBRARY

include(..\common.pri)

SOURCES += \
    async/abstracttask.cpp \
    async/backgroundworker.cpp \
    async/runnabletask.cpp \
    framegrab/abstractframegrabber.cpp \
    framegrab/ocvframegrabber.cpp \
    frameproc/face/facedetector.cpp \
    frameproc/face/faceposeestimator.cpp \
    frameproc/abstractframedetector.cpp \
    util/mat2qimage.cpp \
    util/singleton.cpp \
    videoproc/transmit/abstractvideotransmitter.cpp \
    videoproc/transmit/signalvideotransmitter.cpp \
    videoproc/videoprocessor.cpp \
    videoproc/videoprocessortask.cpp \
    util/io.cpp \
    util/settings.cpp \
    util/timestamp.cpp \
    frameproc/face/fastfacedetector.cpp \
    frameproc/face/abstractfacedetector.cpp

HEADERS +=\
        facelib_global.h \
    async/abstracttask.h \
    async/backgroundworker.h \
    async/runnabletask.h \
    framegrab/abstractframegrabber.h \
    framegrab/ocvframegrabber.h \
    frameproc/face/facedetector.h \
    frameproc/face/faceposeestimator.h \
    frameproc/abstractframedetector.h \
    util/mat2qimage.h \
    util/singleton.h \
    videoproc/transmit/abstractvideotransmitter.h \
    videoproc/transmit/signalvideotransmitter.h \
    videoproc/videoprocessor.h \
    videoproc/videoprocessortask.h \
    util/io.h \
    util/settings.h \
    util/timestamp.h \
    frameproc/face/fastfacedetector.h \
    frameproc/face/abstractfacedetector.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

