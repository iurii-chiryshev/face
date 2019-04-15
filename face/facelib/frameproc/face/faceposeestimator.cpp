#include "faceposeestimator.h"

static cv::Point _toCvPoint(const dlib::point& p)
{
    return cv::Point(p.x(), p.y());
}

/**
 * @brief P3D_SELLION, P3D_RIGHT_EYE, P3D_LEFT_EYE
 * антропометрические 3d точки лица
 * todo построить модель по 58 точкам из opengl
 * http://aifi.isr.uc.pt/Downloads/OpenGL/glAnthropometric3DModel.cpp.
 */
static const cv::Point3f P3D_SELLION(0., 0.,0.);
static const cv::Point3f P3D_RIGHT_EYE(-20., -65.5,-5.);
static const cv::Point3f P3D_LEFT_EYE(-20., 65.5,-5.);
static const cv::Point3f P3D_RIGHT_EAR(-100., -77.5,-6.);
static const cv::Point3f P3D_LEFT_EAR(-100., 77.5,-6.);
static const cv::Point3f P3D_NOSE(21.0, 0., -48.0);
static const cv::Point3f P3D_STOMMION(10.0, 0., -75.0);
static const cv::Point3f P3D_MENTON(0., 0.,-133.0);

/**
 * @brief The FACE_FEATURE enum
 */
enum FACE_FEATURE {
    NOSE=30,
    RIGHT_EYE=36,
    LEFT_EYE=45,
    RIGHT_SIDE=0,
    LEFT_SIDE=16,
    EYEBROW_RIGHT=21,
    EYEBROW_LEFT=22,
    MOUTH_UP=51,
    MOUTH_DOWN=57,
    MOUTH_RIGHT=48,
    MOUTH_LEFT=54,
    SELLION=27,
    MOUTH_CENTER_TOP=62,
    MOUTH_CENTER_BOTTOM=66,
    MENTON=8
};


FacePoseEstimator::FacePoseEstimator()
{

}

Matx44d FacePoseEstimator::pose(const full_object_detection &detection)
{
    cv::Matx44d ret_pose;
    cv::Matx31d tvec;
    cv::Matx31d rvec;
    cv::Matx33f projection;
    FacePoseEstimator::estimate(detection,m_opticalCenter,m_focalLength,ret_pose,tvec,rvec,projection);
    return ret_pose;
}

void FacePoseEstimator::renderAxes(Mat &img, const cv::Matx31d &tvec, const cv::Matx31d &rvec, const cv::Matx33f &projection, const int size/* = 100*/)
{
    std::vector<Point3f> axes;
    axes.push_back(Point3f(0,0,0));
    axes.push_back(Point3f(size,0,0));
    axes.push_back(Point3f(0,size,0));
    axes.push_back(Point3f(0,0,size));
    std::vector<Point2f> projected_axes;
    projectPoints(axes, rvec, tvec, projection, cv::noArray(), projected_axes);
    cv::line(img, projected_axes[0], projected_axes[3], Scalar(255,0,0),2,CV_AA);
    cv::line(img, projected_axes[0], projected_axes[2], Scalar(0,255,0),2,CV_AA);
    cv::line(img, projected_axes[0], projected_axes[1], Scalar(0,0,255),2,CV_AA);
}

void FacePoseEstimator::estimate(const full_object_detection &detection,
                                const Point2f &center,
                                const Point2f &focalLength,
                                cv::Matx44d &pose,
                                cv::Matx31d &tvec,
                                cv::Matx31d &rvec,
                                cv::Matx33f &projection)
{
    projection(0,0) = focalLength.x;
    projection(1,1) = focalLength.y;
    projection(0,2) = center.x;
    projection(1,2) = center.y;
    projection(2,2) = 1;

    std::vector<cv::Point3f> headPoints;
    headPoints.push_back(P3D_SELLION);
    headPoints.push_back(P3D_RIGHT_EYE);
    headPoints.push_back(P3D_LEFT_EYE);
    headPoints.push_back(P3D_RIGHT_EAR);
    headPoints.push_back(P3D_LEFT_EAR);
    headPoints.push_back(P3D_MENTON);
    headPoints.push_back(P3D_NOSE);
    headPoints.push_back(P3D_STOMMION);



    std::vector<Point2f> detectedPoints;
    detectedPoints.push_back(_toCvPoint(detection.part(SELLION)));
    detectedPoints.push_back(_toCvPoint(detection.part(RIGHT_EYE)));
    detectedPoints.push_back(_toCvPoint(detection.part(LEFT_EYE)));
    detectedPoints.push_back(_toCvPoint(detection.part(RIGHT_SIDE)));
    detectedPoints.push_back(_toCvPoint(detection.part(LEFT_SIDE)));
    detectedPoints.push_back(_toCvPoint(detection.part(MENTON)));
    detectedPoints.push_back(_toCvPoint(detection.part(NOSE)));



    cv::Point2f stomion = (_toCvPoint(detection.part(MOUTH_CENTER_TOP)) + _toCvPoint(detection.part(MOUTH_CENTER_BOTTOM))) * 0.5;
    detectedPoints.push_back(stomion);

    //инициализация матриц поворота и вращения
    tvec = {0., 0., 1000.};
    rvec = {1.2, 1.2, -1.2};



    // Поиск 3d позы головы
    cv::solvePnP(headPoints, detectedPoints,
            projection, cv::noArray(),
            rvec, tvec, true,cv::ITERATIVE);
    cv::Matx33d rotation;
    cv::Rodrigues(rvec, rotation);


    for(int i = 0; i < 3; i++){
        pose(i,0) = rotation(i,0);
        pose(i,1) = rotation(i,1);
        pose(i,2) = rotation(i,2);
        pose(i,3) = tvec(i,0)/1000;
    }
    pose(3,0) = 0;
    pose(3,1) = 0;
    pose(3,2) = 0;
    pose(3,3) = 1;
}

void FacePoseEstimator::poseToRPY(const cv::Matx44d &pose, double &roll, double &pitch, double &yaw)
{
    // todo
    roll = 0;
    yaw = 0;
    pitch = 0;
}

cv::Point2f FacePoseEstimator::opticalCenter() const
{
    return m_opticalCenter;
}

void FacePoseEstimator::setOpticalCenter(const cv::Point2f &opticalCenter)
{
    m_opticalCenter = opticalCenter;
}

cv::Point2f FacePoseEstimator::focalLength() const
{
    return m_focalLength;
}

void FacePoseEstimator::setFocalLength(const cv::Point2f &focalLength)
{
    m_focalLength = focalLength;
}
