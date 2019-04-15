#ifndef FLIB_SIGNALFACETRANSMITTER_H
#define FLIB_SIGNALFACETRANSMITTER_H

#include "abstractfacetransmitter.h"
#include "facelib_global.h"

/**
 * @brief The SignalFaceTransmitter class
 * Класс реализует отправку событий об детектировании и трекингу лиц
 * через qt сигналы.
 */
class FACELIBSHARED_EXPORT SignalFaceTransmitter: public AbstractFaceTransmitter
{
public:
    typedef std::shared_ptr<SignalFaceTransmitter> Ptr;
    SignalFaceTransmitter();

    // AbstractFaceTransmitter interface
public:
    virtual void send(DetectionResult &detectionResult);
    virtual bool init();
    virtual void uninit();
};

#endif // FLIB_SIGNALFACETRANSMITTER_H
