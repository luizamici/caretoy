#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <QtGui/QWidget>
#include <QElapsedTimer>
#include "plot.h"

class RandomData : public QWidget
{
    Q_OBJECT
    
public:
    RandomData(QWidget * = 0);
    ~RandomData();

    void generateData();
    void stopPopulation();
    void timerEvent(QTimerEvent *);

    Plot *plot_data;
    QVector<QPointF> data;
    int id_timer;
    int id_timer2;
    QElapsedTimer elapsed_timer;

signals:
    void dataGenerated();

private:

    bool data_creation;
    int index;
    int current_index;
    QVector<QPointF> data_toPush;

public slots:
    void startSendingSamples();
    void stopSendingSamples();
    void signalDecreaseMarker();
    void signalIncreaseMarker();
};

#endif // RANDOMDATA_H
