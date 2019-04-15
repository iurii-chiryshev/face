#include <zmq/zmqconfig.h>

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>

#include <framegrab/ocvframegrabber.h>
#include <frameproc/face/facedetector.h>
#include <frameproc/transmit/zmqfacetransmitter.h>
#include <videoproc/videoprocessor.h>
#include <videoproc/videoprocessortask.h>
#include <async/backgroundworker.h>

using namespace std;
using namespace zmq;
using namespace flib::proto;
using namespace cv;

class ZmqFaceReceiver{
public:
   typedef std::shared_ptr<ZmqFaceReceiver> Ptr;

   ZmqFaceReceiver(context_t &ctx): m_ctx(ctx),
       m_socket(m_ctx, ZMQ_SUB),
       m_addr("tcp://localhost:5570")
   {
       m_winname = "result";
       m_isRun = true;

   }

   ~ZmqFaceReceiver(){

   }

   void run(){

       m_socket.setsockopt<int>(ZMQ_CONFLATE, 1);
       m_socket.setsockopt<int>(ZMQ_RCVTIMEO, 3000);
       m_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
       m_socket.connect(m_addr);
       cv::namedWindow(m_winname);

       while(isRun()){
           cv::waitKey(1);
           zmq::message_t message;
           bool isRead = m_socket.recv(&message);
           cout << "is read= " << isRead << endl;
           if(isRead == false) continue;
           bool isParse = m_result.ParseFromArray(message.data(),message.size());
           cout << " is parse= " << isRead << endl;
           if (isParse == false) continue;
           if (m_result.has_mat() == false) continue;
           const flib::proto::Mat &mat = m_result.mat();
           int rows = mat.rows(),
                   cols = mat.cols(),
                   channels = mat.channels(),
                   depth = mat.depth(),
                   step = mat.step();
           void *data = (void*)mat.data().data();

           cv::Mat img(rows,cols,CV_MAKETYPE(depth,channels),data,step);

           cv::imshow(m_winname,img);
       }

       cv::destroyWindow(m_winname);
       m_socket.disconnect(m_addr);
   }

   bool isRun() const
   {
        return m_isRun;
   }

   void setIsRun(bool isRun)
   {
        m_isRun = isRun;
   }


private:
   context_t &m_ctx;
   socket_t  m_socket;
   std::string m_addr;
   DetectionResult m_result;
   std::string m_winname;
   bool m_isRun;
};


class ZmqFaceReceiverTask: public AbstractTask{
    //Q_OBJECT
public:
    typedef QSharedPointer<ZmqFaceReceiverTask> Ptr;
    explicit ZmqFaceReceiverTask(const ZmqFaceReceiver::Ptr &receiver, QObject *parent = 0): AbstractTask(parent),  m_receiver(receiver){}
// AbstractTask interface
    virtual void run(){
        m_receiver->run();
    }

    virtual QString getName(){
        return "ZmqFaceReceiverTask";
    }

private:
   ZmqFaceReceiver::Ptr m_receiver;
};


int main(int argc, char *argv[])
{
    //создаем zmq контекст
    context_t ctx1(1);

    // передатчик детектора, т.е. тот кто вещает о найденых лицах
    ZMQFaceTransmitter *ft = new ZMQFaceTransmitter(ctx1);
    AbstractFaceTransmitter::Ptr ft_ptr(ft);
    // детектор лиц
    FaceDetector *fd = new FaceDetector();
    fd->setTransmitter(ft_ptr); // установить ему передатчик
    AbstractFrameDetector::Ptr fd_ptr(fd);


    // создаем framegraber c фейковым источником -> видео  файл
    std::string filename = "C:/Users/iurii.chiryshev/Videos/0188_03_021_al_pacino.avi";
    OCVFrameGrabber *fg = new OCVFrameGrabber(filename);
    AbstractFrameGrabber::Ptr fg_ptr(fg);

    // видео процессор
    VideoProcessor *vp = new VideoProcessor();
    VideoProcessor::Ptr vp_ptr(vp);
    vp_ptr->setFrameGrabber(fg_ptr); // установить источник видео
    vp_ptr->setFrameDetector(fd_ptr); // установить детектор
    vp_ptr->setIsRun(true); // состояние -> запущен
    vp_ptr->setIsRepeat(true); // без повторения для видео

    // фоновая задача для видео процессора
    AbstractTask::Ptr vp_task_ptr(new VideoProcessorTask(vp_ptr));




    // получатель
    ZmqFaceReceiver *fr = new ZmqFaceReceiver(ctx1);
    ZmqFaceReceiver::Ptr fr_ptr(fr);
    // фоновая задача для получатля
    AbstractTask::Ptr fr_task_ptr(new ZmqFaceReceiverTask(fr_ptr));

    // исполнитель фоновых задач на один поток
    BackgroundWorker bw(2);

    // запускаем


    bw.start(vp_task_ptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    bw.start(fr_task_ptr);


    while (!_kbhit())
    {

    }

    // останов всего
    vp_ptr->setIsRun(false);
    fr_ptr->setIsRun(false);

    return 0;
}

