#ifndef FLIB_ZMQFACETRANSMITTER_H
#define FLIB_ZMQFACETRANSMITTER_H

#include "abstractfacetransmitter.h"
#include "facelib_global.h"
#include "zmq.hpp"

/**
 * @brief The SignalFaceTransmitter class
 * Класс реализует отправку событий о детектировании и трекингу лиц
 * через zeromq.
 */
using namespace zmq;



class FACELIBSHARED_EXPORT ZMQFaceTransmitter: public AbstractFaceTransmitter
{
public:
    typedef std::shared_ptr<ZMQFaceTransmitter> Ptr;
    ZMQFaceTransmitter(context_t &ctx);

    // AbstractFaceTransmitter interface
public:
    virtual void send(DetectionResult &detectionResult);

private:
    /**
     * @brief m_ctx - контекст
     * Идет ссылок т.к. передается извне
     */
    context_t &m_ctx;
    /**
     * @brief m_socket
     */
    socket_t  m_socket;
    std::string m_addr;

    // AbstractFaceTransmitter interface
public:
    virtual bool init();
    virtual void uninit();
};

#endif // FLIB_ZMQFACETRANSMITTER_H
