#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // Changed from QCoreApplication

    MainWindow window;
    window.show();

    return app.exec();
}
