#ifndef FLIB_VIDEOPROCESSOR_H
#define FLIB_VIDEOPROCESSOR_H

#include "frameproc/abstractframedetector.h"
#include "framegrab/abstractframegrabber.h"
#include "transmit/abstractvideotransmitter.h"
#include "facelib_global.h"

/**
 * @brief The VideoProcessor class
 * Обработчик видео. Организует цикл чтения и обработки кадров.
 */
class FACELIBSHARED_EXPORT VideoProcessor
{

public:
    /**
     * @brief Ptr
     * smart pointer type
     */
    typedef std::shared_ptr<VideoProcessor> Ptr;
    VideoProcessor();

public:
    void loop();

    AbstractFrameGrabber::Ptr frameGrabber() const;
    /**
     * @brief setFrameGrabber
     * @param frameGrabber
     * установить источник медиа данных
     */
    void setFrameGrabber(const AbstractFrameGrabber::Ptr &frameGrabber);


    AbstractVideoTransmitter::Ptr transmitter() const;
    /**
     * @brief setTransmitter
     * @param videoTransmitter
     * установить оповещатель своего состояния
     */
    void setTransmitter(const AbstractVideoTransmitter::Ptr &videoTransmitter);

    AbstractFrameDetector::Ptr frameDetector() const;
    /**
     * @brief setFrameDetector
     * @param frameDetector
     * Установить обработчик кадров
     */
    void setFrameDetector(const AbstractFrameDetector::Ptr &frameDetector);


    bool isRun() const;
    /**
     * @brief setIsRun
     * @param isRun
     * Установить флаг проигрывания, false - прекращаем
     */
    void setIsRun(bool isRun);

    bool isRepeat() const;
    /**
     * @brief setIsRepeat
     * @param isRepeat
     * установить флаг повторения проигрывания
     */
    void setIsRepeat(bool isRepeat);

private:
    /**
     * @brief m_frameGrabber
     * Источник видеоданных: видео файлы, картинки, камеры
     */
    AbstractFrameGrabber::Ptr m_frameGrabber;
    /**
     * @brief m_frameProcessor
     * Вычислитель: детектор, трекер лиц, людей, голов
     */
    AbstractFrameDetector::Ptr m_frameDetector;
    /**
     * @brief m_transmitter
     * Оповещатель, транслятор своего состояния.
     */
    AbstractVideoTransmitter::Ptr m_transmitter;

    /**
     * @brief m_run
     * флаг выполнения
     */
    bool m_isRun;

    /**
     * @brief m_repeat
     * флаг выполнения повторения, актуально только для видеофайла
     */
    bool m_isRepeat;

};

#endif // VIDEOPROCESSOR_H
