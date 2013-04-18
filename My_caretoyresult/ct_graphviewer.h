#ifndef CT_GRAPHVIEWER_H
#define CT_GRAPHVIEWER_H

#include <QWidget>
#include "ct_plot.h"

namespace Ui {
class CTGraphViewer;
}

class CTGraphViewer : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTGraphViewer(QWidget *parent = 0);
    ~CTGraphViewer();

    void initGraph(int number_of_curves, QStringList names_of_curves);
    void setCurveData(QVector<QPointF> plot_points, int id_curve);
    
private:
    Ui::CTGraphViewer *ui;

    CTPlot *plot_data;
    bool zoomEnabled;
    void enableZoom(bool b);
    void trackMarker(bool b);

signals:
     void skipToSelection(qint64 time);
     void zoom_toggled(bool b);

private slots:
    void on_qtb_zoom_clicked(bool checked);
    void on_qtb_zoom_toggled(bool checked);
public slots:
    void increaseMarker();
    void stopMarker();
    void pauseMarker();
};

#endif // CT_GRAPHVIEWER_H
