#ifndef FLIB_MAT2QIMAGE_H
#define FLIB_MAT2QIMAGE_H


#include <QImage>
#include <opencv/cv.hpp>

#include "facelib_global.h"

namespace flib {

/**
 * @brief mat2QImage - cv::Mat to QImage
 * @param[in] mat - opencv изображение
 * @return QImage изображение
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * @note память не копируется т.е. cv::Mat и QImage разделяют общий кусок в памяти
 * @note по умолчанию opencv использует BGR формат для цветных картинок
         для отображения QImage используется RGB т.е. нужно конвертнуть.
 *
 * Мы работаем с cv::mat а выводить на экран нужно QImage,
 * поэтому нужна функция-конвертер м/у представлением изображений в qt и opencv
 */
QImage FACELIBSHARED_EXPORT mat2QImage(const cv::Mat& mat);

}



#endif // MAT2QIMAGE_H
