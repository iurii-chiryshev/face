#define DLIB_USE_CUDA

#include <iostream>
#include <numeric>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <dlib/dnn.h>
#include <dlib/string.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace dlib;
using namespace std;
using namespace cv;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;
template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;
template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;
template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;
template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;
using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
                            alevel0<
                            alevel1<
                            alevel2<
                            alevel3<
                            alevel4<
                            max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                            input_rgb_image_sized<150>
                            >>>>>>>>>>>>;







/**
 * @brief findDirectores
 * @param path
 * @param dirs
 * Найди все папки
 */
void findDirectores(const string &path, std::vector<string> &dirs)
{
    dirs.clear();
    boost::filesystem::directory_iterator end_itr;
    for( boost::filesystem::directory_iterator i( path ); i != end_itr; ++i )
    {
        // папка
        if(boost::filesystem::is_directory( i->status() ) ){
            string filename = i->path().filename().string();
            dirs.push_back( filename );
        }
    }
}

/**
 * @brief findFilesByRegex
 * @param path
 * @param regexp
 * @param files
 * Найди все файлы по регулярному выражению
 */
void findFilesByRegex(const string &path, const string &regexp, std::vector<string> &files)
{
    files.clear();
    const boost::regex filter(regexp);
    boost::filesystem::directory_iterator end_itr;
    for( boost::filesystem::directory_iterator i( path ); i != end_itr; ++i )
    {
        // не файл
        if( !boost::filesystem::is_regular_file( i->status() ) ) continue;

        boost::smatch what;

        // не удовлетворяет регулярному выраженнию
        string filename = i->path().filename().string();
        if( !boost::regex_match(filename, what, filter ) ) continue;

        files.push_back( filename );
    }
}


/**
 * @brief getFaceDescriptor
 * @param fileNmae
 * @param detector
 * @param sp
 * @param net
 * @param[out] desc - вектор признаков для лица на картинке
 * @return true - если нашел лицо и вытащил признаки
 */
bool getFaceDescriptor(const string &fileNmae,
                       frontal_face_detector &detector,
                       shape_predictor &sp,
                       anet_type &net,
                       matrix<float,0,1> &desc){
    //открыть изображение
    cv::Mat cv_img,cv_bgr;
    cv_img = cv::imread(fileNmae);
    if(cv_img.empty() == true) return false;

    if(cv_img.type() == CV_8UC1){
        // серое
        cv::cvtColor(cv_img,cv_bgr,CV_GRAY2BGR);
    }else if(cv_img.type() == CV_8UC3){
        // цветное
        cv_bgr = cv_img;
    }else{
        return false;
    }
    // в dlib формат
    cv_image<bgr_pixel> img(cv_bgr);

    cout << "\t" << cv::Size(cv_bgr.cols,cv_bgr.rows) << endl;

    long long t3,t2,t1,t0 = cv::getTickCount(); // замеры frame rate

    std::vector<dlib::rectangle> face_rects = detector(img);

    t1 = cv::getTickCount();
    cout << "\tdetect \t" << std::to_string((t1 - t0) / cv::getTickFrequency()) << " sec" << endl;

    if (face_rects.size() == 0) return false;

    std::vector<matrix<rgb_pixel>> faces;
    for(int i = 0; i < face_rects.size(); i++)
    {
        full_object_detection shape = sp(img, face_rects[i]);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
        faces.push_back(move(face_chip));
    }

    t2 = cv::getTickCount();
    cout << "\tshape + chip \t" << std::to_string((t2 - t1) / cv::getTickFrequency()) << " sec" << endl;

    if (faces.size() == 0) return false;



    try
      {
        // нужно одно лицо
        desc = net(faces[0]);
      }
      catch (std::exception& e)
      {
        std::cout << "exception caught: " << e.what() << endl;
        return false;
      }

    t3 = cv::getTickCount();
    cout << "\tdnn descriptor \t" << std::to_string( (t3 - t2) / cv::getTickFrequency()) << " sec" << endl;

    return true;
}


/**
 * @brief calcStatistic
 * @param mass
 * @param mean
 * @param stdev
 * расчет среднего и матожидания вектора
 */
void calcStatistic(std::vector<double> &v, double &mean, double &stdev){
    if(v.size() == 0){
        mean = 0;
        stdev = 0;
        return;
    }

    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    mean = sum / v.size();

    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    stdev = std::sqrt(sq_sum / v.size() - mean * mean);
}

void printStatistic(std::vector<double> &v,string label){
    double m,s;
    calcStatistic(v,m,s);
    cout << label << ": " << std::to_string(m) << " +- " << std::to_string(s) << endl;
}


typedef std::map<string,std::vector<matrix<float,0,1>>> TFaceDescriptors;

int main(int argc, char *argv[])
{

    // инициализируем, все что надо для детекции
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp;
    deserialize("C:/Program Files (x86)/dlib-19.4/files/shape_predictor_68_face_landmarks.dat") >> sp;
    anet_type net;
    deserialize("C:/Program Files (x86)/dlib-19.4/files/dlib_face_recognition_resnet_model_v1.dat") >> net;
    // где лежит датасет
    const string targetDir = "D:/dataset/lfw";

    //признаки для распознавания

    TFaceDescriptors faceDescriptors;
    // заполняем карту векторами признаков
    std::vector<string> dirNames;
    findDirectores(targetDir,dirNames);

    const int minFacesForOnePersone = 9;
    const int maxFaces = 600;
    int faceCount = 0;
    // первый проход - собираем вектора признаков
    for(int i = 0; i < dirNames.size(); i++){ //по всем людям (папкам)
        const string faceDir = targetDir + "/" + dirNames[i];
        const string regexp = ".*\.jpg";
        std::vector<string> fileNames;
        findFilesByRegex(faceDir,regexp,fileNames);

        if(fileNames.size() <= minFacesForOnePersone) continue; // не интересно малое число лиц
        cout << std::to_string(i+1) << ") " << faceDir << " (" << std::to_string(fileNames.size()) << ")" << endl;



        std::vector<matrix<float,0,1>> oneFaceDtors;
        for(int j = 0 ; j < fileNames.size(); j++){ // по всем лицам одного человека
            const string fullFileName = faceDir + "/" + fileNames[j];
            cout << "\t" << fileNames[j] << endl;
            matrix<float,0,1> descriptor;
            if(getFaceDescriptor(fullFileName,detector,sp,net,descriptor) == true){
                oneFaceDtors.push_back(descriptor);
                cout << "\ttrue" << endl;
            }else{
                cout << "\tfalse" << endl;
            }
        }
        if(oneFaceDtors.size() > 0){
            faceDescriptors.insert(std::make_pair(dirNames[i], oneFaceDtors));
        }

        faceCount += fileNames.size();
        if(faceCount > maxFaces) break;

    }

    std::vector<double> precisions, recalls, specificites, accuraces, fs;

    TFaceDescriptors::iterator it;
    // второй проход - считаем расстояния и метрики
    for(it = faceDescriptors.begin(); it != faceDescriptors.end(); it++){
        const string &faceName = it->first;
        const std::vector<matrix<float,0,1>> &oneFaceDtors = it->second; // дескрипторы одного лица
        cout << "metrics for " << faceName << " (" << std::to_string(oneFaceDtors.size()) << "):" << endl;

        int tp = 0,fn = 0,fp = 0,tn = 0;


        for(int i = 0; i < oneFaceDtors.size(); i++){
            //сравниваем дескрипторы одного лица м\у собой
            for(int j = i + 1; j < oneFaceDtors.size(); j++){
                if (length(oneFaceDtors[j]-oneFaceDtors[i]) < 0.6){
                   // лица похожи, и классификатор сказал правильно
                    tp++;
                }else{
                    //лица похожи, но классификатор ошибся
                    fn++;
                }
            }

            // сравниваем это лицо с лицами других людей
            TFaceDescriptors::iterator any_it;
            for(any_it = faceDescriptors.begin(); any_it != faceDescriptors.end(); any_it++){
                const string &anyFaceName = any_it->first;
                const std::vector<matrix<float,0,1>> &anyFaceDtors = any_it->second; // дескрипторы одного лица
                if(anyFaceName == faceName) continue;
                for(int j = 0; j < anyFaceDtors.size(); j++){
                    if (length(anyFaceDtors[j]-oneFaceDtors[i]) < 0.6){
                       // лица непохожи, но классификатор ошибся
                        fp++;
                    }else{
                        //лица непохожи, и классификатор  сказал правильно
                        tn++;
                    }
                }
            }
        }

        // пересчитываем метрики
        double precision = tp == 0 ? 0 : (double) (tp) / (tp + fp);  precisions.push_back(precision);
        double recall = tp == 0 ? 0 : (double)(tp) / (tp + fn); recalls.push_back(recall);
        double specificity = tn == 0 ? 0 : (double) (tn) / (tn + fp); specificites.push_back(specificity);
        double accuracy = (tp == 0 && tn == 0) ? 0 : (double) (tp + tn) / (tp + tn + fp + fn); accuraces.push_back(accuracy);
        double f1 = (precision == 0 && recall == 0) ? 0 : 2 * precision * recall / (precision + recall); fs.push_back(f1);

        cout << "\tprecision \t" <<  precision << endl;
        cout << "\trecall \t" <<  recall << endl;
        cout << "\tspecificity \t" <<  specificity << endl;
        cout << "\accuracy \t" <<  accuracy << endl;
        cout << "\f-score \t" <<  f1 << endl;

    }

    cout << "summary:" << endl;
    printStatistic(precisions,"precision");
    printStatistic(recalls,"recall");
    printStatistic(specificites,"specificity");
    printStatistic(accuraces,"accuracy");
    printStatistic(fs,"f-score");

    //cout << "Hello World!" << endl;
    return 0;
}
