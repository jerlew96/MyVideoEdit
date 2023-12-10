//
// Created by Xianming Liu on 06.12.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyVideoEditUI.h" resolved

#include "Headers/myvideoeditui.h"
#include "Forms/ui_MyVideoEditUI.h"
#include <QFileDialog>
#include <QMessageBox>
#include "MyVideoEditThread.h"
#include <string>
#include "MyFilter.h"
#include "MyAudio.h"

static bool pressSlider = false;
static bool isExport = false;
static bool isColor = true;
static bool isMark = false;
static bool isBlend = false;
static bool isMerge = false;

MyVideoEditUI::MyVideoEditUI(QWidget *parent) :
        QWidget(parent), ui(new Ui::MyVideoEditUI) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    //原始图像显示信号
    QObject::connect(MyVideoEditThread::Get(),
                     SIGNAL(ImageDisplay1(cv::Mat)),
                     ui->VideoSrc,
                     SLOT(setImage(cv::Mat))
                     );

    //原始图像显示信号2
    QObject::connect(MyVideoEditThread::Get(),
                     SIGNAL(ImageDisplay3(cv::Mat)),
                     ui->VideoSrc2,
                     SLOT(setImage(cv::Mat))
    );

    //输出图像显示信号
    QObject::connect(MyVideoEditThread::Get(),
                     SIGNAL(ImageDisplay2(cv::Mat)),
                     ui->VideoDst,
                     SLOT(setImage(cv::Mat))
    );
    //导出视频结束
    QObject::connect(MyVideoEditThread::Get(),
                             SIGNAL(SaveEnd()),
                             this,
                             SLOT(ExportEnd())
    );
    Pause();
    startTimer(40);
}

void MyVideoEditUI::timerEvent(QTimerEvent *e) {
    if(pressSlider)return;
    double pos = MyVideoEditThread::Get()->getPos();
    ui->playSlider->setValue(pos*1000);
}

void MyVideoEditUI::Open(){

    QString name = QFileDialog::getOpenFileName(this, "Choose video file");
    if(name.isEmpty()){
        return;
    }
    std::string file = name.toLocal8Bit().data();
    if(!MyVideoEditThread::Get()->Open(file)){
        QMessageBox::information(this, "Error", name + " open failed!");
        return;
    }
    //QMessageBox::information(this, "", name);
}

MyVideoEditUI::~MyVideoEditUI() {
    delete ui;
}

void MyVideoEditUI::SliderPress() {
    pressSlider = true;
}

void MyVideoEditUI::SliderRelease() {
    pressSlider = false;
}

void MyVideoEditUI::SetPos(int pos) {

    MyVideoEditThread::Get()->seek((double)pos/1000);
}

void MyVideoEditUI::Left(int pos) {
    MyVideoEditThread::Get()->SetBegin((double)pos/1000);
    SetPos(pos);
}

void MyVideoEditUI::Right(int pos) {
    MyVideoEditThread::Get()->SetEnd((double)pos/1000);
}


void MyVideoEditUI::Set() {
    //清理初始化
    MyFilter::Get()->Clear();
    isColor = true;
    //视频画面裁剪
    bool isCut = false;
    int x = ui->cutX->value();
    int y = ui->cutY->value();
    int w = ui->cutW->value();
    int h = ui->cutH->value();
    if(x>0||y>0||w>0||h>0){
        isCut = true;
        MyFilter::Get()->Add(EditTask{
            TASK_CUT,
            {(double)x, (double)y,
             (double)w, (double)h}
        });
//        double w_ = MyVideoEditThread::Get()->width;
//        double h_ = MyVideoEditThread::Get()->height;
//        MyFilter::Get()->Add(EditTask{TASK_RESIZE, {w_,h_}});
        ui->width->setValue(w);
        ui->height->setValue(h);

    }

    //图像金字塔
    bool isPy = false;
    int downCount = ui->pydown->value();
    int upCount = ui->pyup->value();
    if(downCount > 0||upCount > 0){
        if(downCount>0){
            isPy = true;
            MyFilter::Get()->Add(EditTask{TASK_PYDOWN, {(double)downCount}});
            int w = MyVideoEditThread::Get()->width;
            int h = MyVideoEditThread::Get()->height;
            while(downCount){
                w = w>>1;
                h = h>>1;
                upCount--;
            }
            ui->width->setValue(w);
            ui->height->setValue(h);

        }
        if(upCount>0){
            isPy = true;
            MyFilter::Get()->Add(EditTask{TASK_PYUP, {(double)upCount}});
            int w = MyVideoEditThread::Get()->width;
            int h = MyVideoEditThread::Get()->height;
            while(upCount){
                w = w<<1;
                h = h<<1;
                upCount--;
            }

            ui->width->setValue(w);
            ui->height->setValue(h);

        }
    }
    //对比度和亮度
    if(ui->bright->value() > 0 ||
       ui->contrast->value() > 1){
        MyFilter::Get()->Add(EditTask{TASK_GAIN,
                                      {(double)ui->bright->value(),
                                       ui->contrast->value()}
        });
    }

    //灰度图
    if(ui->color->currentIndex() == 1){
        MyFilter::Get()->Add(EditTask{TASK_GRAY});
        isColor = false;
    }

    //图像旋转 1 90 2 180 3 270
    if(ui->rotate->currentIndex() == 1){
        MyFilter::Get()->Add(EditTask{TASK_ROTATE90});
    }
    else if(ui->rotate->currentIndex() == 2){
        MyFilter::Get()->Add(EditTask{TASK_ROTATE180});
    }
    else if(ui->rotate->currentIndex() == 3){
        MyFilter::Get()->Add(EditTask{TASK_ROTATE270});
    }

    //图像镜像
    if(ui->flip->currentIndex() == 1){
        MyFilter::Get()->Add(EditTask{TASK_FLIPX});
    }
    else if(ui->flip->currentIndex() == 2){
        MyFilter::Get()->Add(EditTask{TASK_FLIPY});
    }
    else if(ui->flip->currentIndex() == 3){
        MyFilter::Get()->Add(EditTask{TASK_FLIPXY});
    }

    //图像尺寸
    if(!isMerge&&!isCut&&!isPy&&ui->width->value() > 0 && ui->height->value() > 0){
        MyFilter::Get()->Add(EditTask{TASK_RESIZE,
                                      {(double)ui->width->value(),
                                       (double)ui->height->value()}});
    }

    //水印
    if(isMark){
        double x = ui->markx->value();
        double y = ui->marky->value();
        double a = ui->marktransparent->value();
        MyFilter::Get()->Add(EditTask{TASK_MARK, {x, y, a}});
    }

    //融合
    if(isBlend){
        double a = ui->blendtransparent->value();
        MyFilter::Get()->Add(EditTask{TASK_BLEND, {a}});
    }
    //合并
    if(isMerge){
        double h2 = MyVideoEditThread::Get()->height2;
        double h1 = MyVideoEditThread::Get()->height;
        int w = MyVideoEditThread::Get()->width2*(h2/h1);
        MyFilter::Get()->Add(EditTask{TASK_MERGE});
        ui->width->setValue(MyVideoEditThread::Get()->width+w);
        ui->height->setValue(h1);
    }
}

void MyVideoEditUI::Export() {

    if(isExport){
        //停止导出
        MyVideoEditThread::Get()->StopSave();
        isExport = false;
        ui->exportButton->setText("Start Export");
        return;
    }

    //开始导出
    QString name = QFileDialog::getSaveFileName(
            this, "save", "out1.avi");
    if(name.isEmpty())return;
    std::string filename = name.toLocal8Bit().data();
    if(MyVideoEditThread::Get()->StartSave(filename, ui->width->value(), ui->height->value(), isColor)){
        isExport = true;
        ui->exportButton->setText("Stop Export");
    };
}

void MyVideoEditUI::ExportEnd() {

    isExport = false;
    ui->exportButton->setText("Start Export");

    //处理音频
    std::string src = MyVideoEditThread::Get()->src1File;
    std::string dst = MyVideoEditThread::Get()->dstFile;
    int ss = 0;
    int t = 0;
    ss = MyVideoEditThread::Get()->totalMS * ((double)ui->left->value()/1000.);
    int end = MyVideoEditThread::Get()->totalMS * ((double)ui->right->value()/1000.);
    t = end - ss;
    MyAudio::Get()->ExportAudio(src, src + ".mp3", ss, t);

    std::string tmp = dst + ".avi";
    QFile::remove(tmp.c_str());
    QFile::rename(dst.c_str(), tmp.c_str());
    MyAudio::Get()->Merge(tmp, src + ".mp3", dst);


}
void MyVideoEditUI::Play() {
    ui->pauseButton->show();
    ui->pauseButton->setGeometry(ui->playButton->geometry());
    MyVideoEditThread::Get()->Play();
    ui->playButton->hide();
}

void MyVideoEditUI::Pause() {
    ui->playButton->show();
    ui->pauseButton->hide();
    MyVideoEditThread::Get()->Pause();
}

void MyVideoEditUI::Mark() {
    isMark = false;
    QString name = QFileDialog::getOpenFileName(this, "select image:");

    if(name.isEmpty()){
        return;
    }
    std::string file = name.toLocal8Bit().data();
    Mat mark = imread(file);
    if(mark.empty())return;
    MyVideoEditThread::Get()->SetMark(mark);
    isMark = true;

}

void MyVideoEditUI::Blend() {
    isMark = false;
    isBlend = false;
    QString name = QFileDialog::getOpenFileName(this, "select video:");
    if(name.isEmpty()){
        return;
    }
    std::string file = name.toLocal8Bit().data();
    isBlend = MyVideoEditThread::Get()->Open2(file);
}

void MyVideoEditUI::Merge() {
    isMark = false;
    isBlend = false;
    isMerge = false;
    QString name = QFileDialog::getOpenFileName(this, "select video:");
    if(name.isEmpty()){
        return;
    }
    std::string file = name.toLocal8Bit().data();
    isMerge = MyVideoEditThread::Get()->Open2(file);
}



