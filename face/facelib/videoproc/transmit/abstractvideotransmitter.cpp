#include "abstractvideotransmitter.h"

AbstractVideoTransmitter::AbstractVideoTransmitter()
{

}

AbstractVideoTransmitter::~AbstractVideoTransmitter()
{

}

AbstractVideoTransmitter::Ptr AbstractVideoTransmitter::Null()
{
    return AbstractVideoTransmitter::Ptr(new NullVideoTransmitter());
}

NullVideoTransmitter::NullVideoTransmitter()
{

}

void NullVideoTransmitter::sendImage(const cv::Mat &img)
{

}

void NullVideoTransmitter::sendMessage(const std::string &msg)
{

}

void NullVideoTransmitter::sendFPS(double value)
{

}
