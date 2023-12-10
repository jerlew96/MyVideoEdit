//
// Created by Xianming Liu on 06.12.23.
//

#ifndef MYVIDEOEDIT_MYVIDEOEDITTHREAD_H
#define MYVIDEOEDIT_MYVIDEOEDITTHREAD_H
#include <QThread>
#include <QMutex>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


class MyVideoEditThread : public QThread{

    Q_OBJECT

public:

    int fps = 0;
    int width = 0;
    int height = 0;

    int width2 = 0;
    int height2 = 0;

    int totalMS;
    std::string src1File;
    std::string dstFile;

    int begin = 0;
    int end = 0;

    void SetBegin(double p);
    void SetEnd(double p);

    static MyVideoEditThread * Get();
    bool Open(const std::string &file);     //打开一号视频源文件
    bool Open2(const std::string &file);    //打开二号视频

    void Play(){mutex.lock(); isPlay = true; mutex.unlock();}
    void Pause(){mutex.lock(); isPlay = false; mutex.unlock();}
    double getPos();     //返回当前播放位置
    bool seek(int frame);      //跳转视频 @para frame int = 帧位置
    bool seek(double pos);
    //开始保存视频
    bool StartSave(const std::string &filename, int width=0, int height=0, bool isColor = true);
    //停止保存视频， 写入视频帧的索引
    void StopSave();

    void SetMark(cv::Mat m){mutex.lock(); this->mark = m; mutex.unlock();}    //添加水印 水印大小需要修改（不能超过分辨率

    void run();    //线程入口函数

    ~MyVideoEditThread();

signals:
    void ImageDisplay1(cv::Mat mat);
    void ImageDisplay3(cv::Mat mat);

    void ImageDisplay2(cv::Mat mat);
    void SaveEnd();

protected:
    QMutex mutex;
    bool isWrite = false;  //是否开始写视频
    bool isPlay = false;
    cv::Mat mark;
    MyVideoEditThread();
};


#endif //MYVIDEOEDIT_MYVIDEOEDITTHREAD_H
