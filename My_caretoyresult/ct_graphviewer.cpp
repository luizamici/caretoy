#include "ct_graphviewer.h"
#include "ui_ct_graphviewer.h"

CTGraphViewer::CTGraphViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTGraphViewer)
{
    ui->setupUi(this);
    this->setMinimumSize(950,250);
    zoomEnabled = false;
    plot_data = new CTPlot(this);
    this->layout()->addWidget(plot_data);

    connect(plot_data,SIGNAL(skipToSelection(qint64)),this,
            SIGNAL(skipToSelection(qint64)));
}

void CTGraphViewer::initGraph(int number_of_curves, QStringList
                              names_of_curves)
{
    plot_data->setCurves(number_of_curves, names_of_curves);
}

void CTGraphViewer::setCurveData(QVector<QPointF> plot_points, int id_curve)
{
    plot_data->setCurveData(plot_points, id_curve);
}

void CTGraphViewer::enableZoom(bool b){

    plot_data->enableZoom(b);
    this->zoomEnabled = b;
}

void CTGraphViewer::trackMarker(bool b){

    if(b && zoomEnabled){
        qDebug() << "CTDataView::trackMarker() enabling marker tracking";
        plot_data->enableMarkerTracking(true);}
    else{
        qDebug() << "CTDataView::trackMarker() disabling marker tracking";
        plot_data->enableMarkerTracking(false);
        }
}

void CTGraphViewer::increaseMarker(){
    plot_data->increaseMarker();
}

void CTGraphViewer::stopMarker(){
    plot_data->resetMarker();
}

void CTGraphViewer::pauseMarker(){
    qDebug() << "stop updatingMarker!";
}

void CTGraphViewer::on_qtb_zoom_toggled(bool checked)
{
    qDebug() << "toggled" << checked;
    enableZoom(checked);
    trackMarker(checked);
    emit zoom_toggled(checked);
}


void CTGraphViewer::on_qtb_zoom_clicked(bool checked)
{
    qDebug() << " -.-' ";
}


CTGraphViewer::~CTGraphViewer()
{
    delete ui;
}
