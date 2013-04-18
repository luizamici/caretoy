#include <QtGui/QApplication>
#include "randomdata.h"
#include "plot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RandomData w;
    w.show();
    return a.exec();
}
