#include <iostream>
#include<windows.h>
#include <conio.h>
#include <time.h>
#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include <proto/mat.pb.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace flib::proto;

int main(int argc, char *argv[])
{
    cv::Mat load_img = imread("D:/dataset/EPFL-RLC_dataset/frames/cam2/RLCAFTCONF-C2_100066.jpeg");
    //cv::Mat img = imread("D:/dataset/lfw/Aaron_Peirsol/Aaron_Peirsol_0001.jpg");
    cv::Mat img;
    double sf = 256. / load_img.cols;
    cv::resize(load_img,img,cv::Size(),sf,sf);

    //imshow("img",img);
    //cv::waitKey(1);

    size_t sizeInBytes = img.step[0] * img.rows;
    cout << "size in bytes: " << sizeInBytes << endl;

    std::string data_str;
    data_str.resize(sizeInBytes);


    std::string out_str;
    vector<uchar> encode_buf;

    for (int i = 0; i < 1000; i++){
        try
        {
            // нужно одно лицо
            cv::Mat data_img(img.rows,img.cols,img.type(),(void*)data_str.data(),img.step[0]);
            img.copyTo(data_img);

            flib::proto::Mat frame;
            frame.set_depth(img.depth());
            frame.set_channels(img.channels());
            frame.set_rows(img.rows);
            frame.set_cols(img.cols);
            frame.set_step(img.step[0]);
            frame.set_allocated_data(&data_str);

            int byteSize = frame.ByteSize();
            cout << "frame byte size: " << byteSize << endl;

            long long t2,t1,t0 = cv::getTickCount(); // замеры frame rate


            frame.SerializeToString(&out_str);

            t1 = cv::getTickCount();


            vector<int> compression_params;
            compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
            compression_params.push_back(95);
            cv::imencode(".jpg",img,encode_buf,compression_params);

            t2 = cv::getTickCount();

            cout << "out string size: " << out_str.size() << endl;
            cout << "serialize time : " << std::to_string((t1 - t0) / cv::getTickFrequency()) << " sec" << endl;
            cout << "endode size : " << encode_buf.size() << endl;
            cout << "encoded time : " << std::to_string((t2 - t1) / cv::getTickFrequency()) << " sec" << endl;

            frame.release_data();
        }
        catch (std::exception &e)
        {
            std::cout << "exception caught: " << e.what() << endl;
            break;
        }


    }

    while (!_kbhit())
    {
        cv::waitKey(10);
    }
    cv::destroyAllWindows();
    return 0;
}
