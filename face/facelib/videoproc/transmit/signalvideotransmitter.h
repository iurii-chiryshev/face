#ifndef FLIB_SIGNALVIDEOTRANSMITTER_H
#define FLIB_SIGNALVIDEOTRANSMITTER_H

#include <QObject>

#include "abstractvideotransmitter.h"
#include "util/mat2qimage.h"
#include "facelib_global.h"

class FACELIBSHARED_EXPORT SignalVideoTransmitter : public QObject, public AbstractVideoTransmitter
{
    Q_OBJECT
public:
    typedef std::shared_ptr<SignalVideoTransmitter> Ptr;
    explicit SignalVideoTransmitter(QObject *parent = 0);

signals:
    void image(QImage img);
    void message(QString msg);
    void fps(double value);

public slots:

    // AbstractVideoTransmitter interface
public:
    virtual void sendImage(const cv::Mat &img);
    virtual void sendMessage(const std::string &msg);
    virtual void sendFPS(double value);

private:
    /**
     * @brief rgb_img
     * opencv по-умолчанию работает с bgr - форматом изображениий,
     * а вывод должен быть в qimage в rgb,
     * поэтому здесь нужно ковертировать поряодк каналов.
     */
    cv::Mat rgb_img;
};

#endif // FLIB_SIGNALVIDEOTRANSMITTER_H
