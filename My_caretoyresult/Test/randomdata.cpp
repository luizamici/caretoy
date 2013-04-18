#include "randomdata.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QtGui>

RandomData::RandomData(QWidget *parent)
    : QWidget(parent)
{
    data.reserve(500);
    elapsed_timer.start();
    id_timer = startTimer(100);
    data_creation = true;
    index = 0;

    QVBoxLayout* vLayout = new QVBoxLayout();
    QPushButton *stop = new QPushButton(tr("Stop"));
    QPushButton *play = new QPushButton(tr("Play"));
    QPushButton *forward = new QPushButton(tr(">>"));
    QPushButton *backword = new QPushButton(tr("<<"));
    vLayout->setMargin(20);
    vLayout->addWidget(stop);
    vLayout->addWidget(play,1);
    vLayout->addWidget(forward);
    vLayout->addWidget(backword);
    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->setMargin(20);
    hLayout1->addSpacing(20);
    this->setMinimumSize(900,450);
    this->setLayout(hLayout1);
    plot_data = new Plot(this);
    hLayout1->addWidget(plot_data);
    hLayout1->addLayout(vLayout);


    connect(stop,SIGNAL(clicked()),this, SLOT(stopSendingSamples()));
    connect(play,SIGNAL(clicked()),this, SLOT(startSendingSamples()));
    connect(backword, SIGNAL(clicked()),this, SLOT(signalDecreaseMarker()));
    connect(forward,SIGNAL(clicked()),this,SLOT(signalIncreaseMarker()));
}


//This part of the code is also helpful for the presentation, but will be
//discarded later when the real data is available
void RandomData::generateData(){

    qDebug() << "populating the vector of Points!";
    const QPointF s((double)elapsed_timer.elapsed()/1000,(double)qrand()/100000000);
    data.append(s);
}

void RandomData::stopPopulation(){

    elapsed_timer.invalidate();
    killTimer(id_timer);
    data_creation = false;
    startSendingSamples();

}


void RandomData::timerEvent(QTimerEvent *event){

    //first part of the timerEvent is neccessary for the presentation
    //the generation of the data will not be neccessary once the real data
    //is obtained from the db!
    if(data_creation){
        if(data.size() < 500){
            qDebug() << "the size of the vector" << data.size();
            generateData();}
        else{
            stopPopulation();
            emit dataGenerated();
        }
    }else{
        if(data_toPush.size() < 500){
        data_toPush.append(data.at(index));
        index++;
        plot_data->setSamples(data_toPush);
        }
    }
}


void RandomData::startSendingSamples(){

    //start a new timer for passing each sample at a time to the
    //plotter, so that it looks real time
    id_timer2 = startTimer(100);
    elapsed_timer.start();
}

void RandomData::stopSendingSamples(){

    elapsed_timer.invalidate();
    killTimer(id_timer2);
    data_creation = false;
    current_index = index;
}

void RandomData::signalDecreaseMarker(){
    plot_data->decreaseMarker();
}

void RandomData::signalIncreaseMarker(){
    plot_data->increaseMarker();
}

RandomData::~RandomData()
{
}
