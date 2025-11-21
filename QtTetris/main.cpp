#include "mainwindow.h"

#include <QApplication>

#include <cstdlib> // For srand and rand
#include <ctime>   // For time

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Seed with current time

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
