#include "_blobmap.h"


using namespace cv;
using namespace std;

namespace internal_blobmap{
    /*перечисление индексов в векторе признаков*/
    enum
    {
        FV_AREA = 0,
        FV_WIDTH = 1,
        FV_HEIGHT = 2,
        FV_MINX = 3,
        FV_MINY = 4,
        FV_MAXX = 5,
        FV_MAXY = 6,
        FV_CX = 7,
        FV_CY = 8,
        FV_ALL = 9
    };

    /*признаки блобов по которым сравниваем*/
    typedef struct FV
    {
        int feature[FV_ALL];
    } FV;
    /*объединить два fv в один */
    inline static void fvJoin(const int *src, int *dst)
    {
        dst[FV_MINX] = MIN(dst[FV_MINX],src[FV_MINX]);
        dst[FV_MINY] = MIN(dst[FV_MINY],src[FV_MINY]);
        dst[FV_MAXX] = MAX(dst[FV_MAXX],src[FV_MAXX]);
        dst[FV_MAXY] = MAX(dst[FV_MAXY],src[FV_MAXY]);
        int sum_area = dst[FV_AREA] + src[FV_AREA];
        dst[FV_CX] = (dst[FV_CX]*dst[FV_AREA] + src[FV_CX]*src[FV_AREA])/sum_area;
        dst[FV_CY] = (dst[FV_CY]*dst[FV_AREA] + src[FV_CY]*src[FV_AREA])/sum_area;
        dst[FV_AREA] = sum_area;
        dst[FV_WIDTH] = dst[FV_MAXX] - dst[FV_MINX] + 1;
        dst[FV_HEIGHT] = dst[FV_MAXY] - dst[FV_MINY] + 1;
    }

    /**
     * @brief fvAbsDiff
     * @param src1
     * @param src2
     * @return
     *   растояние межу векторами призкаков
     *   порядок параметров имеет значение
     *   //////////////////////////////////
     *   sum( abs(src1[i] - src2[i])/src2[i]	) i = [0..n]
     */
    inline static double fvAbsDiff(const int *src1,const int *src2)
    {
        double diff = (double)abs(src1[FV_AREA] - src2[FV_AREA])/src2[FV_AREA];
        diff += (double)abs(src1[FV_WIDTH] - src2[FV_WIDTH])/src2[FV_WIDTH];
        diff += (double)abs(src1[FV_HEIGHT] - src2[FV_HEIGHT])/src2[FV_HEIGHT];
        return diff;
    }
    /*заполнить вектор первичных признаков*/
    inline static void fvFillFeatures(int *dst, const Blob& blob)
    {
        CV_Assert(!blob.isEmpty() && "blob can`t be empty");
        dst[FV_AREA] = blob.area;
        dst[FV_WIDTH] = blob.rect.width;
        dst[FV_HEIGHT] = blob.rect.height;
        dst[FV_MINX] = blob.rect.x;
        dst[FV_MINY] = blob.rect.y;
        dst[FV_MAXX] = dst[FV_MINX] + dst[FV_WIDTH] - 1;
        dst[FV_MAXY] = dst[FV_MINY] + dst[FV_HEIGHT] - 1;
        dst[FV_CX] = blob.centroid.x;
        dst[FV_CY] = blob.centroid.y;
    }
    inline static void fvFillFeatures(int *dst, const SPtrBlob& blob)
    {
        fvFillFeatures(dst,*blob);
    }
    /*расстояние между двумя векторами признаков. Определяет примерное соответствие объектов и блобов*/
    inline static int fvBoundingBoxDistant(const int *src1,const int *src2)
    {
        int d1,d2;
        if (src1[FV_CX] < src2[FV_MINX])
        {
            if (src1[FV_CY] < src2[FV_MINY])
                d1 = MAX(src2[FV_MINX] - src1[FV_CX], src2[FV_MINY] - src1[FV_CY]);
            else if (src1[FV_CY]>src2[FV_MAXY])
                d1 = MAX(src2[FV_MINX] - src1[FV_CX], src1[FV_CY] - src2[FV_MAXY]);
            else // (pf2[PF_MINY] < pf1[PF_CY])&&(pf1[PF_CY] < pf2[PF_MAXY])
                d1 = src2[FV_MINX] - src1[FV_CX];
        }
        else if (src1[FV_CX]>src2[FV_MAXX])
        {
            if (src1[FV_CY]<src2[FV_MINY])
                d1 = MAX(src1[FV_CX] - src2[FV_MAXX], src2[FV_MINY] - src1[FV_CY]);
            else if (src1[FV_CY]>src2[FV_MAXY])
                d1 = MAX(src1[FV_CX] - src2[FV_MAXX], src1[FV_CY] - src2[FV_MAXY]);
            else
                d1 = src1[FV_CX] - src2[FV_MAXX];
        }
        else // (pf2[PF_MINX] =< pf1[PF_CX]) && (pf1[PF_CX] =< pf2[PF_MAXX])
        {
            if (src1[FV_CY]<src2[FV_MINY])
                d1 = src2[FV_MINY] - src1[FV_CY];
            else if (src1[FV_CY]>src2[FV_MAXY])
                d1 = src1[FV_CY] - src2[FV_MAXY];
            else
                return 0;
        }

        if (src2[FV_CX] < src1[FV_MINX])
        {
            if (src2[FV_CY]<src1[FV_MINY])
                d2 = MAX(src1[FV_MINX] - src2[FV_CX], src1[FV_MINY] - src2[FV_CY]);
            else if (src2[FV_CY]>src1[FV_MAXY])
                d2 = MAX(src1[FV_MINX] - src2[FV_CX], src2[FV_CY] - src1[FV_MAXY]);
            else // (pf1[PF_MINY] < pf2[PF_CY])&&(pf2[PF_CY] < pf1[PF_MAXY])
                d2 = src1[FV_MINX] - src2[FV_CX];
        }
        else if (src2[FV_CX] > src1[FV_MAXX])
        {
            if (src2[FV_CY]<src1[FV_MINY])
                d2 = MAX(src2[FV_CX] - src1[FV_MAXX], src1[FV_MINY] - src2[FV_CY]);
            else if (src2[FV_CY]>src1[FV_MAXY])
                d2 = MAX(src2[FV_CX] - src1[FV_MAXX], src2[FV_CY] - src1[FV_MAXY]);
            else
                d2 = src2[FV_CX] - src1[FV_MAXX];
        }
        else // (pf1[PF_MINX] =< pf2[PF_CX]) && (pf2[PF_CX] =< pf1[PF_MAXX])
        {
            if (src2[FV_CY] < src1[FV_MINY])
                d2 = src1[FV_MINY] - src2[FV_CY];
            else if (src2[FV_CY]>src1[FV_MAXY])
                d2 = src2[FV_CY] - src1[FV_MAXY];
            else
                return 0;
        }
        return MIN(d1, d2);
    }

    template<typename _Ty>
    /**
     * @brief blobsToFeatures
     * @param blobs
     * @param features одноканальное [int]
     * блобы в вектора признаков.
     * количество столбцов соответствует blobs.size();
     */
    inline void blobsToFeatures(const vector<_Ty>& blobs,Mat& features)
    {
        const size_t nblobs = blobs.size();
        if(nblobs <= 0)
        {
            features.release();
            return;
        }
        features.create(nblobs,FV_ALL,CV_32SC1);
        for(size_t y = 0; y < nblobs; ++y)
        {
            fvFillFeatures(features.ptr<int>(y),blobs[y]);
        }
    }

    typedef void (*CompareFunc)(BlobMapsInt& blob_maps,const cv::Mat& prev_feat, const cv::Mat& curr_feat,
                                vector<int>& prev_id, vector<int>& curr_id);
    /**
     * @brief compare_2prev_2curr
     * @param blob_maps
     * @param prev_feat
     * @param curr_feat
     * @param prev_id
     * @param curr_id
     * сопоставляем блобы по умолчанию
     * Непонятный случай
     * ----------------------------------------
     * В таком случае
     * 1. ищем максимальный по площади предыдущий и текущий объекдиняем
     * 2. оставшиеся текущие помечаем как новые
     * 3. оставшиеся предыдущие, оставляем как потерянные
     */
    static void compare_2prev_2curr(BlobMapsInt& blob_maps,const cv::Mat& prev_feat, const cv::Mat& curr_feat,
        vector<int>& prev_id, vector<int>& curr_id)
    {
        const int ncurr = (int)curr_id.size(), nprev = (int)prev_id.size();
        CV_Assert( (ncurr > 1) && (nprev > 1) );
        int i,j,area = 0,pId = -1, cId = -1;
        /*найди самый большой по площади предыдущий блоб*/
        for(i = 0; i< nprev; ++i)
        {
            j = prev_id[i];
            if(prev_feat.at<int>(j,FV_AREA) > area)
            {
                pId = j;
            }
        }
        /*найди самый большой по площади текущий блоб*/
        area = 0;
        for(i = 0; i < ncurr; ++i)
        {
            j = curr_id[i];
            if(curr_feat.at<int>(j,FV_AREA) > area)
            {
                cId = j;
            }
        }
        /*пару нашли, нужно сопоставить*/
        blob_maps[pId].curr_val.push_back(cId);
        /*остальные предыдущие делаем потерянными

        for(i = 0; i< nprev; ++i)
        {

        }
         ОНИ УЖЕ ПОМЕЧЕНЫ КАК ПОТЕРЯННЫЕ
        */
        /*остальные текущие делаем новыми*/
        for(i = 0; i < ncurr; ++i)
        {
            j = curr_id[i];
            if(j != cId)
            {
                blob_maps.push_back(BlobMapInt());
                blob_maps.back().curr_val.push_back(j);
            }
        }
    }

    /**
     * @brief compare_2prev_1curr
     * @param blob_maps
     * @param prev_feat
     * @param curr_feat
     * @param prev_id
     * @param curr_id
     * объединяем 1 текущий блоб и несколько предыдущих
     * Случай соединения двух и более кусков в один
     * -----------------------------------------------
     * В таком случае создаем новый объект, и указываем на все объекты из предыдущего кадра
     * (собираемся использовать схему разрезанием, пересечение не актуально)
     */
    static void compare_2prev_1curr(BlobMapsInt& blob_maps,const cv::Mat& prev_feat, const cv::Mat& curr_feat,
        vector<int>& prev_id, vector<int>& curr_id)
    {
        const int ncurr = (int)curr_id.size(), nprev = (int)prev_id.size();
        CV_Assert( (ncurr == 1) && (nprev > 1) );
        blob_maps.push_back(BlobMapInt());
        BlobMapInt& map = blob_maps.back();
        map.curr_val.push_back(curr_id[0]);
        for(int i = 0; i<nprev; ++i)
        {
            map.prev_val.push_back(prev_id[i]);
        }
    }
    /*объединяем 1 текущий и 1 предыдущий*/
    static void compare_1prev_1curr(BlobMapsInt& blob_maps,const cv::Mat& prev_feat, const cv::Mat& curr_feat,
        vector<int>& prev_id, vector<int>& curr_id)
    {
        CV_Assert( (curr_id.size() == 1) && (prev_id.size() == 1) );
        BlobMapInt& map = blob_maps[prev_id[0]];
        map.curr_val.push_back(curr_id[0]);
    }
    /**
     * @brief compare_1prev_2curr
     * @param blob_maps
     * @param prev_feat
     * @param curr_feat
     * @param prev_id
     * @param curr_id
     * объединяем много текущих и 1 предыдущий
     * Случай разъединения одного объекта на несколько кусков
     * -----------------------------------------------------------
     * В таком случае смотрим какие блобы наилучшим образом соответствуют
     * предыдущему. Оставшиеся блобы превращаем в новые
     */
    static void compare_1prev_2curr(BlobMapsInt& blob_maps,const cv::Mat& prev_feat, const cv::Mat& curr_feat,
        vector<int>& prev_id, vector<int>& curr_id)
    {
        /*выделить память под вектора признаков блобов и инициализировать*/
        const int ncurr = (int)curr_id.size(), nprev = (int)prev_id.size();
        CV_Assert( (ncurr > 1) && (nprev == 1) );
        int i,j;
        cv::Mat dist(1,ncurr,CV_64FC1);
        double *pDist = dist.ptr<double>(0);
        const int *pPrevFV = prev_feat.ptr<int>(prev_id[0]);
        // данные на заголовок и сами вектора
        int **ppCurrFV = (int**)malloc(ncurr*(sizeof(int*) + sizeof(FV)));
        int *pCurrFVBody = (int*)(void*)(ppCurrFV + ncurr); // указатель где лежат сами данные
        for(i = 0; i < ncurr; i++,pCurrFVBody+=FV_ALL)
        {
            /*инициализация*/
            memcpy((void*)pCurrFVBody,(void*)curr_feat.ptr(curr_id[i]),sizeof(FV));
            /*растояние от предыдущего до всех текущих*/
            pDist[i] = fvAbsDiff(pCurrFVBody,pPrevFV);
            ppCurrFV[i] = pCurrFVBody;
        }
        /*отсортируем текущие по возрастанию дистанции*/
        for (i=1; i<ncurr ;i++)
        {
            for (j=0; j<ncurr-i ;j++)
            {
                if (pDist[j] > pDist[j+1])
                {
                    std::swap(pDist[j],pDist[j+1]);
                    std::swap(ppCurrFV[j],ppCurrFV[j+1]);
                    std::swap(curr_id[j],curr_id[j+1]);
                }
            }
        }
        /*объединяем признаки пока расстояние до ед. следа уменьшается*/
        for( i = 1; i < ncurr; i++)
        {
            /*объединенный вектор с предыдущим*/
            fvJoin(ppCurrFV[i-1],ppCurrFV[i]);
            /*растояние от предыдущего до объединенного вектора текущих*/
            pDist[i] = fvAbsDiff(ppCurrFV[i],pPrevFV);
            /*повторять это пока растояние уменьшается*/
            if(pDist[i] > pDist[i-1])
                break;
        }

        /*исправить существующий*/
        BlobMapInt& map = blob_maps[prev_id[0]];
        for(j = 0 ; j < i ;j++)
        {
            map.curr_val.push_back(curr_id[j]);
        }
        /*
            остальные из текущих это новые объекты
            добавить их в карту
        */
        for( ;j< ncurr;j++)
        {
            blob_maps.push_back(BlobMapInt());
            blob_maps.back().curr_val.push_back(curr_id[j]);
        }
        /*освободить память под ppCurrFV*/
        free((void*)ppCurrFV);
    }


    /**
     * @brief getCompareFunc
     * @param nprev
     * @param ncurr
     * @return
     * вернет ту, функцию, которую нужно вызвать в зависимости от количества сопоставляемых объектов
     */
    static CompareFunc getCompareFunc(unsigned int nprev,unsigned int ncurr)
    {
        // [prev][curr]
        static CompareFunc COMPARE_LUT[3][3] = {{0	,0								 ,0								  },
                                                {0	,(CompareFunc)compare_1prev_1curr,(CompareFunc)compare_1prev_2curr},
                                                {0	,(CompareFunc)compare_2prev_1curr,(CompareFunc)compare_2prev_2curr    }};

        return COMPARE_LUT[MIN(2,nprev)][MIN(2,ncurr)];

    }

    /*доступ к матрице смеждости*/
    #define ADJ(cu,pr) adj.at<int>(cu,pr)
    #define CURR_ACC(cu) ADJ(cu,nprev)
    #define PREV_ACC(pr) ADJ(ncurr,pr)

    /* функции для кластеризации
    */
    void currClastering(int x, cv::Mat& adj,vector<int>& prev_id,vector<int>& curr_id,vector<bool>& use_prev,vector<bool>& use_curr);
    /*найдет для блоба след*/
    void prevClastering(int y, cv::Mat& adj,vector<int>& prev_id,vector<int>& curr_id,vector<bool>& use_prev,vector<bool>& use_curr)
    {
        const int ncurr = adj.rows - 1, nprev = adj.cols - 1;
        /*цикл по предыдущим*/
        for (int x = 0; x < nprev; ++x)
        {
            if (ADJ(y, x) == 0)
                continue;

            int acc = PREV_ACC(x);
            /*добавить след в список использованных*/
            if(use_prev[x])
            {
                prev_id.push_back(x);
                use_prev[x] = false;
            }


            /*сделать обнуление в матрице смежности */
            ADJ(y, x) = 0;
            /*уменьшить аккумы*/
            CURR_ACC(y)--;
            PREV_ACC(x)--;

            if (acc > 1)
            {
                /*если у этого блоба есть еще связи найди их*/
                currClastering(x,adj,prev_id,curr_id,use_prev,use_curr);
            }
        }
    }
    /*найдет для следа блоб*/
    void currClastering(int x, cv::Mat& adj,vector<int>& prev_id,vector<int>& curr_id,vector<bool>& use_prev,vector<bool>& use_curr)
    {
        const int ncurr = adj.rows - 1, nprev = adj.cols - 1;
        /*цикл по текущим*/
        for (int y = 0; y < ncurr; ++y)
        {
            if (ADJ(y, x) == 0)
                continue;
            int acc = CURR_ACC(y);
            /*добавить блоб в список использованных*/
            if(use_curr[y])
            {
                curr_id.push_back(y);
                use_curr[y] = false;
            }

            /*сделать обнуление в матрице смежности */
            ADJ(y, x) = 0;
            /*уменьшить аккумы*/
            CURR_ACC(y)--;
            PREV_ACC(x)--;
            if (acc > 1)
            {
                /*если у этого блоба есть еще связи найди их*/
                prevClastering(y,adj,prev_id,curr_id,use_prev,use_curr);
            }
        }
    }
} // end internal_blobmap
using namespace internal_blobmap;


/************************************************************************/
/*
                   внешние функции
*/
/************************************************************************/
template<typename _Ty>
/**
 * @brief _blobMapping
 * @param blob_maps
 * @param prev_blobs
 * @param curr_blobs
 */
inline void _blobMapping(BlobMapsInt& blob_maps,const vector<_Ty>& prev_blobs, const vector<_Ty>& curr_blobs)
{
    //количество предыдущих блобов
    const int nprev = (int)prev_blobs.size();
    //количество текущих блобов
    const int ncurr = (int)curr_blobs.size();
    //изменить размер выходного вектора
    blob_maps.clear();
    if((nprev == 0) && (ncurr == 0)) return;
    blob_maps.resize(nprev);
    /*изначально карта потерянных блобов [1,0]*/
    for(int x = 0; x < nprev; x++)
    {
        blob_maps[x].prev_val.push_back(x);
    }
    /*
        вектор признаков предыдущих и текущих блобов,
        /////////////////////////////////////////////////////
        матрица соответствия (строки -> curr, столбцы -> prev)
        1 будет при пересечении
    */
    cv::Mat curr_feat, prev_feat, adj = cv::Mat::zeros(ncurr+1,nprev+1,CV_32SC1);
    blobsToFeatures(curr_blobs,curr_feat);
    blobsToFeatures(prev_blobs,prev_feat);
    /*инициализация матрицы соответствия*/
    for(int y = 0; y < ncurr; ++y)
    {
        for(int x = 0; x < nprev; ++x)
        {
            if( fvBoundingBoxDistant(curr_feat.ptr<int>(y),prev_feat.ptr<int>(x)) <= 0 )
            {
                /*есть соответствие, помечаем и увеличиваем аккум.*/
                ADJ(y,x) = 1;
                CURR_ACC(y)++;
                PREV_ACC(x)++;
            }
        }
    }

    /*
        детектируем потерянные,
        это те из предыдущих у которых аккум = 0, т.е. ни с кем не сопоставил
        их не трогаем потому как map пустой

    for(int x = 0; x < nprev; ++x)
    {
        if(TRAIL_ACC(x) == 0)
        {

        }
    }
    */

    /*
        детектируем новые объекты.
        это те из текущих у которых аккум = 0, т.е. ни с кем не сопоставил
    */
    for(int y = 0; y < ncurr; ++y)
    {
        if(CURR_ACC(y) == 0)
        {
            blob_maps.push_back( BlobMapInt() );
            blob_maps.back().curr_val.push_back(y);
        }
    }
    /*пытаемся сопоставить остальные объекты*/
    vector<int> prev_id; prev_id.reserve(nprev);
    vector<int> curr_id; curr_id.reserve(ncurr);
    for(int x = 0; x< nprev; ++x)
    {
        /*если аккум пустой на следующую итерацию*/
        if(PREV_ACC(x) == 0)
            continue;
        /*
            если акум не пустой поиск соответствий
        */
        vector<bool> use_prev(nprev,true);
        vector<bool> use_curr(ncurr,true);
        prev_id.resize(0);
        curr_id.resize(0);
        prev_id.push_back(x);
        use_prev[x] = false;
        currClastering(x,adj,prev_id,curr_id,use_prev,use_curr);
        /*получить функцию для вызова. зависит от количества сопоставлений*/
        CompareFunc fnc = getCompareFunc(prev_id.size(),curr_id.size());
        if(fnc)
        {
            fnc(blob_maps,prev_feat,curr_feat,prev_id,curr_id);
        }
    }
}

void blobMapping(BlobMapsInt& blob_maps,const Blobs& prev_blobs, const Blobs& curr_blobs)
{
    _blobMapping(blob_maps,prev_blobs,curr_blobs);
}

void blobMapping(BlobMapsInt& blob_maps,const SPtrBlobs& prev_blobs, const SPtrBlobs& curr_blobs)
{
    _blobMapping(blob_maps,prev_blobs,curr_blobs);
}


