#include "signalvideotransmitter.h"

SignalVideoTransmitter::SignalVideoTransmitter(QObject *parent) : QObject(parent)
{

}

void SignalVideoTransmitter::sendImage(const cv::Mat &img)
{
    if(!img.empty() && img.type() == CV_8UC3){
        //цветное
        cv::cvtColor(img,rgb_img,CV_BGR2RGB);
        emit image(flib::mat2QImage(rgb_img));
    }
    else if(!img.empty() && img.type() == CV_8UC1){
        //серое
        cv::cvtColor(img,rgb_img,CV_GRAY2RGB);
        emit image(flib::mat2QImage(rgb_img));
    }
}


void SignalVideoTransmitter::sendMessage(const std::string &msg)
{
    emit message(QString::fromStdString(msg));
}

void SignalVideoTransmitter::sendFPS(double value)
{
    emit fps(value);
}
