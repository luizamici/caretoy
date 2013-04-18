#include "ct_dataview.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QtGui>

CTDataView::CTDataView(QWidget *parent)
    : QWidget(parent)
{
    data_1.reserve(500);
    data_2.reserve(500);
    index = 0;
    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->setMargin(10);
    hLayout1->addSpacing(10);
    this->setMinimumSize(950,250);
    this->setLayout(hLayout1);
    plot_data = new CTPlot(this);
    hLayout1->addWidget(plot_data,1);
//    generateDataForPlotting();
//    sendGeneratedDataToPlot();
//    generateReport();
    zoomEnabled = false;

    connect(plot_data,SIGNAL(skipToSelection(qint64)),this, SIGNAL(skipToSelection(qint64)));
}

void CTDataView::initialize()
{
    generateDataForPlotting();
    sendGeneratedDataToPlot();
}

void CTDataView::generateDataForPlotting(){

    QFile *file = new QFile("./Source/output.xml");
    if (!file->open(QIODevice::ReadOnly))
        return;

    bool getChar = false;
    QStringList valueList_sensor1;
    QStringList valueList_sensor2;
    QXmlStreamReader reader(file);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "UToyValues")
        {
            qDebug() << "UToyValues";
            getChar = true;
        }
        if(reader.isStartElement() && reader.name() == "Values" && getChar == true)
        {
            valueList_sensor1 = reader.attributes().value("UToy1").toString().split(" ");
            valueList_sensor2 = reader.attributes().value("UToy2").toString().split(" ");
        }
        if(reader.isEndElement() && reader.name() == "UToyValues")
        {
            getChar = false;
        }
    }

    double values_sensor1[valueList_sensor1.size()];
    double values_sensor2[valueList_sensor2.size()];
    int i = 0;
    foreach(QString value, valueList_sensor1)
    {
        if(!value.trimmed().isEmpty())
        {
//            qDebug() << value;
            values_sensor1[i] = value.toDouble();
            i++;
        }
    }
    i = 0;
    foreach(QString value, valueList_sensor2)
    {
        if(!value.trimmed().isEmpty())
        {
//            qDebug() << value;
            values_sensor2[i] = value.toDouble();
            i++;
        }
    }

    qDebug() << i;
    for(int j = 0; j < i ; j++)
    {
        const QPointF s1((double)j/10,values_sensor1[j]);
        const QPointF s2((double)j/10,values_sensor2[j]);
//        qDebug()<< s1 << s2;
        data_1.append(s1);
        data_2.append(s2);
    }
//    for(double i=0.0 ;data.size()< 500; i = i + 0.1 ){
//        const QPointF s(i,(double)qrand()/100000000);
//        data.append(s);
//    }
}

void CTDataView::sendGeneratedDataToPlot(){
    plot_data->setSamples_data_1(data_1);
    plot_data->setSamples_data_2(data_2);


//    foreach(QPointF pointToPlot, data){
////      qDebug() << "########Point to plot:" << pointToPlot;
//        data_toPush.append(pointToPlot);
//        plot_data->setSamples(data_toPush);
//    }
}

void CTDataView::signalDecreaseMarker(){
    plot_data->decreaseMarker();
}

void CTDataView::increaseMarker(){
    plot_data->increaseMarker();
}

void CTDataView::stopMarker(){
    plot_data->resetMarker();
}

void CTDataView::pauseMarker(){
    qDebug() << "stop updatingMarker!";
}

void CTDataView::trackMarker(bool b){

    if(b && zoomEnabled){
        qDebug() << "CTDataView::trackMarker() enabling marker tracking";
        plot_data->enableMarkerTracking(true);}
    else{
        qDebug() << "CTDataView::trackMarker() disabling marker tracking";
        plot_data->enableMarkerTracking(false);
        }
}

CTDataView::~CTDataView()
{
}

void CTDataView::enableZoom(bool b){

    plot_data->enableZoom(b);
    this->zoomEnabled = b;
}

