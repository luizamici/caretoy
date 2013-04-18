#ifndef PLOT_H
#define PLOT_H

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"

class Plot : public QwtPlot
{

public:
    Plot(QWidget *parent = 0);
    void setSamples(QVector<QPointF> dataSamples);
    void updateMarker();
    void decreaseMarker();
    void increaseMarker();

private:
    QwtPlotCurve *d_curve;
    QwtPlotMarker *d_origin;
    QwtText y_val;
    QVector<QPointF> data;
    QPointF current_point;
    int current_index;

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void showEvent(QShowEvent *);



};

#endif // PLOT_H
