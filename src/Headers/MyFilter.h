//
// Created by Xianming Liu on 07.12.23.
//

#ifndef MYVIDEOEDIT_MYFILTER_H
#define MYVIDEOEDIT_MYFILTER_H
#include <opencv2/core.hpp>
#include <vector>

enum TaskType{
    TASK_NONE,
    TASK_GAIN,
    TASK_ROTATE90,
    TASK_ROTATE180,
    TASK_ROTATE270,
    TASK_FLIPX,
    TASK_FLIPY,
    TASK_FLIPXY,
    TASK_RESIZE,
    TASK_PYDOWN,
    TASK_PYUP,
    TASK_CUT,
    TASK_GRAY,
    TASK_MARK,
    TASK_BLEND,
    TASK_MERGE
};
struct EditTask{
    TaskType type;
    std::vector<double> para;
};

class MyFilter {

public:
    virtual ~MyFilter();

    static MyFilter * Get();
    virtual cv::Mat Filter(cv::Mat mat1, cv::Mat mat2) = 0;
    virtual void Add(EditTask task) = 0;
    virtual void Clear() = 0;

protected:
    MyFilter();
};


#endif //MYVIDEOEDIT_MYFILTER_H
