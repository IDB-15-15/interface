#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a {argc, argv};
    Browser::MainWindow w;
    w.show();

    return a.exec();
}
