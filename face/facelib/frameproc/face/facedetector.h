#ifndef FLIB_FACEDETECTOR_H
#define FLIB_FACEDETECTOR_H

#include "abstractfacedetector.h"

using namespace cv;
using namespace dlib;

/**
 * @brief The FaceDetector class
 * Класс детектора лиц.
 */
class FACELIBSHARED_EXPORT FaceDetector: public AbstractFaceDetector
{
public:
    typedef std::shared_ptr<FaceDetector> Ptr;
    FaceDetector();



    // AbstractFrameDetector interface
public:
    /**
     * @brief apply
     * @param[in] src - входное изображение CV_8UC1 (gray) или CV_8UC3 (BGR)
     * @param[out] dst - выходное изображение CV_8UC3 (BGR)
     */
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    /**
     * @brief init
     * @return true, если ОК
     * Инициализация детектора, загрузка файлов, еще что-нибудь
     */
    virtual bool init();

    /**
     * @brief uninit
     * денинициализация
     */
    virtual void uninit();

private:
    void drawPolyline(cv::Mat &img, const full_object_detection& d, const int start, const int end, bool isClosed = false);

    void renderFace(cv::Mat &img, const full_object_detection& d);

    int m_count;

    frontal_face_detector m_detector;

    shape_predictor m_poseModel;

    /**
     * @brief m_garay
     * серое, рабочее изображение для детекции
     */
    cv::Mat m_gray;

    std::string m_str_data;
    cv::Mat m_cv_data;

public:

};

#endif // FLIB_FACEDETECTOR_H
