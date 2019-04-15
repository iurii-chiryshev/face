#include "abstractfacetransmitter.h"

AbstractFaceTransmitter::AbstractFaceTransmitter()
{

}

AbstractFaceTransmitter::~AbstractFaceTransmitter()
{

}

AbstractFaceTransmitter::Ptr AbstractFaceTransmitter::Null()
{
    return AbstractFaceTransmitter::Ptr(new NullFaceTransmitter());
}

NullFaceTransmitter::NullFaceTransmitter()
{

}


void NullFaceTransmitter::send(DetectionResult &detectionResult)
{
}

bool NullFaceTransmitter::init()
{
    return true;
}


void NullFaceTransmitter::uninit()
{
}
