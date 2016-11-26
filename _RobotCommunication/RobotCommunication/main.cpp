#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("NanoRobotics Laboratory");
    a.setApplicationName("Kuka Communication");
    MainWindow w;
    w.show();

    return a.exec();
}
