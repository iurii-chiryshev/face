#-------------------------------------------------
#
# Project created by QtCreator 2017-08-14T12:17:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = testfacelib
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    testsettings.cpp \
    main.cpp
HEADERS += \
    testsettings.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

# полный путь до папки где собралась facelib (.lib .dll)
FACELIB_DIR = $$shadowed(../facelib)
message($$FACELIB_DIR)
win32:CONFIG(release, debug|release): LIBS += -L$$FACELIB_DIR/release/ -lfacelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$FACELIB_DIR/debug/ -lfacelibd
else:unix:!macx: LIBS += -L$$FACELIB_DIR/../facelib/ -lfacelib

INCLUDEPATH += $$PWD/../facelib
DEPENDPATH += $$PWD/../facelib


