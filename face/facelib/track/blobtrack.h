#ifndef FLIB_BLOBTRACK_H
#define FLIB_BLOBTRACK_H

#include "blob.h"
#include "facelib_global.h"

// opencv
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/gpu/gpu.hpp>

// std
#include <vector>
#include <array>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

class FACELIBSHARED_EXPORT BlobTracker;
class FACELIBSHARED_EXPORT BlobTrack;


/**
 * @brief The BlobTrack class
 * Трек для одного блоба
 */
class FACELIBSHARED_EXPORT BlobTrack
{
public:
    friend class BlobTracker;
    typedef unsigned int ID;
    typedef unsigned int LifeTime;
    BlobTrack(ID id = 0, LifeTime lifetime = 1);
    /**
     * @brief tryGetBlob
     * @param outBlob
     * @param lavel 0 - значит последний добавленный
     * @return
     * получить блоб из кольцевого буфера
     */
    bool tryGetBlob(SPtrBlob& outBlob,int lavel = 0);
    //идентификатор трека
    BlobTrack::ID getId() const;
    //получить время жизни
    BlobTrack::LifeTime getLifeTime() const;
    //получить емкость истории блобов
    int getCapacity() const;
private:
    // идентификатор трека
    BlobTrack::ID _id;
    // время сущестования, кадров
    BlobTrack::LifeTime _lifeTime;
    // емкость истории блобов
    int _capacity;
    // кольцевой буфер блобов (история)
    RingSPtrBlobs _blobHistory;
};
typedef shared_ptr<BlobTrack> SPtrBlobTrack;


/**
 * @brief The BlobTracker class
 * BlobTracker Треки блобов, доступны по id
 */
class FACELIBSHARED_EXPORT  BlobTracker
{
public:
    //тип ключа для мапы
    typedef BlobTrack::ID Key;
    typedef std::pair<Key,SPtrBlobTrack> Entry;
    BlobTracker();
    // обновить
    void update(const SPtrBlobs& blobs);
    // верент последние блобы и ключи им соответствующие
    void getLastBlobs(SPtrBlobs& blobs,vector<Key>& keys,BlobTrack::LifeTime lefeTimeThreshold = 0);
    // получить умный указатель на блоб по ключу
    bool tryGetLastBlob(Key key,SPtrBlob& outBlob);
    // получить треки и ключи им соответствующие
    void getTracks(vector<SPtrBlobTrack> tracks,vector<Key>& keys,BlobTrack::LifeTime lefeTimeThreshold = 0);
    // получить трек по ключу
    bool tryGetTrack(Key key,SPtrBlobTrack& outTrack);
private:
    // последний идентификатор, который был присвоен
    // всегда увеличивается на 1, как только нужно создать новый объект
    BlobTrack::ID _last_id;
    // треки это карта тректов, доступных по ключу id
    std::map<Key,SPtrBlobTrack> _tracks;
};

#endif // FLIB_BLOBTRACK_H
