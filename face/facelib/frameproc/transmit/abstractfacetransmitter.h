#ifndef FLIB_ABSTRACTFRAMETRANSMITTER_H
#define FLIB_ABSTRACTFRAMETRANSMITTER_H

#include <memory>
#include "proto/detectionresult.pb.h"
#include "facelib_global.h"

/**
 * @brief The AbstractFaceTransmitter class
 * Абстрактный класс вещателя (уведомителя) о происходящем в ходе обработки кадров.
 * Этот класс используют детекторы лиц. Реальный протокол вещания - реализуют наследники.
 * Пока смотрю в сторону qt signal\slot или zeroMQ.
 * @note Класс по своей задумке предназначен для отправки только событий (структурированных сообщений), связанных с детектированием
 * и трекингом объектов в кадре. Т.е. не видео поток!!!
 *
 */
using namespace flib::proto;

class FACELIBSHARED_EXPORT AbstractFaceTransmitter
{
public:
    typedef std::shared_ptr<AbstractFaceTransmitter> Ptr;
    AbstractFaceTransmitter();
    virtual ~AbstractFaceTransmitter();
    static AbstractFaceTransmitter::Ptr Null();

    virtual void send(DetectionResult &detectionResult) = 0;
    virtual bool init() = 0;
    virtual void uninit() = 0;

public:

};


/**
 * @brief The NullFaceTransmitter class
 * Заглушка. Ничего никому не отправляет
 */
class FACELIBSHARED_EXPORT NullFaceTransmitter: public AbstractFaceTransmitter
{
public:
    typedef std::shared_ptr<NullFaceTransmitter> Ptr;
    NullFaceTransmitter();

    // AbstractFaceTransmitter interface
public:
    virtual void send(DetectionResult &detectionResult);
    virtual bool init();
    virtual void uninit();
};

#endif // FLIB_ABSTRACTFRAMETRANSMITTER_H
