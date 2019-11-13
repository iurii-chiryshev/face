#include "facedetector.h"
#include "util/timestamp.h"

#include <QUuid>


using namespace dlib;

FaceDetector::FaceDetector(): m_count(0)
{

}

void FaceDetector::apply(const cv::Mat &src, cv::Mat &dst)
{
    CV_Assert(!src.empty() && "src is empty");
    CV_Assert( (src.type() == CV_8UC1 || src.type() == CV_8UC3) && "src type not CV_8UC1 or CV_8UC3");

    if(src.type() == CV_8UC3){
        cv::cvtColor(src,m_gray,CV_BGR2GRAY);
        src.copyTo(dst);
    }else{
        src.copyTo(m_gray);
        cv::cvtColor(src,dst,CV_GRAY2BGR);
    }

    // Change to dlib's image format. No memory is copied.
    cv_image<uchar> d_src(m_gray);


    std::vector<dlib::rectangle> faces = m_detector(d_src);

    // Find the pose of each face.
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < faces.size(); ++i)
    {
        // Landmark detection on full sized image
        full_object_detection shape = m_poseModel(d_src, faces[i]);
        shapes.push_back(shape);

        // Custom Face Render
        renderFace(dst, shape);


#ifndef _DEBUG
        //расчитываем положение и рисуем оси
        cv::Matx44d pose;
        cv::Matx31d tvec;
        cv::Matx31d rvec;
        cv::Matx33f projection;
        const Point2f center(src.rows / 2.f, src.cols / 2.f);
        const Point2f focalLength(src.rows * 2,src.rows * 2);
        FacePoseEstimator::estimate(shape,center,focalLength,pose,tvec,rvec,projection);
        // todo где-то здесь ошибка именно на дебаге
        FacePoseEstimator::renderAxes(dst,tvec,rvec,projection);
#endif


    }
}

bool FaceDetector::init()
{
    m_count = 0;
    std::cout << "m_detector = get_frontal_face_detector();" << std::endl;
    m_detector = get_frontal_face_detector();
    std::cout << "m_gray.release();" << std::endl;
    m_gray.release();
    std::cout << "deserialize" << std::endl;
    deserialize("shape_predictor_68_face_landmarks.dat") >> m_poseModel;
    std::cout << "return true;" << std::endl;
    return true;
}

void FaceDetector::drawPolyline(cv::Mat &img, const full_object_detection &d, const int start, const int end, bool isClosed)
{
    std::vector <cv::Point> points;
    for (int i = start; i <= end; ++i)
    {
        points.push_back(cv::Point(d.part(i).x(), d.part(i).y()));
    }
    cv::polylines(img, points, isClosed, cv::Scalar(0,255,255), 1, 16);

}

void FaceDetector::renderFace(cv::Mat &img, const full_object_detection &d)
{
    drawPolyline(img, d, 0, 16);           // Jaw line
    drawPolyline(img, d, 17, 21);          // Left eyebrow
    drawPolyline(img, d, 22, 26);          // Right eyebrow
    drawPolyline(img, d, 27, 30);          // Nose bridge
    drawPolyline(img, d, 30, 35, true);    // Lower nose
    drawPolyline(img, d, 36, 41, true);    // Left eye
    drawPolyline(img, d, 42, 47, true);    // Right Eye
    drawPolyline(img, d, 48, 59, true);    // Outer lip
    drawPolyline(img, d, 60, 67, true);    // Inner lip

    dlib::rectangle rect = d.get_rect();
    cv::rectangle(img,cv::Rect(rect.tl_corner().x(),rect.tl_corner().y(),rect.width(),rect.height()),cv::Scalar(0,255,255),2);
//    for(int i = 0; i < d.num_parts(); i++){
//        const dlib::point &p = d.part(i);
//        cv::circle(img,cv::Point(p.x(),p.y()),1,cv::Scalar(255,0,0), 1);
//    }

}

void FaceDetector::uninit()
{

}
