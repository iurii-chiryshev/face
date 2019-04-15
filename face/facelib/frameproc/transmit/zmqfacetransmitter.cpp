#include "zmqfacetransmitter.h"
#include <iostream>

ZMQFaceTransmitter::ZMQFaceTransmitter(context_t &ctx): m_ctx(ctx),
    m_socket(m_ctx, ZMQ_PUB), m_addr("tcp://*:5570")
{


}


void ZMQFaceTransmitter::send(DetectionResult &detectionResult)
{
    int size = detectionResult.ByteSize();
    zmq::message_t message(size);
    detectionResult.SerializeToArray(message.data(),size);
    std::cout << "send status = " << m_socket.send(message) << " byte size= " << size << std::endl;
}

bool ZMQFaceTransmitter::init()
{
    try{
        m_socket.setsockopt<int>(ZMQ_LINGER, 0);
        m_socket.setsockopt<int>(ZMQ_IMMEDIATE, 1);
        m_socket.setsockopt<int>(ZMQ_SNDHWM, 10); // ограничение выходной очереди в 10 сообщений

        // следующий код я закометтил т.к. он плохо работает когда один клиент быстрый, а другой медленный
        // ХЗ как, но они отваливаются. А вот на клиетах CONFLATE установлен.
        //m_socket.setsockopt<int>(ZMQ_CONFLATE, 1); // подписчики будут получать только последнее сообщение, если не успевают
        m_socket.bind(m_addr);
    }catch(error_t &/*e*/){
        return false;
    }
    return true;
}


void ZMQFaceTransmitter::uninit()
{
    try{
        m_socket.unbind(m_addr);
    }catch(error_t &/*e*/){
        // todo
    }


}
