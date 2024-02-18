#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    //QCoreApplication coreApp(argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.exec();
    return 0;
}
