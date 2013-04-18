#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <QtGui/QWidget>
#include <QXmlStreamReader>
#include <QDebug>
#include <QElapsedTimer>
#include "ct_plot.h"

class CTDataView : public QWidget
{
    Q_OBJECT

public:
    CTDataView(QWidget * = 0);
    ~CTDataView();

    CTPlot *plot_data;
    QVector<QPointF> data_1;
    QVector<QPointF> data_2;

    void initialize();

signals:
    void dataGenerated();
    void skipToSelection(qint64 time);


private:
    int index;
    int current_index;
    bool zoomEnabled;
    QVector<QPointF> data_toPush;
    void sendGeneratedDataToPlot();
    void generateDataForPlotting();

public slots:
    void signalDecreaseMarker();
    void increaseMarker();
    void stopMarker();
    void pauseMarker();
    void trackMarker(bool b); //while playing this signal triggers the tracking of the marker
    void enableZoom(bool b);
};

#endif // RANDOMDATA_H

