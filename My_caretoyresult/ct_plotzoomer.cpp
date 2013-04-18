#include "ct_plotzoomer.h"
#include "qwt_plot_zoomer.h"

CTPlotZoomer::CTPlotZoomer(QwtPlotCanvas *parent) :
    QwtPlotZoomer(parent)
{
}

void CTPlotZoomer::scaleRectChanged(){
    emit zoomed(zoomRect());
}
