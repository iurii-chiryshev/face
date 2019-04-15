/**
 * @file
 * @brief Заголовочный файл мапы блобов
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * @note это инклюдник для внутреннего пользования
 * */
#ifndef FLIB__BLOBMAP_H
#define FLIB__BLOBMAP_H

#include "blob.h"


/**
 * Преречисление допустимых результатом мапинга блобов
 * Blob Mapping Status
 */
enum{
    /*Такого состояния не может быть*/
    BMS_FAIL,
    /*[prev,curr]*/
    /* Новый [0,1]*/
    BMS_NEW,
    /* Потерянный [1,0]*/
    BMS_LOSE,
    /* Однозначное соответствие [1,1]*/
    BMS_ONE_TO_ONE,
    /* Развалился [1,>1]*/
    BMS_SPLIT,
    /* Слился [>1,1]*/
    BMS_MERGE
};

//look up table [prev][curr]
static const int BM_LUT[3][3] = { {BMS_FAIL,	BMS_NEW,		BMS_FAIL},
                                    {BMS_LOSE,	BMS_ONE_TO_ONE,	BMS_SPLIT},
                                    {BMS_FAIL,	BMS_MERGE,		BMS_FAIL}	};

/**
 * Структура хранит соответствия блобов предыдущего и текущего кадра
 */
template<typename _ValTy>
struct BlobMap
{
public:
    // указатель на предыдущие блобы
    vector<_ValTy> prev_val;
    // указатель на текущие блобы
    vector<_ValTy> curr_val;
    // статус
    int getStatus() const
    {
        return BM_LUT[MIN(2,prev_val.size())][MIN(2,curr_val.size())];
    }
};

typedef BlobMap<int> BlobMapInt;
typedef BlobMap<SPtrBlob> BlobMapSPtr;
typedef vector<BlobMapInt> BlobMapsInt;
typedef vector<BlobMapSPtr> BlobMapsSPtr;


/**
 * @brief mappingStatus
 * @param blob_map
 * @return
 * Статус мапы
 */
template<typename _ValTy>
int mappingStatus(const BlobMap<_ValTy>& blob_map)
{
    return blob_map.getStatus();
}

//************************************
// Method:    blobMapping
// Parameter: BlobMapsi & blob_maps		-	Карта совмещения блобов prev_blobs -> curr_blobs.
//											blob_maps имеет размер, такойже как у prev_blobs
//											или больше (в случае появления новых объектов).
//											Для просмотра, что чему соответствует у blob_maps
//											есть поля curr_val и prev_val
// Parameter: const Blobs & prev_blobs	-	вектор предыдущих блобов
// Parameter: const Blobs & curr_blobs	-	вектор текущих блобов
// ПРИМЕР:
// 1.	если у blob_maps[0] в curr_val записано {1,2} это значит, что блоб prev_blobs[0]
//		из передыдущего кадра соответствует блобам curr_blobs[1] и curr_blobs[2] в текущем
//		(т.е. объект по какимто причинам распался на 2)
// 2.	если у blob_maps[1] ничего нету, значит prev_blobs[1] потерян
// 3.	если у "нового" объекта blob_maps[5] в prev_val записано {1,2} а в curr_val - {1}
//		это значит, что блобу curr_blobs[1] в текущем кадре соответствуют блобы
//		prev_blobs[1] и prev_blobs[2] предыдущего кадра (т.е. объекты слились в один)
//************************************

/**
 * @brief blobMapping
 * @param blob_maps -	Карта совмещения блобов prev_blobs -> curr_blobs.
 *						blob_maps имеет размер, такойже как у prev_blobs
 *						или больше (в случае появления новых объектов).
 *						Для просмотра, что чему соответствует у blob_maps
 *						есть поля curr_val и prev_val
 * @param prev_blobs	-	вектор предыдущих блобов
 * @param curr_blobs	-	вектор текущих блобов
 * @note
 * 1.	если у blob_maps[0] в curr_val записано {1,2} это значит, что блоб prev_blobs[0]
 *		из передыдущего кадра соответствует блобам curr_blobs[1] и curr_blobs[2] в текущем
 *		(т.е. объект по какимто причинам распался на 2)
 * 2.	если у blob_maps[1] ничего нету, значит prev_blobs[1] потерян
 * 3.	если у "нового" объекта blob_maps[5] в prev_val записано {1,2} а в curr_val - {1}
 *		это значит, что блобу curr_blobs[1] в текущем кадре соответствуют блобы
 *		prev_blobs[1] и prev_blobs[2] предыдущего кадра (т.е. объекты слились в один)
 */
void blobMapping(BlobMapsInt& blob_maps,const Blobs& prev_blobs, const Blobs& curr_blobs);
void blobMapping(BlobMapsInt& blob_maps,const SPtrBlobs& prev_blobs, const SPtrBlobs& curr_blobs);


#endif // FLIB__BLOBMAP_H
