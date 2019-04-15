/**
 * @file
 * @brief Маркировка связных компонент sauf
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * @note это инклюдник для внутреннего пользования
 * */
#ifndef FLIB__CCL_SAUF_H
#define FLIB__CCL_SAUF_H

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


using namespace cv;
using namespace std;

// enum-ы для статистики
enum { CC_32_STAT_LEFT   = 0,
    CC_32_STAT_TOP    = 1,
    CC_32_STAT_WIDTH  = 2,
    CC_32_STAT_HEIGHT = 3,
    CC_32_STAT_AREA   = 4,
    CC_32_STAT_PIX = 5,
    CC_32_STAT_MAX    = 6
};

enum { CC_64_STAT_M10   = 0,
    CC_64_STAT_M01    = 1,
    CC_64_STAT_MAX    = 2
};

/**
 * @brief connectedComponents_sauf
 * @param image
 * @param labels
 * @param connectivity
 * @param ltype
 * @return
 */
int connectedComponents_sauf(InputArray image, OutputArray labels,
                                     int connectivity = 8, int ltype = CV_32S);
/**
 * @brief connectedComponentsWithStats_sauf
 * @param image
 * @param labels
 * @param stats_32s
 * @param stat_64s
 * @param connectivity
 * @param ltype
 * @return
 */
int connectedComponentsWithStats_sauf(InputArray image, OutputArray labels,
                                              OutputArray stats_32s, OutputArray stat_64s,
                                              int connectivity = 8, int ltype = CV_32S);

#endif // FLIB__CCL_SAUF_H
