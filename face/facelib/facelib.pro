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
    frameproc/bgs/backgroundsubtractor.cpp \
    frameproc/face/facedetector.cpp \
    frameproc/face/faceposeestimator.cpp \
    frameproc/head/headdetector.cpp \
    frameproc/pedestrian/simplepeopledetector.cpp \
    frameproc/transmit/signalfacetransmitter.cpp \
    frameproc/transmit/zmqfacetransmitter.cpp \
    frameproc/abstractframedetector.cpp \
    improc/_ccl_sauf.cpp \
    improc/connectedcomponents.cpp \
    subtract/subtractormg1.cpp \
    subtract/subtractormog2p.cpp \
    subtract/subtractorog1.cpp \
    track/blob.cpp \
    track/blobmap.cpp \
    track/blobtrack.cpp \
    util/mat2qimage.cpp \
    util/singleton.cpp \
    videoproc/transmit/abstractvideotransmitter.cpp \
    videoproc/transmit/signalvideotransmitter.cpp \
    videoproc/transmit/zmqvideotransmitter.cpp \
    videoproc/videoprocessor.cpp \
    videoproc/videoprocessortask.cpp \
    zmq/zmqconfig.cpp \
    util/io.cpp \
    util/settings.cpp \
    proto/detectionresult.pb.cc \
    proto/face.pb.cc \
    proto/landmark.pb.cc \
    proto/mat.pb.cc \
    proto/pair32i.pb.cc \
    proto/point2i.pb.cc \
    proto/rect.pb.cc \
    proto/timestamp.pb.cc \
    proto/common.pb.cc \
    frameproc/transmit/abstractfacetransmitter.cpp \
    util/timestamp.cpp \
    mongo/entity/abstractmongoentity.cpp \
    frameproc/face/fastfacedetector.cpp \
    frameproc/face/abstractfacedetector.cpp

HEADERS +=\
        facelib_global.h \
    async/abstracttask.h \
    async/backgroundworker.h \
    async/runnabletask.h \
    framegrab/abstractframegrabber.h \
    framegrab/ocvframegrabber.h \
    frameproc/bgs/backgroundsubtractor.h \
    frameproc/face/facedetector.h \
    frameproc/face/faceposeestimator.h \
    frameproc/head/headdetector.h \
    frameproc/pedestrian/simplepeopledetector.h \
    frameproc/transmit/signalfacetransmitter.h \
    frameproc/transmit/zmqfacetransmitter.h \
    frameproc/abstractframedetector.h \
    improc/_ccl_sauf.h \
    improc/connectedcomponents.h \
    subtract/subtractor.h \
    track/_blobmap.h \
    track/blob.h \
    track/blobtrack.h \
    util/mat2qimage.h \
    util/singleton.h \
    videoproc/transmit/abstractvideotransmitter.h \
    videoproc/transmit/signalvideotransmitter.h \
    videoproc/transmit/zmqvideotransmitter.h \
    videoproc/videoprocessor.h \
    videoproc/videoprocessortask.h \
    zmq/zmqconfig.h \
    util/io.h \
    util/settings.h \
    proto/detectionresult.pb.h \
    proto/face.pb.h \
    proto/landmark.pb.h \
    proto/mat.pb.h \
    proto/pair32i.pb.h \
    proto/point2i.pb.h \
    proto/rect.pb.h \
    proto/timestamp.pb.h \
    proto/common.pb.h \
    frameproc/transmit/abstractfacetransmitter.h \
    util/timestamp.h \
    mongo/entity/abstractmongoentity.h \
    frameproc/face/fastfacedetector.h \
    frameproc/face/abstractfacedetector.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    proto/face.proto \
    proto/detectionresult.proto \
    proto/landmark.proto \
    proto/mat.proto \
    proto/pair32i.proto \
    proto/point2i.proto \
    proto/rect.proto \
    proto/timestamp.proto \
    proto/uchir.bat

