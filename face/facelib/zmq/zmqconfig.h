#ifndef FLIB_ZMQCONFIG_H
#define FLIB_ZMQCONFIG_H

// Пока сделал так, что б собрал без ругани на ws2def.h and winsock2.h
// http://www.zachburlingame.com/2011/05/resolving-redefinition-errors-betwen-ws2def-h-and-winsock-h/
#define WIN32_LEAN_AND_MEAN
#include <zmq.hpp>
#include "util/singleton.h"
#include "facelib_global.h"

namespace flib {
/**
 * @brief The ZmqConfig class
 */
class FACELIBSHARED_EXPORT ZMQConfig
{
public:
    /**
    * @brief inproc_ctx
    * @return один контекст на всех для передачи сообщений
    * внутри процсса
    */
   zmq::context_t &inproc_ctx();

private:
    ZMQConfig();
    ~ZMQConfig();

    /**
     * @brief m_inproc_ctx
     */
    zmq::context_t m_inproc_ctx;

    friend class Singleton<ZMQConfig>;
};

#define ZMQ_CONFIG Singleton<ZMQConfig>::instance()

}//flib



#endif // FLIB_ZMQCONFIG_H
