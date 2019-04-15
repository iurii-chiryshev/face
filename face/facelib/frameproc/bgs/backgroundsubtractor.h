#ifndef FLIB_BACKGROUNDSUBTRACTOR_H
#define FLIB_BACKGROUNDSUBTRACTOR_H
#include "frameproc/abstractframedetector.h"
#include "subtract/subtractor.h"
#include "facelib_global.h"


/**
 * @brief The BackgroundSubtractor class
 * Вычитатель фона. Детектор объектов по цветовому признаку
 */
class FACELIBSHARED_EXPORT BackgroundSubtractor: public AbstractFrameDetector
{
public:
    typedef std::shared_ptr<BackgroundSubtractor> Ptr;
    BackgroundSubtractor();

    // AbstractFrameDetector interface
public:
    virtual void apply(const cv::Mat &src, cv::Mat &dst);
    virtual bool init();
private:
    /**
     * @brief m_count - счетчик кадров
     */
    int m_count;

    /**
     * @brief m_subtractor
     * вычитатель
     */
    SubtractorMOG2P m_subtractor;

    /**
     * @brief m_grayFG
     * выходная картинка с вычитателя серая
     */
    cv::Mat m_grayFG;


};

#endif // FLIB_BACKGROUNDSUBTRACTOR_H
