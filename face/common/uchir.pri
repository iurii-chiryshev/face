HOST_NAME = $$QMAKE_HOST.name
#-------------------------------------------------
#   Chiryshev Iurii <iurii.chiryshev@mail.ru>
#   DESKTOP-QRN46PP - home pc
#   DESKTOP-48BO0EE - work pc
#-------------------------------------------------
win32:contains(HOST_NAME,DESKTOP-QRN46PP|DESKTOP-48BO0EE) {
    message( "Chiryshev Iurii $$HOST_NAME detected" )
	
    #-------------------------------------------------
    #               Eigen
    #-------------------------------------------------
    EIGEN_DIR = 'C:\Program Files (x86)\PCL 1.8.0\3rdParty\Eigen'
    EIGEN_INCLUDE = $$EIGEN_DIR\eigen3
	
    #-------------------------------------------------
    #               Boost
    #-------------------------------------------------
    BOOST_DIR = 'C:\Program Files (x86)\PCL 1.8.0\3rdParty\Boost'
    BOOST_INCLUDE = $$BOOST_DIR\include\boost-1_61
    BOOST_LIB = $$BOOST_DIR\lib
	
    #-------------------------------------------------
    #               FLANN
    #-------------------------------------------------
    FLANN_DIR = 'C:\Program Files (x86)\PCL 1.8.0\3rdParty\FLANN'
    FLANN_INCLUDE = $$FLANN_DIR\include
    FLANN_LIB = $$FLANN_DIR\lib

    #-------------------------------------------------
    #               OpenCV
    #-------------------------------------------------
    OPENCV_DIR = 'D:\lib\opencv-2.4.13\build'
    OPENCV_LIB = $$OPENCV_DIR\x86\vc12\lib
    OPENCV_INCLUDE = $$OPENCV_DIR\include
	
    #-------------------------------------------------
    #               dlib
    #-------------------------------------------------
    DLIB_DIR = 'C:\Program Files (x86)\dlib-19.4'
    DLIB_LIB = $$DLIB_DIR\lib
    DLIB_INCLUDE = $$DLIB_DIR\include

    #-------------------------------------------------
    #               ZeroMQ
    #-------------------------------------------------
    ZMQ_DIR = 'C:\Program Files (x86)\ZeroMQ'
    ZMQ_LIB = $$ZMQ_DIR\lib
    ZMQ_INCLUDE = $$ZMQ_DIR\include

    #-------------------------------------------------
    #               google protobuf
    #-------------------------------------------------
    PROTOBUF_DIR = 'C:\Program Files (x86)\protobuf.2.6.1.1\build\native'
    PROTOBUF_LIB = $$PROTOBUF_DIR\lib\Win32\v140
    PROTOBUF_INCLUDE = $$PROTOBUF_DIR\include




    #-------------------------------------------------
    #               all together
    #-------------------------------------------------
    INCLUDEPATH += $$EIGEN_INCLUDE \
                   $$BOOST_INCLUDE \
                   $$FLANN_INCLUDE \
                   $$OPENCV_INCLUDE \
                   $$DLIB_INCLUDE \
                   $$ZMQ_INCLUDE \
                   $$PROTOBUF_INCLUDE

    QMAKE_LIBDIR += $$BOOST_LIB \
                    $$FLANN_LIB \
                    $$OPENCV_LIB \
                    $$DLIB_LIB \
                    $$ZMQ_LIB


    CONFIG(debug, debug|release) {
        #debug
        LIBS += $$files($$BOOST_LIB\*-mt-gd-1_61.lib) \
                $$files($$OPENCV_LIB\*2413d.lib) \
                $$DLIB_LIB\dlibd.lib \
                $$ZMQ_LIB\libzmq-v140-mt-gd-4_2_1.lib \
                $$PROTOBUF_LIB\Debug\libprotobuf.lib
    } else {
        #release
        LIBS += $$files($$BOOST_LIB\*-mt-1_61.lib) \
                $$files($$OPENCV_LIB\*2413.lib)\
                $$DLIB_LIB\dlib.lib \
                $$ZMQ_LIB\libzmq-v140-mt-4_2_1.lib \
                $$PROTOBUF_LIB\Release\libprotobuf.lib
    }
} # Chiryshev Iurii PCs
