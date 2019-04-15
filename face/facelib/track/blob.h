#ifndef FLIB_BLOB_H
#define FLIB_BLOB_H

// opencv
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/gpu/gpu.hpp>

//boost
#include <boost/circular_buffer.hpp>
// std
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>

#include "facelib_global.h"




using namespace std;
using namespace cv;


/**
 * @brief The Blob struct
 * Некая структура, содержащая в себе некоторые (пока примитивные)
 * геометрические атрибуты (признаки) объектов для сопоставления му собой
 * и соответственно для трекинга
 */
struct FACELIBSHARED_EXPORT Blob
{
public:
    /**
     * все атрибуты блоба как флаги
     * Пока протые геометрические признаки
     */
    enum
    {
        BIT_AREA = 1,
        BIT_CX = 2,
        BIT_CY = 4,
        BIT_MINX = 8,
        BIT_MAXX = 16,
        BIT_MINY = 32,
        BIT_MAXY = 64,
        BIT_WIDTH = 128,
        BIT_HEIGHT = 256,
        BIT_ALL = 512 - 1,
    };

    /**
     * Перечисление, передний план, задний план, тень
     */
    enum
    {
        PIX_FOREGROUND = 255, // передний план
        PIX_SHADOW = 127, // тень
        PIX_BACKGROUND = 0, // задний план
    };

    /**
     * перечисление статуса, либо yes либо no
     */
    enum
    {
        STATUS_NO = 0,
        STATUS_YES = 1,
    };
    /**
     * @brief Blob
     * @param _label
     * @param _pix
     * @param _area
     * @param _rect
     * @param _centroid
     * ctor
     */
    Blob(int _label = 0,int _pix = Blob::PIX_BACKGROUND, int _area = 0, const cv::Rect& _rect = cv::Rect(),const cv::Point& _centroid = cv::Point(0,0));
    /**
     * @brief label метка
     */
    int label;

    /**
     * @brief pix
     * цвет пикселя, чтобы отличать объект и его тень
     */
    int pix;
    /**
     * @brief area
     * площадь объекта
     */
    int area; // M00

    /**
     * @brief rect
     * ограничивающий пр¤моугольник
     */
    cv::Rect rect;

    /**
     * @brief centroid
     * центр масс блоба
     */
    cv::Point centroid; // { M10/M00 , M01/M00 }

    /**
     * @brief status
     * абстрактное поле, статуса,на сколько этот блоб интересен
     */
    int status;
    /**
     * @brief isEmpty
     * @return true если пустой
     */
    bool isEmpty() const;

    /**
     * @brief convertToVec
     * @param dst
     * @param flags
     * преобразование в вектор признако
     */
    void convertToVec(vector<int>& dst,int flags = Blob::BIT_ALL) const;

    /**
     * @brief distance
     * @param src
     * @param flags вектор признаков
     * @return
     * расстояние между двум¤ блобами,
     */
    int distance(const Blob& src, int flags = Blob::BIT_ALL) const;
    int distance(const Blob* src, int flags = Blob::BIT_ALL) const;
    int distance(const shared_ptr<Blob>& src, int flags = Blob::BIT_ALL) const;

    /**
     * @brief boundingBoxDistance
     * @param blob1
     * @param blob2
     * @return
     * вернет расстояние до ограничивающего пр¤моугольника
     */
    static int boundingBoxDistance(const Blob& blob1,const Blob& blob2);
    static int boundingBoxDistance(const Blob* blob1,const Blob* blob2);
    static int boundingBoxDistance(const shared_ptr<Blob>& blob1,const shared_ptr<Blob>& blob2);
    static int boundingBoxDistance(const Blob& blob,const cv::Rect& rect);
    static int boundingBoxDistance(const Blob* blob,const cv::Rect& rect);
    static int boundingBoxDistance(const shared_ptr<Blob>& blob,const cv::Rect& rect);

    /**
     * @brief operator ==
     * @param blob
     * @return
     * операторы
     */
    bool operator == (const Blob& blob) const;
    bool operator != (const Blob& blob) const;
};






typedef Blob Blob; // блоб
typedef Blob* PtrBlob; // указатель
typedef shared_ptr<Blob> SPtrBlob; // умный указатель
typedef vector<Blob> Blobs; //
typedef vector<PtrBlob> PtrBlobs;  //
typedef vector<SPtrBlob> SPtrBlobs; //
typedef boost::circular_buffer<SPtrBlob> RingSPtrBlobs; // кольцевой буфер


/**
 * @brief deletePtrBlobs
 * @param ptr_blobs
 * корректно удалить вектор указателей на блобы
 */
void FACELIBSHARED_EXPORT deletePtrBlobs(PtrBlobs& ptr_blobs);


/**
 * @brief splitBlobsByPix
 * @param blobs
 * @param fg_blobs
 * @param shad_blobs
 * разделит блобы на две части объекты fore, shadow
 * если используетс¤ SPtrBlobs данные не копируются
 */
void FACELIBSHARED_EXPORT splitBlobsByPix(const Blobs& blobs,Blobs& fg_blobs, Blobs& shad_blobs);
void FACELIBSHARED_EXPORT splitBlobsByPix(const SPtrBlobs& blobs,SPtrBlobs& fg_blobs, SPtrBlobs& shad_blobs);


/**
 * @brief getBlobsByPix
 * @param src
 * @param dst
 * @param pix
 * вернет вектор блобов по указанному цвету пикселя
 * если используетс¤ SPtrBlobs данные не копируются
 */
void FACELIBSHARED_EXPORT getBlobsByPix(const Blobs& src,Blobs& dst, int pix = Blob::PIX_FOREGROUND);
void FACELIBSHARED_EXPORT getBlobsByPix(const SPtrBlobs& src,SPtrBlobs& dst, int pix = Blob::PIX_FOREGROUND);

/**
 * @brief mergeBlobs
 * @param dst
 * @param src1
 * @param src2
 * cобрать блобы в один вектор
 * если используетс¤ SPtrBlobs данные не копируются
 */
void FACELIBSHARED_EXPORT mergeBlobs(Blobs& dst,const Blobs& src1,const Blobs& src2);
void FACELIBSHARED_EXPORT mergeBlobs(SPtrBlobs& dst,const SPtrBlobs& src1,const SPtrBlobs& src2);

/**
 * каким алгоритмом сопоставлять блобы му калрами
 */
enum
{
    HUNGARIAN, // венгерский метод в задаче о назначениях
    GREEDY, // жадный алгоритм
};


/**
 * @brief assignment
 * @param query
 * @param train
 * @param assign
 * @param flags
 * @param method
 * назначит блобы из query блобам из train
 * на основе method и flags
 */
void FACELIBSHARED_EXPORT assignment(const SPtrBlobs& query,const SPtrBlobs& train,vector<DMatch>& assign, int flags = Blob::BIT_ALL, int method = HUNGARIAN);


#endif // FLIB_BLOB_H
