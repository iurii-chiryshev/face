/**
 * @file
 * @brief Заголовочный файл класса, который оценивает положение
 * головы по 68 особым точкам лица
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * */
#ifndef FLIB_FACEPOSEESTIMATOR_H
#define FLIB_FACEPOSEESTIMATOR_H

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

// opencv
#include <opencv/cv.hpp>

#include "facelib_global.h"

using namespace cv;
using namespace dlib;

/**
 * @brief The FacePoseEstimator class
 * Класс оценки положения головы
 */
class FACELIBSHARED_EXPORT FacePoseEstimator
{
public:
    /**
     * @brief Pose
     * Матрица 4x4 однозначно описывает положение лица
     */
    typedef cv::Matx44d Pose;
    /**
     * @brief FacePoseEstimator
     * ctor
     */
    FacePoseEstimator();

    /**
     * @brief estimate
     * @param[in] detection
     * @return положение головы
     * Оуенить положение лица
     */
    cv::Matx44d pose(const full_object_detection& detection);

    /**
     * @brief renderAxes
     * @param img
     * @param tvec
     * @param rvec
     * @param projection
     * @param size - размер осей
     */
    static void renderAxes(cv::Mat &img,
                           const cv::Matx31d &tvec,
                           const cv::Matx31d &rvec,
                           const cv::Matx33f &projection,
                           const int size = 100);

    /**
     * @brief compute
     * @param[in] detection - лицо из dlib
     * @param[in] center - оптический центр камеры
     * @param[in] focalLenght - фокусное растояние
     * @param[out] pose - положение лица
     * статический метод дасчета положения лица
     */
    /**
     * @brief estimate
     * @param[in] detection - лицо из dlib
     * @param[in] center - оптический центр камеры
     * @param[in] focalLenght - фокусное растояние
     * @param[out] pose - положение лица
     * @param[out] tvec
     * @param[out] rvec
     * @param[out] projection
     */
    static void estimate(const full_object_detection& detection,
                        const cv::Point2f &center,
                        const cv::Point2f &focalLength,
                        cv::Matx44d &pose,
                        cv::Matx31d &tvec,
                        cv::Matx31d &rvec,
                        cv::Matx33f &projection);

    /**
     * @brief poseToRPY
     * @param pose
     * @param roll
     * @param pitch
     * @param yaw
     * матрицу преобразования в углы в радианах (крен, тангаж и рыскание)
     */
    static void poseToRPY(const cv::Matx44d &pose, double &roll, double &pitch, double &yaw);

    /**
     * @brief opticalCenter
     * @return - оптический центр камеры
     */
    cv::Point2f opticalCenter() const;
    /**
     * @brief setOpticalCenter
     * @param opticalCenter
     * Установить оптический центр камеры
     */
    void setOpticalCenter(const cv::Point2f &opticalCenter);
    /**
     * @brief focalLength
     * @return - фокусное расстояние
     */
    cv::Point2f focalLength() const;
    /**
     * @brief setFocalLength
     * @param focalLength
     * Установить фокусное расстояние
     */
    void setFocalLength(const cv::Point2f &focalLength);

private:
    /**
     * @brief m_opticalCenter
     * Оптичский центр. Параметры камеры
     */
    cv::Point2f m_opticalCenter;
    /**
     * @brief m_focalLength
     * Фокусное расстояние. Параметры камеры
     */
    cv::Point2f m_focalLength;
};

#endif // FLIB_FACEPOSEESTIMATOR_H
