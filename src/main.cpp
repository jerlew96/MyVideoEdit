#include <QApplication>
#include "Headers/myvideoeditui.h"
//#include "MyAudio.h"

int main(int argc, char *argv[]) {
//    MyAudio::Get()->ExportAudio("11.mov", "11.mp3");
    QApplication a(argc, argv);
    MyVideoEditUI win;
    win.show();
    return QApplication::exec();
}
