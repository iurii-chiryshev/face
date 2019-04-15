#include "backgroundsubtractor.h"

BackgroundSubtractor::BackgroundSubtractor()
{
    m_count = 0;
}

void BackgroundSubtractor::apply(const cv::Mat &src, cv::Mat &dst)
{
    CV_Assert(!src.empty() && "src is empty");
    CV_Assert( src.type() == CV_8UC3 && "src type not CV_8UC3");
    m_subtractor(src,m_grayFG);

    // преобразуем в цветное изображение
    cvtColor(m_grayFG,dst,CV_GRAY2BGR);

}

bool BackgroundSubtractor::init()
{
    m_count = 0;
    m_subtractor = SubtractorMOG2P();
    m_grayFG.release();
    return true;
}
