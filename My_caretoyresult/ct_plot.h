#ifndef CT_PLOT_H
#define CT_PLOT_H

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_panner.h"
#include "ct_plotmagnifier.h"
#include "qwt_plot_picker.h"
#include "ct_plotzoomer.h"
#include "qwt_system_clock.h"
#include "QTimer"

class CTPlot : public QwtPlot
{
    Q_OBJECT

public:
    CTPlot(QWidget *parent = 0);

    void setCurves(int number_of_curves, QStringList names_of_curves);
    void setCurveData(QVector<QPointF> samples, int id_curve);

    void setSamples_data_1(QVector<QPointF> dataSamples);
    void setSamples_data_2(QVector<QPointF> dataSamples);
    void decreaseMarker();
    void increaseMarker();
    void resetMarker();
    void enableZoom(bool b);
    void enableMarkerTracking(bool b);

    qint64 time_of_selection;
    qint64 getTimeOfSelection();
    int id_timer;

private:
    QList<QPen> list_of_colors;
    QList<QwtPlotCurve*> list_of_curves;
    QwtPlotCurve *d_curve;

    QwtPlotCurve *d_curve_1;
    QwtPlotCurve *d_curve_2;


    QwtPlotMarker *d_origin;
    QwtText y_val;
    QVector<QPointF> data;
    QPointF current_point;
    QwtPlotPicker *picker;
    CTPlotZoomer *zoomer;
    QwtPlotPanner *panner;
    CTPlotMagnifier *magnifier;
    QwtScaleDiv scaleDiv;
    int current_index;
    bool zoomEnabled;
    QRectF scaleRect;

    void updateMarker(QPointF currentPoint);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void skipToSelection(qint64 time);
    void alertScaleRectChanged();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void showEvent(QShowEvent *);
    void timerEvent(QTimerEvent *event);

public slots:
    void onZoom(QRectF rect);

    //this is for test
    void printCurrent(double scaleDivRangeZoomed);

};

#endif // CT_PLOT_H
