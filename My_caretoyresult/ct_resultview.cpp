#include "ct_resultview.h"
#include "ui_ct_resultview.h"
#include <QStatusBar>

CTResultView::CTResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTResultView)
{
    ui->setupUi(this);
    this->setMinimumSize(1100, 700);

   //This is a toolButton that helps switching between zoomed mode and normal mode of the plot
    generateReport();
    generateDataForPlotting();

    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
//    statusBar->setSizeGripEnabled(false);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    statusBar->showMessage("Press 'Zoom' for enabling the zooming function on the graph");
    ui->status_bar_layout->addWidget(statusBar);

    connect(ui->qwt_graphviewer, SIGNAL(zoom_toggled(bool)),this, SLOT(showMessage(bool)));
    connect(ui->qwgVideoView,SIGNAL(videoPlaying()),ui->qwt_graphviewer,SLOT(increaseMarker()));
    connect(ui->qwgVideoView,SIGNAL(stopPlaying()),ui->qwt_graphviewer,SLOT(stopMarker()));
    connect(ui->qwgVideoView,SIGNAL(pausePlaying()),ui->qwt_graphviewer,SLOT(pauseMarker()));
    connect(ui->qwt_graphviewer,SIGNAL(skipToSelection(qint64)),ui->qwgVideoView,SLOT(seek(qint64)));
}

void CTResultView::showMessage(bool b){
    if(b){statusBar->showMessage("Select an area for zooming in");}
    else{statusBar->showMessage("Press 'Zoom' for enabling the zooming function on the graph");}
}

void CTResultView::generateReport()
{
//    QFile *file = new QFile("./Source/output.xml");
//    if (!file->open(QIODevice::ReadOnly))
//        return;
//    QXmlStreamReader reader(file);
    QString notes;
//    while(!reader.atEnd())
//    {
//        reader.readNext();
//        if(reader.isStartElement() && reader.name() != "Values" && reader.name() != "UToyValues")
//        {
//            notes += reader.attributes().value("Label").toString() + "\n";
//        }
//        if(reader.isStartElement() && reader.name() == "UToyValues")
//        {
//            break;
//        }
//        if(reader.isStartElement() && reader.name() == "Values")
//        {
//            if(!reader.attributes().isEmpty()){
//                notes += QString("UToy sensor 1 : ").leftJustified(4) +
//                        reader.attributes().value("UToy1").toString().leftJustified(6);
//                notes += QString("UToy sensor 2 : ").leftJustified(4) +
//                        reader.attributes().value("UToy2").toString().leftJustified(4) + "\n\n";
//            }
//        }
//    }
    notes = "Number of grasps \n"
            "   UToy sensor 1 = 17.0  UToy sensor 2 = 14.0 \n\n"

            "Grasp latency \n"
            "   UToy sensor 1 = 0     UToy sensor 2 = 0 \n\n"

            "Grasp strength \n"
            "   UToy sensor 1 = 1.1   UToy sensor 2 = 1.1 \n\n"

            "Button presses \n"
            "Button press latency\n"
            "Leans forward\n"
            "Head lifts\n"
            "   Mat = 0.0 \n\n"

            "Weigth distribution\n"
            "   Mat = 0 \n\n";
    ui->notesText->setPlainText(notes);
}


void CTResultView::generateDataForPlotting(){

    QFile *file = new QFile("./Source/output.xml");
    if (!file->open(QIODevice::ReadOnly))
        return;

    bool getChar = false;
    QStringList valueList_sensor1;
    QStringList valueList_sensor2;
    QVector<QPointF> data_1;
    QVector<QPointF> data_2;
    QXmlStreamReader reader(file);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "UToyValues")
        {
            qDebug() << "UToyValues";
            getChar = true;
        }
        if(reader.isStartElement() && reader.name() == "Values" && getChar == true)
        {
            valueList_sensor1 = reader.attributes().value("UToy1").toString().split(" ");
            valueList_sensor2 = reader.attributes().value("UToy2").toString().split(" ");
        }
        if(reader.isEndElement() && reader.name() == "UToyValues")
        {
            getChar = false;
        }
    }

    double values_sensor1[valueList_sensor1.size()];
    double values_sensor2[valueList_sensor2.size()];
    int i = 0;
    foreach(QString value, valueList_sensor1)
    {
        if(!value.trimmed().isEmpty())
        {
            values_sensor1[i] = value.toDouble();
            i++;
        }
    }
    i = 0;
    foreach(QString value, valueList_sensor2)
    {
        if(!value.trimmed().isEmpty())
        {
            values_sensor2[i] = value.toDouble();
            i++;
        }
    }
    for(int j = 0; j < i ; j++)
    {
        const QPointF s1((double)j/10,values_sensor1[j]);
        const QPointF s2((double)j/10,values_sensor2[j]);
        data_1.append(s1);
        data_2.append(s2);
    }

    QStringList s = QStringList();
    s  << "UToy sensor1" << "UToy sensor2";
    ui->qwt_graphviewer->initGraph(2, s);
    ui->qwt_graphviewer->setCurveData(data_1, 1);
    ui->qwt_graphviewer->setCurveData(data_2, 2);
}


CTResultView::~CTResultView()
{
    delete ui;
}
