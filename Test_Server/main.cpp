#include <QtGui/QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server sserver(44444);

    return a.exec();
}
