#ifndef FLIB_ABSTRACTFRAMEGRABBER_H
#define FLIB_ABSTRACTFRAMEGRABBER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "facelib_global.h"

/**
 * @brief The AbstractFrameGrabber class
 * Абстрактный класс видеозахвата, предоставляет интерфейс для доступа
 * к устройству или видеофайлу. Типичная работа с классом сводится к следующей
 * послеовательности вызовов.
 * @code
 * func(){
 *      FrameGrabber fg;
 *      // открыть
 *      fg.open();
 *      // инициализировать
 *      fg.init();
 *      // запустить
 *      fg.start();
 *      //mainloop
 *      for(;;){
 *          // следующий кадр
 *          fg.read()
 *      }
 *      // отстанов
 *      fg.stop()
 *      // деинициализация
 *      fg.uninit();
 *      // закрываем
 *      fg.close();
 * }
 * @endcode
 */
class FACELIBSHARED_EXPORT AbstractFrameGrabber
{
public:
    /**
     * @brief Ptr
     * smart ptr type
     */
    typedef std::shared_ptr<AbstractFrameGrabber> Ptr;
    /**
     * @brief AbstractFrameGrabber
     * ctor
     */
    AbstractFrameGrabber();
    /**
     * @brief ~AbstractFrameGrabber
     * dtor
     */
    virtual ~AbstractFrameGrabber();
public:
    /**
     * @brief open
     * @return
     * Открыть устройство, файл, пр.
     */
    virtual bool open() = 0;
    /**
     * @brief init
     * @return
     * Инициализировать устройство
     */
    virtual bool init() = 0;
    /**
     * @brief start
     * @return
     * Запустить устройство
     */
    virtual bool start() = 0;
    /**
     * @brief read
     * @param[out] image
     * @return
     * Прочитать очередную картинку с устройства
     * Обычно вызывается в цикле
     */
    virtual bool read(cv::Mat &image) = 0;
    /**
     * @brief stop
     * остановить устройство
     */
    virtual void stop() = 0;
    /**
     * @brief uninit
     * деинициализация устройства
     */
    virtual void uninit() = 0;
    /**
     * @brief close
     * Закрываем, освобождаем устройство
     */
    virtual void close() = 0;
public:
    static AbstractFrameGrabber::Ptr Null();
};

/**
 * @brief The NullFrameGrabber class
 * NULL OBJECT на класс видеозахвата
 */
class FACELIBSHARED_EXPORT NullFrameGrabber: public AbstractFrameGrabber
{

public:
    typedef std::shared_ptr<NullFrameGrabber> Ptr;
    NullFrameGrabber();
    // AbstractFrameGrabber interface
public:
    virtual bool open();
    virtual bool init();
    virtual bool start();
    virtual bool read(cv::Mat &image);
    virtual void stop();
    virtual void uninit();
    virtual void close();
};

#endif // FLIB_ABSTRACTFRAMEGRABBER_H
