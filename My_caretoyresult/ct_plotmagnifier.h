#ifndef CT_PLOTMAGNIFIER_H
#define CT_PLOTMAGNIFIER_H
#include "qwt_plot_canvas.h"
#include "qwt_plot_magnifier.h"

class CTPlotMagnifier : public QwtPlotMagnifier
{
    Q_OBJECT
public:
    explicit CTPlotMagnifier(QwtPlotCanvas *parent = 0);
    virtual void rescale(double factor);

    void setCenter(QPointF point);

private:
    double center;

signals:
    void zoomed(double scaleDivRangeZoomed);
public slots:

};

#endif // CT_PLOTMAGNIFIER_H
