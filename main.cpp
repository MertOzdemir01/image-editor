#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QIcon appIcon("/ImagEditor/favicon.ico");
    a.setWindowIcon(appIcon);
    MainWindow w;
    w.show();
    return a.exec();
}

