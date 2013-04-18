#include "ct_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"
#include "QRectF"
#include "qwt_legend.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_zoomer.h"
#include "ct_plotmagnifier.h"
#include "ct_plotzoomer.h"
#include "QTimerEvent"


CTPlot::CTPlot(QWidget *parent) :  QwtPlot(parent)
{
    setAxisTitle(QwtPlot::xBottom, "Time [s]");
    setAxisScale(QwtPlot::xBottom, 0.0, 200.0);
    setAxisScale(QwtPlot::yLeft, 1.0, 2.5);

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
    y_val = QwtText ("0");
    y_val.setColor(QColor(255, 0, 0, 227));
    d_origin->setLabel(y_val);
    d_origin->attach(this);

    list_of_colors.append(QPen(Qt::green));
    list_of_colors.append(QPen(Qt::red));
    list_of_colors.append(QPen(Qt::magenta));

//    d_curve_1 = new QwtPlotCurve("Pressure sensor 1");
//    d_curve_1->setStyle(QwtPlotCurve::Lines);
//    d_curve_1->setPen(QPen(Qt::green));
//    d_curve_1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
//    d_curve_1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
//    d_curve_1->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
//    d_curve_1->attach(this);

//    d_curve_2 = new QwtPlotCurve("Pressure sensor 2");
//    d_curve_2->setStyle(QwtPlotCurve::Lines);
//    d_curve_2->setPen(QPen(Qt::red));
//    d_curve_2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
//    d_curve_2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
//    d_curve_2->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
//    d_curve_2->attach(this);

    insertLegend(new QwtLegend(), QwtPlot::TopLegend);

    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,QwtPlotPicker::CrossRubberBand,
                                              QwtPicker::AlwaysOn,canvas());
    picker->setStateMachine(new QwtPickerPolygonMachine());
    picker->setRubberBandPen(QColor(Qt::yellow));
    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerPen(QColor(Qt::white)); //QColor bg_color(255, 0, 0, 0) = transparent

    // zoom in/out with the wheel
    magnifier = new CTPlotMagnifier(canvas());

    //a zoomer that permits to chose the interval interesting to the examination
    zoomer = new CTPlotZoomer(canvas());
    zoomer->setRubberBand(QwtPicker::RectRubberBand);
    zoomer->setRubberBandPen(QColor(Qt::green));
    zoomer->setTrackerMode(QwtPicker::ActiveOnly);
    zoomer->setTrackerPen(QColor(Qt::black));
    //In order to do the zoom out by only one level at a time when the Rightbutton of the mouse is clicked
    //and if necessary the total zoom out by pressing CTRL+ RightButton
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton );
    zoomer->setEnabled(false);
    zoomEnabled = false;

    connect(zoomer,SIGNAL(zoomed(QRectF)),this, SLOT(onZoom(QRectF))); //Everytime an interval is chosen to zoom
                                                                       //the new x-axis range is memorized for
                                                                       //the marker tracking
    connect(this, SIGNAL(alertScaleRectChanged()),zoomer, SLOT(scaleRectChanged())); //Everytime the scaleRect is moving
                                                                                     //in order to keep track of the marker
                                                                                     //the zoomer is stimulated to emit zoomed(QRectF)

    connect(magnifier,SIGNAL(zoomed(double)),this, SLOT(printCurrent(double)));
    //    panner = new QwtPlotPanner(canvas());
    //    panner->setMouseButton(Qt::MidButton);
    //    panner->setEnabled(true);
}

void CTPlot::enableZoom(bool b){

    zoomEnabled = b;
    //enable zoomer
    zoomer->setEnabled(b);
    //TODO find a workaround for disabling the magnifier
    zoomer->zoom(0);
}

void CTPlot::printCurrent(double scaleDivRangeZoomed){
    //this shows that the scaleDiv range of this plot is not changed
    //when zoomed
    qDebug() << scaleDivRangeZoomed;
}

void CTPlot::onZoom(QRectF rect){

    scaleRect = rect;
//    qDebug() << "CTPlot::onZoom : x-axis leftBoundery :" << scaleRect.x() ;
//    qDebug() << "CTPlot::onZoom : x-axis rightBoundery :" << scaleRect.x() + scaleRect.width();
}

void CTPlot::enableMarkerTracking(bool b){
    if(b == true){
        id_timer = startTimer(100);
    }else{
        killTimer(id_timer);
    }
}

void CTPlot::setCurves(int number_of_curves, QStringList names_of_curves)
{
    qDebug() << "CTPlot::setCurves";
    for (int var = 0; var < number_of_curves; ++var) {
        d_curve = new QwtPlotCurve(names_of_curves.at(var));
        d_curve->setStyle(QwtPlotCurve::Lines);
        d_curve->setPen(list_of_colors.at(var));
        d_curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        d_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
        d_curve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
        d_curve->attach(this);
        list_of_curves.append(d_curve);
    }
}

void CTPlot::setCurveData(QVector<QPointF> samples, int id_curve)
{
    data = samples;
    list_of_curves.at(id_curve - 1)->setSamples(data);
    list_of_curves.at(id_curve - 1)->attach(this);
    replot();
}

void CTPlot::setSamples_data_1(QVector<QPointF> dataSamples){

//    data = dataSamples;
//    d_curve_1->setSamples(data);
//    d_curve_1->attach(this);
//    replot();
}

void CTPlot::setSamples_data_2(QVector<QPointF> dataSamples){

//    data = dataSamples;
//    d_curve_2->setSamples(data);
//    d_curve_2->attach(this);
//    replot();
}

void CTPlot::updateMarker(QPointF currentPoint){

    d_origin->setValue(currentPoint.x(), 0.0);
    y_val = QwtText (QString::number (currentPoint.y()));
    d_origin->setLabel(y_val);
    d_origin->attach(this);
    replot();
}

void CTPlot::resetMarker(){

    current_index = 0;
    current_point = data.at(current_index);
    updateMarker(current_point);
    if(zoomEnabled){magnifier->setCenter(current_point);}
}

void CTPlot::decreaseMarker(){

    current_index = current_index -1 ;
    current_point = data.at(current_index);
    updateMarker(current_point);
    if(zoomEnabled){magnifier->setCenter(current_point);}
}

void CTPlot::increaseMarker(){
    if(current_index >= data.size()+1){qDebug() << "Beyond the size of the data!";}
    else{
        current_index = current_index + 1;
        current_point = data.at(current_index);
        updateMarker(current_point);
        if(zoomEnabled){magnifier->setCenter(current_point);}
    }
}

void CTPlot::mouseDoubleClickEvent(QMouseEvent *event){

    //First we get the point on the canvas
    QPoint selection = picker->trackerPosition();

    //update the current index and current point with the closest value OF the data sensor
    double currentPoint_x = floor(invTransform(QwtPlot::xBottom,selection.x())*10)/10; //we take the floor rounding double value
    qDebug() << "closest point's x coordinate on the graph--not canvas!" << currentPoint_x;
    current_index = currentPoint_x * 10; //this is ok for the way I have generated the values for the example
    current_point = data.at(current_index);
    qDebug() << "calculated closest point to the selection which will then be the current point:"<< current_point;

    //in in this way we skip the video to a new position regarding the approximate value we calculated above
    time_of_selection = (qint64)(currentPoint_x * 1000);
    qDebug() << "approximate time of selection:" << time_of_selection;

    emit skipToSelection(time_of_selection);
    updateMarker(current_point); //this point is the selected point on the graph's canvas!
    if(zoomEnabled){magnifier->setCenter(current_point);}
}

qint64 CTPlot::getTimeOfSelection(){
    return this->time_of_selection;
}

void CTPlot::timerEvent(QTimerEvent *event){

    if(event->timerId() == id_timer)
    {
//        qDebug() << "CTPlot::timerEvent() timersId equal!";
        double dx;
        if(scaleRect.width() < 50 && current_point.x() > scaleRect.x() && current_point.x() > scaleRect.x()+ scaleRect.width()/2){
            dx = 0.5;
            qDebug() << "CTPlot::timerEvent moves the zoomer by : " << dx ;
            zoomer->moveBy(dx,0.0);
            emit alertScaleRectChanged();
        }else{/*qDebug() << "No need to move the zoomer!";*/}

    }else{qDebug() << "CTPlot::timerEvent() event->timerId() :" << event->timerId() << " id_timer: " << id_timer;}
}

void CTPlot::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resizeEvent called";
    QwtPlot::resizeEvent(event);
}

void CTPlot::showEvent( QShowEvent * )
{
    qDebug() << "showEvent called";
    replot();
}
