/**
 * @file
 * @brief Маркировка связных компонент
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * */
#ifndef FLIB_CONNECTEDCOMPONENTS_H
#define FLIB_CONNECTEDCOMPONENTS_H

#include "track/blob.h"
#include "facelib_global.h"



/**
 * каким алгоритмом желаем получить связные компоненты
 */
enum
{
    CCL_SAUF, // Two Strategies to Speed up Connected Component Labeling Algorithms
    CCL_FLOODFILL, // cv::floodFill
};

/**
 * @brief connectedComponents
 * @param src_img - входной массив, будет испорчен если установлены minarea, maxarea [uchar]
 * @param dst_labels dst_labels - выходной массив меток [int]
 * @param blobs блобы, структуры данных с информацией об областях
 * @param connectivity связность 4 или 8
 * @param algorithm какой алгоритм
 * @param minarea минимально допустимая площадь региона
 * @param maxarea максимально допустимая площадь региона
 */
void FACELIBSHARED_EXPORT connectedComponents(InputOutputArray src_img,OutputArray dst_labels, Blobs& blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);

/**
 * @brief connectedComponents
 * @param src_img - входной массив, будет испорчен если установлены minarea, maxarea [uchar]
 * @param dst_labels - выходной массив меток [int]
 * @param sp_blobs - блобы, структуры данных с информацией об областях
 * @param connectivity - связность 4 или 8
 * @param algorithm - какой алгоритм
 * @param minarea - минимально допустимая площадь региона
 * @param maxarea - максимально допустимая площадь региона
 */
void FACELIBSHARED_EXPORT connectedComponents(InputOutputArray src_img,OutputArray dst_labels, SPtrBlobs& sp_blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);

/**
 * @brief connectedComponents
 * @param src_img - входной массив, будет испорчен если установлены minarea, maxarea [uchar]
 * @param dst_labels - выходной массив меток [int]
 * @param p_blobs - блобы, структуры данных с информацией об областях
 * @param connectivity - связность 4 или 8
 * @param algorithm - какой алгоритм
 * @param minarea - минимально допустимая площадь региона
 * @param maxarea - максимально допустимая площадь региона
 */
void FACELIBSHARED_EXPORT connectedComponents(InputOutputArray src_img,OutputArray dst_labels, PtrBlobs& p_blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);

/************************************************************************/
/*                     параллельный вариант                             */
/************************************************************************/
/**
 * @brief parallelConnectedComponents
 * @param src_img - входной массив, будет испорчен если установлены minarea, maxarea [uchar]
 * @param dst_labels - выходной массив меток [int]
 * @param blobs - блобы, структуры данных с информацией об областях
 * @param connectivity - связность 4 или 8
 * @param algorithm - какой алгоритм
 * @param minarea - минимально допустимая площадь региона
 * @param maxarea - максимально допустимая площадь региона
 */
void FACELIBSHARED_EXPORT  parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<Blobs>& blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);
void FACELIBSHARED_EXPORT  parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<SPtrBlobs>& blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);
void FACELIBSHARED_EXPORT  parallelConnectedComponents(vector<Mat>& src_imgs,vector<Mat>& dst_labels, vector<PtrBlobs>& blobs,int connectivity = 4,int algorithm = CCL_SAUF, int minarea = 0, int maxarea = INT_MAX);



#endif // FLIB_CONNECTEDCOMPONENTS_H
