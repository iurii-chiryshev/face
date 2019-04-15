HOST_NAME = $$QMAKE_HOST.name
#-------------------------------------------------
#   Chiryshev Iurii <iurii.chiryshev@mail.ru>
#   DESKTOP-QRN46PP - home pc
#   DESKTOP-48BO0EE - work pc
#-------------------------------------------------
win32:contains(HOST_NAME,DESKTOP-QRN46PP|DESKTOP-48BO0EE) {
    message( "Chiryshev Iurii $$HOST_NAME detected" )

    #-------------------------------------------------
    #               Boost
    #-------------------------------------------------
    BOOST_DIR = 'C:\Program Files\boost_1_61_0'
    BOOST_INCLUDE = $$BOOST_DIR
    BOOST_LIB = $$BOOST_DIR\lib64-msvc-14.0
	
    #-------------------------------------------------
    #               OpenCV
    #-------------------------------------------------
    OPENCV_DIR = 'D:\lib\opencv-2.4.13\build'
    OPENCV_LIB = $$OPENCV_DIR\x64\vc12\lib
    OPENCV_INCLUDE = $$OPENCV_DIR\include
	
    #-------------------------------------------------
    #               dlib
    #-------------------------------------------------
    DLIB_DIR = 'C:\Program Files\dlib-19.4'
    DLIB_LIB = $$DLIB_DIR\lib
    DLIB_INCLUDE = $$DLIB_DIR\include

    #-------------------------------------------------
    #               CUDA
    #-------------------------------------------------
    CUDA_DIR = 'C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v8.0'
    CUDA_LIB = $$CUDA_DIR\lib\x64
    CUDA_INCLUDE = $$CUDA_DIR\include

    #-------------------------------------------------
    #               all together
    #-------------------------------------------------
    INCLUDEPATH +=  $$BOOST_INCLUDE \
                    $$OPENCV_INCLUDE \
                    $$DLIB_INCLUDE \
                    $$CUDA_INCLUDE

    QMAKE_LIBDIR += $$BOOST_LIB \
                    $$OPENCV_LIB \
                    $$DLIB_LIB \
                    $$CUDA_LIB

    CONFIG(debug, debug|release) {
        #debug
        LIBS += $$files($$BOOST_LIB\libboost_*-mt-gd-1_61.lib) \
                $$files($$OPENCV_LIB\*2413d.lib) \
                $$DLIB_LIB\dlibd.lib \
                $$files($$CUDA_LIB\*.lib)
    } else {
        #release
        LIBS += $$files($$BOOST_LIB\libboost_*-mt-1_61.lib) \
                $$files($$OPENCV_LIB\*2413.lib)\
                $$DLIB_LIB\dlib.lib \
                $$files($$CUDA_LIB\*.lib)
    }
} # Chiryshev Iurii PCs
