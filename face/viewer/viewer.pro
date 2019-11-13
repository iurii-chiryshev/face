#-------------------------------------------------
#
# Project created by QtCreator 2017-06-20T15:31:51
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG(debug, debug|release) {
    TARGET = viewerd
} else {
    TARGET = viewer
}
CONFIG += console
TEMPLATE = app

include(..\common.pri)


SOURCES += main.cpp\
    ui/mainwindow.cpp \
    ui/viewer.cpp \

HEADERS  += ui/mainwindow.h \
    ui/viewer.h \


RESOURCES += \
    icons.qrc



# полный путь до папки где собралась facelib (.lib .dll)
FACELIB_DIR = $$PWD/../bin/$${SPEC}
message($$FACELIB_DIR)
win32:CONFIG(release, debug|release): LIBS += -L$$FACELIB_DIR/Release/ -lfacelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$FACELIB_DIR/Debug/ -lfacelibd

INCLUDEPATH += $$PWD/../facelib
DEPENDPATH += $$PWD/../facelib
