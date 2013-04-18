#include "ct_plotmagnifier.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot.h"
#include "qwt_scale_div.h"

CTPlotMagnifier::CTPlotMagnifier(QwtPlotCanvas *parent) :
    QwtPlotMagnifier(parent)
{
    center = 0.0;
}

void CTPlotMagnifier::rescale(double factor){
    if ( factor == 1.0 || factor == 0.0 )
             return;

         bool doReplot = false;
         QwtPlot* plt = plot();

         const bool autoReplot = plt->autoReplot();
         plt->setAutoReplot(false);
         int axisId = QwtPlot::xBottom;
         const QwtScaleDiv *scaleDiv = plt->axisScaleDiv(axisId);
         if ( isAxisEnabled(axisId) && scaleDiv->isValid() )
         {
              if(center == 0 ){center = scaleDiv->lowerBound() + scaleDiv->range() / 2;}
              emit zoomed(scaleDiv->range());
              const double width_2 = scaleDiv->range() / 2 * factor;

              plt->setAxisScale(axisId, center - width_2, center + width_2);
              doReplot = true;
          }
         plt->setAutoReplot(autoReplot);
         if ( doReplot )
         plt->replot();
}

void CTPlotMagnifier::setCenter(QPointF point){
    this->center = (double)point.x();
}
