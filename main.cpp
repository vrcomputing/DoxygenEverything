#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

    #ifdef __SANITIZE_ADDRESS__
        printf("Address sanitizer enabled");
    #else
        printf("Address sanitizer not enabled");
    #endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}