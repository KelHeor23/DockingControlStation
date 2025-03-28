#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef Q_OS_WIN
        a.setWindowIcon(QIcon(":/icins/Resurses/this-is-fine.ico"));  // Windows
    #else
        a.setWindowIcon(QIcon(":/icins/Resurses/this-is-fine.png"));  // Linux/Mac
    #endif

    MainWindow w;
    w.show();
    return a.exec();
}
