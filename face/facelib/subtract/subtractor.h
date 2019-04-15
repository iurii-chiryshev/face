/**
 * @file
 * @brief Заголовочный файл нескольких вычитателей фона.
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * @note Для детекции людей 100% понадобится детектор движения в кадре.
 * В данном файле реализованы и оптимизированы три различных детектора
 * */

#ifndef FLIB_SUBTRACTOR_H
#define FLIB_SUBTRACTOR_H

// opencv
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/gpu/gpu.hpp>

// std
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "facelib_global.h"

using namespace cv;
using namespace std;


/**
 * @brief The SubtractorOG1 class
 */
class FACELIBSHARED_EXPORT SubtractorOG1
{
public:
    /**
     * @brief SubtractorOG1
     * конструктор
     */
    SubtractorOG1();
    /**
     * @brief SubtractorOG1
     * @param threshFactor множитель для порога threshFactor*Deviation (по умолчанию правило трех сигм threshFactor = 3)
     * @param alpha скорость обновления фона
     * ctor
     */
    SubtractorOG1(float threshFactor, float alpha);

    ~SubtractorOG1();
    /**
     * @brief createBackgroundModel
     * создание фоновой модели по накопленной статистике
     */
    void createBackgroundModel();

    /**
     * @brief accumulateBackground
     * @param src
     * добавить фрейм в статистику для построения модели
     */
    void accumulateBackground(cv::InputArray src);

    /**
     * @brief updateBackgroundModel
     * @param src
     * @param src_mask
     * обновление фона по маске (обновляются пикселы = 0)
     * если src_mask не указана обновляется все изображение
     */
    void updateBackgroundModel(cv::InputArray src,cv::InputArray src_mask = cv::noArray());

    /**
     * @brief getBackgroundImage
     * @param dst
     * получить текущую модель фона
     */
    void getBackgroundImage(cv::OutputArray dst) const;
    /**
     * @brief operator ()
     * @param src
     * @param dst
     * получить маску переднего плана
     */
    void operator()(InputArray src, OutputArray dst);

private:
    // размер фрейма
    cv::Size frameSize_;
    // нулевая маска переднего плана
    // восьмибитная одноканальная
    cv::Mat emptyFgmask_;
    // модель фона
    cv::Mat bgmodel_;
    // сумма изображений
    cv::Mat accum_;
    // сумма квадратов изображений
    cv::Mat accumSq_;
    // число накопленных кадров
    int nAccum_;
    // флаг инициализации модели фона
    int fInitBgModel_;
    // порог по дисперсии для всех каналов
    float varThreshFactor_;
    // скорость обновления модели
    float alpha_;

private:

    /**
     * @brief initializeAccum
     * @param size
     * @param type
     * инициализация аккумуляторов
     */
    void initializeAccum(const cv::Size& size, int type);

};

/**
 * @brief The SubtractorMG1 class
 */
class FACELIBSHARED_EXPORT SubtractorMG1
{
public:
    /**
     * @brief SubtractorMG1
     */
    SubtractorMG1();
    /**
     * @brief SubtractorMG1
     * @param threshFactor
     * @param alpha
     */
    SubtractorMG1(float threshFactor, float alpha);

    ~SubtractorMG1();

    /**
     * @brief updateBackgroundModel
     * @param src
     * @param src_mask
     * обновление фона по маске (обновляются пикселы = 0)
     * если src_mask не указана обновляется все изображение
     */
    void updateBackgroundModel(cv::InputArray src,cv::InputArray src_mask = cv::noArray());

    /**
     * @brief getBackgroundImage
     * @param dst
     * получить текущую модель фона
     */
    void getBackgroundImage(cv::OutputArray dst) const;

    /**
     * @brief operator ()
     * @param src
     * @param dst_fg
     * @param dst_shadow
     * получить маску переднего плана
     * опционально можно получить тень
     */
    void operator()(InputArray src, OutputArray dst_fg, OutputArray dst_shadow);

    /**
     * @brief initialize
     * @param size
     * инициализация сабтрактора размером изображения
     */
    void initialize(const cv::Size& size);
private:
    // размер фрейма
    cv::Size frameSize_;
    // нулевая маска переднего плана
    // восьмибитная одноканальная
    cv::Mat emptyFgmask_;
    // модель фона
    cv::Mat bgmodel_;
    // количество используемых смесей
    cv::Mat useBgmixture_;
    // флаг инициализации модели фона
    int fInitBgModel_;
    // порог по дисперсии для всех каналов
    float varThreshFactor_;
    // скорость обновления модели
    float alpha_;

};

/**
 * @brief The SubtractorMOG2P class
 */
class FACELIBSHARED_EXPORT SubtractorMOG2P
{
public:
    /**
     * @brief SubtractorMOG2P
     * ctor
     */
    SubtractorMOG2P();

    /**
     * @brief SubtractorMOG2P
     * @param history
     * @param varThreshold
     * @param bShadowDetection
     * ctor
     */
    SubtractorMOG2P(int history,  float varThreshold, bool bShadowDetection=true);

    ~SubtractorMOG2P();
    /**
     * @brief operator ()
     * @param src
     * @param dst_fgmask
     * @param src_predict
     * @param learningRate
     * основная функция
     */
    void operator()(InputArray src, OutputArray dst_fgmask, InputArray src_predict = noArray(), float learningRate = -1);

    /**
     * @brief getBackgroundImage
     * @param backgroundImage
     * получить изображение фона
     */
    void getBackgroundImage(OutputArray backgroundImage) const;

    /**
     * @brief initialize
     * @param frameSize
     * инициализация
     */
    void initialize(Size frameSize);

    /**
     * @brief getUseMixturesImage
     * @param dst
     * получить изображение используемых компонент смеси
     * восьмибитное одноканальное
     */
    void getUseMixturesImage(OutputArray dst);

    /**
     * @brief getMeanVarImage
     * @param dst
     * получить изображение средней дисперсии
     * восьмибитное одноканальное
     */
    void getMeanVarImage(OutputArray dst);

private:
    Size frameSize_;
    cv::Mat bgmodel_;
    cv::Mat bgmodelUsedModes_;
    cv::Mat emptyPredict_;
    int nframes_;
    int history_;
    int nmixtures_;
    double varThreshold_;
    float backgroundRatio_;
    float varThresholdGen_;
    float fVarInit_;
    float fVarMin_;
    float fVarMax_;
    float CT_;
    bool bShadowDetection_;
    bool bInitBgmodel_;
    uchar nShadowDetection_;
    float Tau_;
};

#endif // FLIB_SUBTRACTOR_H
