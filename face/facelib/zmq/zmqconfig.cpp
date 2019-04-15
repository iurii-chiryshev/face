#include "zmqconfig.h"

namespace flib {

zmq::context_t &ZMQConfig::inproc_ctx()
{
    return m_inproc_ctx;
}

ZMQConfig::ZMQConfig()
{

}

ZMQConfig::~ZMQConfig()
{

}

}

