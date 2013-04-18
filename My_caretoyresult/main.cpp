#include <QtGui/QApplication>
#include "ct_resultview.h"
#include <QtGui>

#include "ct_dataview.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyle(new QWindowsStyle);
    CTResultView w;
    w.show();

//    RandomData random_data;
//    w.connect(&w, SIGNAL(startButtonClicked()),&random_data,SLOT(startSendingSamples()));
//    w.connect(&w,SIGNAL(stopButtonClicked()),&random_data,SLOT(stopSendingSamples()));

    bool ok = a.exec();

    return ok;

}
