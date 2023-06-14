#include "MainWindow.h"
#include "DatabaseHolder.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // init database before window creation
    if (!DatabaseHolder::getInstance().init())
    {
        QMessageBox::critical(nullptr, "OperatorEditor", "Database was not initialized");
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
