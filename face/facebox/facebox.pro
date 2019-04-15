TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp

include(..\common.pri)

FACELIB_DIR = $$shadowed(../facelib)
message($$FACELIB_DIR)
win32:CONFIG(release, debug|release): LIBS += -L$$FACELIB_DIR/release/ -lfacelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$FACELIB_DIR/debug/ -lfacelibd
else:unix:!macx: LIBS += -L$$FACELIB_DIR/../facelib/ -lfacelib

INCLUDEPATH += $$PWD/../facelib
DEPENDPATH += $$PWD/../facelib
