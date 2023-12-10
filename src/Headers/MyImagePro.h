//
// Created by Xianming Liu on 07.12.23.
//

#ifndef MYVIDEOEDIT_MYIMAGEPRO_H
#define MYVIDEOEDIT_MYIMAGEPRO_H
#include <opencv2/core.hpp>

class MyImagePro {
public:
    MyImagePro();
    ~MyImagePro();
    // 设置原图， 会清理处理结果
    void Set(cv::Mat mat1, cv::Mat mat2);

    //获取输出结果
    cv::Mat Get(){return dst;}

    //设置亮度(0~100)和对比度(1.0~3.0)
    void Gain(double bright, double contrast);

    void Rotate90();
    void Rotate180();
    void Rotate270();

    void FlipX();
    void FlipY();
    void FlipXY();

    void Resize(int width, int height);

    void PyramidDown(int count);
    void PyramidUp(int count);

    void Cut(int x, int y, int w, int h);   //画面裁切
    void Gray();

    void Mark(int x, int y, double a);    //水印 水印大小需要修改（不能超过分辨率

    void Blend(double a); //融合
    void Merge();  //合并

private:
    //原图
    cv::Mat src1, src2;
    cv::Mat dst;

};


#endif //MYVIDEOEDIT_MYIMAGEPRO_H
