#include "ct_dataview.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QtGui>

CTDataView::CTDataView(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->setMargin(10);
    hLayout1->addSpacing(10);
    this->setMinimumSize(950,250);
    this->setLayout(hLayout1);
    plot_data = new CTPlot(this);
    hLayout1->addWidget(plot_data,1);
    zoomEnabled = false;

    connect(plot_data,SIGNAL(skipToSelection(qint64)),this,
            SIGNAL(skipToSelection(qint64)));
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

