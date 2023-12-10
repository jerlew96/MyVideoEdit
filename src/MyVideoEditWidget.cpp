//
// Created by Xianming Liu on 06.12.23.
//

#include "Headers/MyVideoEditWidget.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>

using namespace cv;

MyVideoEditWidget::MyVideoEditWidget(QWidget *p) : QOpenGLWidget(p){

}
void MyVideoEditWidget::paintEvent(QPaintEvent *e){

    QPainter p;
    p.begin(this);
    p.drawImage(QPoint(0,0), img);
    p.end();

}
MyVideoEditWidget::~MyVideoEditWidget(){

}

void MyVideoEditWidget::setImage(cv::Mat mat) {

    QImage::Format fmt = QImage::Format_RGB888;
    int pixelSize = 3;
    //判断是否为灰度图
    if(mat.type() == CV_8UC1){
        fmt = QImage::Format_Grayscale8;
        pixelSize = 1;
    }

    if(img.isNull() || img.format() !=fmt){
        delete img.bits();
        uchar *buf = new uchar[width()*height()*pixelSize];
        img = QImage(buf,width(), height(), fmt);
    }
    Mat dst;
    cv::resize(mat, dst, Size(img.size().width(), img.size().height()));

    if(pixelSize > 1){
        cv::cvtColor(dst, dst, COLOR_BGR2RGB);
    }

    //复制内存空间
    memcpy(img.bits(), dst.data, dst.cols*dst.rows*dst.elemSize());
    update();
}