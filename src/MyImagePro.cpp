//
// Created by Xianming Liu on 07.12.23.
//

#include "MyImagePro.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

MyImagePro::MyImagePro(){

}
MyImagePro::~MyImagePro(){

}
void MyImagePro::Set(cv::Mat mat1, cv::Mat mat2) {

    if(mat1.empty())return;
    this->src1 = mat1;
    this->src2 = mat2;
    mat1.copyTo(dst);
}

void MyImagePro::Gain(double bright, double contrast) {

    if(dst.empty())return;
    dst.convertTo(dst, -1, contrast, bright);
}

void MyImagePro::Rotate90(){
    if(dst.empty()) return;
    rotate(dst,dst, ROTATE_90_CLOCKWISE);
}
void MyImagePro::Rotate180(){
    if(dst.empty()) return;
    rotate(dst,dst, ROTATE_180);
}
void MyImagePro::Rotate270(){
    if(dst.empty()) return;
    rotate(dst,dst, ROTATE_90_COUNTERCLOCKWISE);
}

void MyImagePro::FlipX(){
    if(dst.empty())return;
    flip(dst, dst, 0);
}
void MyImagePro::FlipY(){
    if(dst.empty())return;
    flip(dst, dst, 1);
}
void MyImagePro::FlipXY(){
    if(dst.empty())return;
    flip(dst, dst, -1);
}

void MyImagePro::Resize(int width, int height) {
    if(dst.empty())return;
    resize(dst, dst, Size(width, height));
}

void MyImagePro::PyramidDown(int count) {
    if(dst.empty())return;
    for(int i = 0; i!=count; i++){
        pyrDown(dst, dst);
    }
}
void MyImagePro::PyramidUp(int count) {
    if(dst.empty())return;
    for(int i = 0; i!=count; i++){
        pyrUp(dst, dst);
    }
}

void MyImagePro::Cut(int x, int y, int w, int h) {

    if(dst.empty())return;
    if(x<0||y<0||w<=0||h<=0) return;
    if(x>dst.cols||y>dst.rows) return;
    dst = dst(Rect(x,y,w,h));
}

void MyImagePro::Gray() {
    if(dst.empty())return;
    cvtColor(dst, dst, COLOR_RGB2GRAY);
}

void MyImagePro::Mark(int x, int y, double a) {
    if(dst.empty())return;
    if(src2.empty())return;
    Mat roi = dst(Rect(x, y, src2.cols, src2.rows));
    addWeighted(src2, a, roi, 1-a, 0, roi);

}

void MyImagePro::Blend(double a) {
    if(dst.empty())return;
    if(src2.empty())return;
    if(src2.size() != dst.size()){
        resize(src2, src2, dst.size());
    }
    addWeighted(src2, a, dst, 1-a, 0, dst);
}

void MyImagePro::Merge() {
    if(dst.empty())return;
    if(src2.empty())return;
    if(src2.size() != dst.size()){
        int w = src2.cols * ((double)src2.rows/(double)dst.rows);
        resize(src2, src2, Size(w, dst.rows));
    }
    int dw = dst.cols + src2.cols;
    int dh = dst.rows;
    dst = Mat(Size(dw, dh), src1.type());
    Mat roi1 = dst(Rect(0,0,src1.cols, dh));
    Mat roi2 = dst(Rect(src1.cols,0,src2.cols, dh));
    src1.copyTo(roi1);
    src2.copyTo(roi2);
}
