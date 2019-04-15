#include "simplepeopledetector.h"

SimplePeopleDetector::SimplePeopleDetector()
{
    m_defaultDescriptor = HOGDescriptor::getDefaultPeopleDetector();
    m_daimlerDescriptor = HOGDescriptor::getDaimlerPeopleDetector();
}


void SimplePeopleDetector::apply(const cv::Mat &src, cv::Mat &dst)
{
    //todo где-то в методе валится на дебаге

    // проверка на наличие входного изображения + формат картинки (серое или цветное)
    CV_Assert(!src.empty() && "src is empty");
    CV_Assert( (src.type() == CV_8UC1 || src.type() == CV_8UC3) && "src type not CV_8UC1 or CV_8UC3");

    if(src.type() == CV_8UC3){
        //цветное
        cv::cvtColor(src,m_gray,CV_BGR2GRAY);
        src.copyTo(dst);
    }else{
        //серое
        src.copyTo(m_gray);
        cv::cvtColor(src,dst,CV_GRAY2BGR);
    }

    vector<cv::Rect> found, found_filtered; // результат деткции (прямоугольники)
    // ищем людей
    m_hog.detectMultiScale(m_gray, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    size_t i, j;
    // фильтруем
    for( i = 0; i < found.size(); i++ )
    {
        cv::Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }
    // рисуем
    for( i = 0; i < found_filtered.size(); i++ )
    {
        cv::Rect r = found_filtered[i];
        // уменьшить прямоугольники
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        cv::rectangle(dst, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}

bool SimplePeopleDetector::init()
{
    m_hog.setSVMDetector(m_defaultDescriptor);
    m_gray.release();
    return true;
}


void SimplePeopleDetector::uninit()
{
}
