#include "MainWindow.h"
#include "DatabaseHolder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // init database before window creation
    if (!DatabaseHolder::getInstance().init())
        return 1;

    MainWindow w;
    w.show();
    return a.exec();
}
