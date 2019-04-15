#ifndef FLIB_ZMQVIDEOTRANSMITTER_H
#define FLIB_ZMQVIDEOTRANSMITTER_H

#include "abstractvideotransmitter.h"
#include "facelib_global.h"

class FACELIBSHARED_EXPORT ZMQVideoTransmitter: public AbstractVideoTransmitter
{
public:
    typedef std::shared_ptr<ZMQVideoTransmitter> Ptr;
    ZMQVideoTransmitter();

    // AbstractVideoTransmitter interface
public:
    virtual void sendImage(const cv::Mat &img);
    virtual void sendMessage(const std::string &msg);
    virtual void sendFPS(double value);
};

#endif // FLIB_ZMQVIDEOTRANSMITTER_H
