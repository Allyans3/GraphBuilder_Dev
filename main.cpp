#include "mainwindow.h"
#include "auth.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    Auth au;
//    au.show();

    return a.exec();
}
