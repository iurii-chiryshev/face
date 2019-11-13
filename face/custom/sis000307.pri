HOST_NAME = $$QMAKE_HOST.name

message( "Chiryshev Iurii $$HOST_NAME detected" )

3RDPARTY_DIR= $$PWD/../3rdparty

#-------------------------------------------------
#               Eigen
#-------------------------------------------------
EIGEN_DIR = $$3RDPARTY_DIR\Eigen
EIGEN_INCLUDE = $$EIGEN_DIR\eigen3

#-------------------------------------------------
#               OpenCV
#-------------------------------------------------
OPENCV_DIR = $$3RDPARTY_DIR\opencv\opencv-3.4.6\build
OPENCV_LIB = $$OPENCV_DIR\x64\vc14\lib
OPENCV_INCLUDE = $$OPENCV_DIR\include

#-------------------------------------------------
#               dlib
#-------------------------------------------------
DLIB_DIR = $$3RDPARTY_DIR\dlib\dlib-19.18\build
DLIB_LIB = $$DLIB_DIR\x64\vc14\lib
DLIB_INCLUDE = $$DLIB_DIR\include


#-------------------------------------------------
#               all together
#-------------------------------------------------
INCLUDEPATH += $$EIGEN_INCLUDE \
               $$OPENCV_INCLUDE \
               $$DLIB_INCLUDE

QMAKE_LIBDIR += $$OPENCV_LIB \
                $$DLIB_LIB


CONFIG(debug, debug|release) {
    #debug
    LIBS += $$files($$OPENCV_LIB\*346d.lib) \
            $$DLIB_LIB\dlib19.18.0_debug_64bit_msvc1900.lib
} else {
    #release
    LIBS += $$files($$OPENCV_LIB\*346.lib)\
            $$DLIB_LIB\dlib19.18.0_release_64bit_msvc1900.lib
}


