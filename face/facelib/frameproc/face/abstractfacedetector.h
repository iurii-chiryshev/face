#ifndef ABSTRACTFACEDETECTOR_H
#define ABSTRACTFACEDETECTOR_H

#include "frameproc/abstractframedetector.h"
#include "frameproc/transmit/abstractfacetransmitter.h"
#include "faceposeestimator.h"
#include "facelib_global.h"

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

/**
 * @brief The AbstractFaceDetector class
 * Абстрактный класс для детекции лиц
 */
class FACELIBSHARED_EXPORT AbstractFaceDetector: public AbstractFrameDetector
{
public:
    typedef std::shared_ptr<AbstractFaceDetector> Ptr;
    AbstractFaceDetector();
    virtual ~AbstractFaceDetector();

    /**
     * @brief setTransmitter
     * @param transmitter
     * Установить трансмиттер
     */
    void setTransmitter(const AbstractFaceTransmitter::Ptr &transmitter);

protected:
    /**
     * @brief m_transmitter
     * Передатчик или вещатель о найденых лицах
     */
    AbstractFaceTransmitter::Ptr m_transmitter;
};

#endif // ABSTRACTFACEDETECTOR_H
