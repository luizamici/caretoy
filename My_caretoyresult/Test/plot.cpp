#include "plot.h"
#include "qwt_plot_grid.h"


Plot::Plot(QWidget *parent) :  QwtPlot(parent)
{
    setAxisTitle(QwtPlot::xBottom, "Time [s]");
    setAxisScale(QwtPlot::xBottom, 0.0, 50.0);
    setAxisScale(QwtPlot::yLeft, -5.0, 25.0);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QPen(Qt::gray, 0.0, Qt::DotLine));
    grid->enableX(true);
    grid->enableXMin(true);
    grid->enableY(true);
    grid->enableYMin(false);
    grid->attach(this);

    d_origin = new QwtPlotMarker();
    d_origin->setLineStyle(QwtPlotMarker::Cross);
    d_origin->setValue(0.0, 0.0);
    d_origin->setLinePen(QPen(Qt::gray, 0.0, Qt::DashLine));
    d_origin->attach(this);

    d_curve = new QwtPlotCurve();
    d_curve->setStyle(QwtPlotCurve::Lines);
    d_curve->setPen(QPen(Qt::green));
    d_curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    d_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    d_curve->attach(this);
}

void Plot::setSamples(QVector<QPointF> dataSamples){

    data = dataSamples;
    d_curve->setSamples(data);
    d_curve->attach(this);
    updateMarker();
    replot();
}

void Plot::updateMarker(){

    current_index = data.size()-1;
    current_point = data.at(current_index);
    d_origin->setValue(current_point.x(), 0.0);
    y_val = QwtText (QString::number (current_point.y()));
    y_val.setColor(QColor(255, 0, 0, 227));
    d_origin->setLabel(y_val);
    d_origin->attach(this);
}

void Plot::decreaseMarker(){

    current_index = current_index -1 ;
    current_point = data.at(current_index);
    d_origin->setValue(current_point.x(), 0.0);
    y_val = QwtText (QString::number (current_point.y()));
    y_val.setColor(QColor(255, 0, 0, 227));
    d_origin->setLabel(y_val);
    d_origin->attach(this);
    replot();
}

void Plot::increaseMarker(){
    if(current_index >= data.size()+1){qDebug() << "Beyond the size of the data!";}
    else{
        current_index = current_index + 1;
        current_point = data.at(current_index);
        d_origin->setValue(current_point.x(),0.0);
        y_val = QwtText(QString::number(current_point.y()));
        y_val.setColor(QColor(255,0,0,227));
        d_origin->setLabel(y_val);
        d_origin->attach(this);
        replot();
    }
}

void Plot::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resizeEvent called";
    QwtPlot::resizeEvent(event);
}

void Plot::showEvent( QShowEvent * )
{
    qDebug() << "showEvent called";
    replot();
}
