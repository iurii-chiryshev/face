#ifndef FLIB_ABSTRACTVIDEOTRANSMITTER_H
#define FLIB_ABSTRACTVIDEOTRANSMITTER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "facelib_global.h"

/**
 * @brief The AbstractVideoTransmitter class
 */
class FACELIBSHARED_EXPORT AbstractVideoTransmitter
{
public:
    typedef std::shared_ptr<AbstractVideoTransmitter> Ptr;
    AbstractVideoTransmitter();
    virtual ~AbstractVideoTransmitter();
    /**
     * @brief sendImage
     * @param img
     * отправить очередное изображение видеопотока
     */
    virtual void sendImage(const cv::Mat &img) = 0;
    /**
     * @brief sendMessage
     * @param msg
     * отправить сообщение о своем статусе
     */
    virtual void sendMessage(const std::string &msg) = 0;
    /**
     * @brief sendFPS
     * @param value
     * отправить frame rate
     */
    virtual void sendFPS(double value) = 0;

public:
    /**
     * @brief Null
     * @return - заглушка трансмиттера
     */
    static AbstractVideoTransmitter::Ptr Null();
};


/**
 * @brief The NullVideoTransmitter class
 * Заглушка
 */
class FACELIBSHARED_EXPORT NullVideoTransmitter: public AbstractVideoTransmitter
{

public:
    typedef std::shared_ptr<NullVideoTransmitter> Ptr;
    NullVideoTransmitter();
    // AbstractVideoTransmitter interface
public:
    virtual void sendImage(const cv::Mat &img);
    virtual void sendMessage(const std::string &msg);
    virtual void sendFPS(double value);
};

#endif // FLIB_ABSTRACTVIDEOTRANSMITTER_H
