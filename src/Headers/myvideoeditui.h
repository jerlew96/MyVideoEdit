//
// Created by Xianming Liu on 06.12.23.
//

#ifndef MYVIDEOEDIT_MYVIDEOEDITUI_H
#define MYVIDEOEDIT_MYVIDEOEDITUI_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MyVideoEditUI; }
QT_END_NAMESPACE

class MyVideoEditUI : public QWidget {
    Q_OBJECT

public:
    explicit MyVideoEditUI(QWidget *parent = nullptr);

    void timerEvent(QTimerEvent *e);
    ~MyVideoEditUI() override;
public slots:
    void Open();
    void Play();
    void Pause();

    void SliderPress();
    void SliderRelease();
    void SetPos(int pos);    //滑动条拖动
    void Left(int pos);
    void Right(int pos);

    void Set();     //设置过滤器
    void Export();
    void ExportEnd();    //导出结束

    void Mark();  //水印
    void Blend();  //融合
    void Merge();


private:
    Ui::MyVideoEditUI *ui;
};


#endif //MYVIDEOEDIT_MYVIDEOEDITUI_H
