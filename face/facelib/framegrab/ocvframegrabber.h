#ifndef FLIB_OCVFRAMEGRABBER_H
#define FLIB_OCVFRAMEGRABBER_H

#include "abstractframegrabber.h"
#include "facelib_global.h"

/**
 * @brief The OCVFrameGrabber class
 * Класс видеозахвата через opencv cv::VideoCapture
 * В нем реализована поддержка многих видео форматов и устройств.
 * И все же, для специфических устройств, наверное, придется писать свою реализацию
 */
class FACELIBSHARED_EXPORT OCVFrameGrabber: public AbstractFrameGrabber
{
public:
    /**
     * @brief Ptr
     * smaprt pointer type
     */
    typedef std::shared_ptr<OCVFrameGrabber> Ptr;
    /**
     * @brief OCVFrameGrabber
     * @param device - устройство
     * @see ctor cv::VideoCarture
     * ctor
     */
    OCVFrameGrabber(int device = -1);
    /**
     * @brief OCVFrameGrabber
     * @param fileName - имя файла
     * @see ctor cv::VideoCarture class
     * ctor
     */
    OCVFrameGrabber(std::string fileName);
    /**
     * @brief ~OCVFrameGrabber
     * dtor
     */
    virtual ~OCVFrameGrabber();
    // AbstractFrameGrabber interface
public:
    virtual bool open();
    virtual bool init();
    virtual bool start();
    virtual bool read(cv::Mat &image);
    virtual void stop();
    virtual void uninit();
    virtual void close();
private:
    /**
     * @brief m_fileName - имя файла
     */
    std::string m_fileName;
    /**
     * @brief m_device - устройство
     */
    int m_device;
    /**
     * @brief m_videoCapture -
     */
    cv::VideoCapture m_videoCapture;
};

#endif // FLIB_OCVFRAMEGRABBER_H
