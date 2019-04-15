#include "abstractfacedetector.h"

AbstractFaceDetector::AbstractFaceDetector(): m_transmitter(AbstractFaceTransmitter::Null())
{

}

AbstractFaceDetector::~AbstractFaceDetector()
{

}

void AbstractFaceDetector::setTransmitter(const AbstractFaceTransmitter::Ptr &transmitter)
{
    m_transmitter = transmitter;
}
