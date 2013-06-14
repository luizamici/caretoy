#include <QtGui/QApplication>
#include "ct_resultview.h"
#include <QtGui>

#include "ct_dataview.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTResultView w;
    w.show();

    bool ok = a.exec();
    return ok;
}
