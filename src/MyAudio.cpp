//
// Created by Xianming Liu on 09.12.23.
//

#include "MyAudio.h"
#include <iostream>
#include <QMutex>
using namespace std;

struct ATime{
    ATime(int tms){

        h = (tms/1000)/3600;
        m = ((tms/1000)%3600)/60;
        s = (tms/1000)%(3600*60);
        ms = tms % 1000;
    }
    std::string ToString(){
        char buf[16] = {0};
        sprintf(buf, "%d:%d:%d.%d", h, m, s, ms);
        return buf;
    }
    int h = 0;
    int m = 0;
    int s = 0;
    int ms = 0;
};

class CMyAudio :public MyAudio{
public:

    QMutex mutex;
    bool ExportAudio(string src, string out, int beginMS, int outMS){
        //ffmpeg -i test.mp4 -ss 0:1:10.111 -t 0:1:10.100 -vn -y test.mp3 | iconv -f gbk -t utf-8
        string cmd = "ffmpeg";
        cmd += " -i ";
        cmd += src;
        cmd += " ";
        if(beginMS > 0){
            cmd += " -ss ";
            ATime at(beginMS);
            cmd += at.ToString();
        }
        if(outMS > 0){
            cmd += " -t ";
            ATime at(outMS);
            cmd += at.ToString();
        }
        cmd += " -vn -y ";
        cmd += out + " | iconv -f gbk -t utf-8";
        cout<<cmd<<endl;
        mutex.lock();
        system(cmd.c_str());
        mutex.unlock();
        return true;
    }

    bool Merge(string v, string a, string out){
        //ffmpeg -i test.avi -i test.mp3 -c copy out.avi
        string cmd = "ffmpeg -i ";
        cmd += v;
        cmd += " -i ";
        cmd += a;
        cmd += " -c copy ";
        cmd += out + " | iconv -f gbk -t utf-8";
        cout<<cmd<<endl;
        mutex.lock();
        system(cmd.c_str());
        mutex.unlock();
        return true;

    }

};

MyAudio * MyAudio::Get() {
    static CMyAudio cma;
    return &cma;
}

MyAudio::MyAudio() {

}
MyAudio::~MyAudio(){

}