//
// Created by Xianming Liu on 09.12.23.
//

#ifndef MYVIDEOEDIT_MYAUDIO_H
#define MYVIDEOEDIT_MYAUDIO_H
#include <string>


class MyAudio {
public:
    static MyAudio * Get();
    //导出音频
    //beginMS开始毫秒， outMS输出音频时长毫秒
    virtual bool ExportAudio(std::string src, std::string out, int beginMS = 0, int outMS = 0) = 0;
    //合并音视频
    //beginMS开始毫秒， outMS输出音频时长毫秒
    virtual bool Merge(std::string v, std::string a, std::string out) = 0;
    virtual ~MyAudio();

protected:
    MyAudio();

};


#endif //MYVIDEOEDIT_MYAUDIO_H
