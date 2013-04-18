#ifndef CT_PLOTZOOMER_H
#define CT_PLOTZOOMER_H
#include "qwt_plot_zoomer.h"

class CTPlotZoomer : public QwtPlotZoomer
{
    Q_OBJECT
public:
    explicit CTPlotZoomer(QwtPlotCanvas *parent );
    
signals:

public slots:
    void scaleRectChanged();

};

#endif // CT_PLOTZOOMER_H
