//
// Created by Xianming Liu on 06.12.23.
//

#include "MyVideoEditThread.h"
#include "MyFilter.h"

using namespace std;
using namespace cv;

static VideoCapture cap1; //一号视频源
static VideoCapture cap2; //二号视频源

//保存视频
static VideoWriter vw;
static bool isExit = false;

bool MyVideoEditThread::seek(double pos){
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    int frame = pos*count;
    return seek(frame);
}

MyVideoEditThread::MyVideoEditThread(){
    std::cout<<"thread start"<<std::endl;
    start();
}
MyVideoEditThread::~MyVideoEditThread(){
    mutex.lock();
    isExit = true;
    mutex.unlock();
    wait();

}
void MyVideoEditThread::run() {
    Mat mat1;
    for(;;){
        mutex.lock();
        if(isExit){
            mutex.unlock();
            break;
        }
        //判断视频是否打开
        if(!cap1.isOpened()){
            mutex.unlock();
            msleep(5);
            continue;
        }
        if(!isPlay){
            mutex.unlock();
            msleep(5);
            continue;
        }

        int cur = cap1.get(CAP_PROP_POS_FRAMES);
        //读取一帧视频， 解码并颜色转换
        if((end>0&&cur>=end) || !cap1.read(mat1) || mat1.empty()){
            mutex.unlock();
            //导出到结尾位置, 停止导出
            if(isWrite) {
                StopSave();
                SaveEnd();
            }
            msleep(5);
            continue;
        }

        //水印
        Mat mat2 = mark;

        if(cap2.isOpened()){
            cap2.read(mat2);
        }

        //显示图像1
        if(!isWrite){
            ImageDisplay1(mat1);
            if(!mat2.empty()) ImageDisplay3(mat2);
        }

        //通过过滤器处理视频


        Mat dst = MyFilter::Get()->Filter(mat1, mat2);
        //显示图像2
        if(!isWrite) ImageDisplay2(dst);

        int s = 0;
        s = 900/fps;

        if(isWrite){
            s = 1;
            vw.write(dst);
        }
        //sleep(40);

        msleep(s);
        mutex.unlock();
    }
}

bool MyVideoEditThread::Open(const std::string &file) {
    cout<<"open:"<<file<<endl;
    seek(0);
    mutex.lock();
    bool re = cap1.open(file);
    mutex.unlock();

    cout<<re<<endl;
    if(!re){
        return re;
    }
    fps = cap1.get(CAP_PROP_FPS);
    width = cap1.get(CAP_PROP_FRAME_WIDTH);
    height = cap1.get(CAP_PROP_FRAME_HEIGHT);

    if(fps <= 0){
        fps = 25;
    }
    src1File = file;
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    totalMS = (count / (double)fps)*1000;
    return true;
}

MyVideoEditThread * MyVideoEditThread::Get() {
    static MyVideoEditThread mvt;
    return &mvt;
}

double MyVideoEditThread::getPos() {
    double pos = 0;
    mutex.lock();
    if(!cap1.isOpened()){
        mutex.unlock();
        return pos;
    }
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    double cur = cap1.get(CAP_PROP_POS_FRAMES);
    if(count>0.001) pos = cur / count;
    mutex.unlock();
    return pos;
}

bool MyVideoEditThread::seek(int frame) {
    mutex.lock();
    if(!cap1.isOpened()){
        mutex.unlock();
        return false;
    }
    bool re = cap1.set(CAP_PROP_POS_FRAMES, frame);
    if(cap2.isOpened()) cap2.set(CAP_PROP_POS_FRAMES, frame);
    mutex.unlock();
    return re;
}

void MyVideoEditThread::SetBegin(double p){
    mutex.lock();
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    int frame = p * count;
    begin = frame;
    mutex.unlock();
}
void MyVideoEditThread::SetEnd(double p){
    mutex.lock();
    double count = cap1.get(CAP_PROP_FRAME_COUNT);
    int frame = p * count;
    end = frame;
    mutex.unlock();
}

bool MyVideoEditThread::StartSave(const string &filename, int width, int height, bool isColor) {
    cout<<"start export"<<endl;
    seek(begin);
    mutex.lock();
    if(!cap1.isOpened()) {
        mutex.unlock();
        return false;
    }
    if(width<=0){
        width = cap1.get(CAP_PROP_FRAME_WIDTH);
    }
    if(height<=0){
        height = cap1.get(CAP_PROP_FRAME_HEIGHT);
    }
    vw.open(filename,
            VideoWriter::fourcc('X','2','6','4'),
            this->fps,
            Size(width,height), isColor
            );
    if(!vw.isOpened()){
        mutex.unlock();
        cout<<"start save failed!"<<endl;
        return false;
    }
    this->isWrite = true;

    dstFile = filename;
    mutex.unlock();
    return true;
}

void MyVideoEditThread::StopSave() {
    cout<<"stop export"<<endl;
    mutex.lock();
    vw.release();
    isWrite = false;
    mutex.unlock();
}

bool MyVideoEditThread::Open2(const string &file) {
    cout<<"open2:"<<file<<endl;
    seek(0);
    mutex.lock();
    bool re = cap2.open(file);
    mutex.unlock();
    cout<<re<<endl;
    if(!re){
        return re;
    }
//    fps = cap1.get(CAP_PROP_FPS);
    width2 = cap2.get(CAP_PROP_FRAME_WIDTH);
    height2 = cap2.get(CAP_PROP_FRAME_HEIGHT);
//
//    if(fps <= 0){
//        fps = 25;
//    }
    return true;
}


//void MyVideoEditThread::ImageDisplay1(Mat mat){
//
//}
