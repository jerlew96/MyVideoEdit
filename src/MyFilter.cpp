//
// Created by Xianming Liu on 07.12.23.
//

#include "MyFilter.h"
#include <QMutex>
#include "MyImagePro.h"

class CMyFilter : public MyFilter{
public:
    std::vector<EditTask> tasks;
    QMutex mutex;
    cv::Mat Filter(cv::Mat mat1, cv::Mat mat2){
        mutex.lock();
        MyImagePro pro;
        pro.Set(mat1, mat2);
        auto task_size = tasks.size();
        for (int i = 0; i < task_size; i++){
            switch (tasks[i].type) {
                case TASK_GAIN:
                    pro.Gain(tasks[i].para[0], tasks[i].para[1]);
                    break;
                case TASK_ROTATE90:
                    pro.Rotate90();
                    break;
                case TASK_ROTATE180:
                    pro.Rotate180();
                    break;
                case TASK_ROTATE270:
                    pro.Rotate270();
                    break;
                case TASK_FLIPX:
                    pro.FlipX();
                    break;
                case TASK_FLIPY:
                    pro.FlipY();
                    break;
                case TASK_FLIPXY:
                    pro.FlipXY();
                    break;
                case TASK_RESIZE:
                    pro.Resize(tasks[i].para[0], tasks[i].para[1]);
                case TASK_PYDOWN:
                    pro.PyramidDown(tasks[i].para[0]);
                    break;
                case TASK_PYUP:
                    pro.PyramidUp(tasks[i].para[0]);
                    break;
                case TASK_CUT:
                    pro.Cut(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
                    break;
                case TASK_GRAY:
                    pro.Gray();
                    break;
                case TASK_MARK:
                    pro.Mark(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2]);
                    break;
                case TASK_BLEND:
                    pro.Blend(tasks[i].para[0]);
                    break;
                case TASK_MERGE:
                    pro.Merge();
                    break;
                default:
                    break;
            }
        }
        cv::Mat re = pro.Get();
        mutex.unlock();
        return re;
    }
    void Add(EditTask task){
        mutex.lock();
        tasks.push_back(task);
        mutex.unlock();

    }
    void Clear(){
        mutex.lock();
        tasks.clear();
        mutex.unlock();

    }

};
MyFilter::MyFilter(){

}
MyFilter::~MyFilter(){

}
MyFilter *MyFilter::Get() {
    static CMyFilter cmf;
    return &cmf;
}
