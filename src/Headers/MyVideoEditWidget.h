//
// Created by Xianming Liu on 06.12.23.
//

#ifndef MYVIDEOEDIT_MYVIDEOEDITWIDGET_H
#define MYVIDEOEDIT_MYVIDEOEDITWIDGET_H
#include <QOpenGLWidget>
#include <QWidget>
#include <opencv2/core.hpp>
using namespace cv;

class MyVideoEditWidget :public QOpenGLWidget{

    Q_OBJECT

public:
    MyVideoEditWidget(QWidget *p);
    void paintEvent(QPaintEvent *e);
    virtual ~MyVideoEditWidget();

public slots:

    void setImage(Mat mat);
protected:
    QImage img;
};


#endif //MYVIDEOEDIT_MYVIDEOEDITWIDGET_H
