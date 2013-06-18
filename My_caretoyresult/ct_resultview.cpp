#include "ct_resultview.h"
#include "ui_ct_resultview.h"
#include <QStatusBar>

CTResultView::CTResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTResultView)
{
    ui->setupUi(this);
    this->setMinimumSize(1100, 700);

   //This is a toolButton that helps switching between zoomed mode
   //and normal mode of the plot

    /*******************This is temporary******************************/
    QString name = QFileDialog::getOpenFileName(this, "Select xml file",
                                                QDir::currentPath());
    if (!name.isNull())
    {
         QFile *file = new QFile(name);
         QFile *file2 = new QFile(name);
         if (file->open(QIODevice::ReadOnly))
         {
             generateReport(file);

         }
         file->close();
         if(file2->open(QIODevice::ReadOnly))
         {
            generateDataForPlotting(file2);
         }

         file2->close();
    }
    /********************************************************************/

    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
//    statusBar->setSizeGripEnabled(false);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    statusBar->showMessage("Press 'Zoom' for enabling the zooming function "
                           "on the graph");
    ui->status_bar_layout->addWidget(statusBar);

    connect(ui->qwt_graphviewer, SIGNAL(zoom_toggled(bool)),this, SLOT(
                showMessage(bool)));
    connect(ui->qwgVideoView,SIGNAL(videoPlaying()),ui->qwt_graphviewer,SLOT(
                increaseMarker()));
    connect(ui->qwgVideoView,SIGNAL(stopPlaying()),ui->qwt_graphviewer,SLOT(
                stopMarker()));
    connect(ui->qwgVideoView,SIGNAL(pausePlaying()),ui->qwt_graphviewer,SLOT(
                pauseMarker()));
    connect(ui->qwt_graphviewer,SIGNAL(skipToSelection(qint64)),ui->qwgVideoView,
            SLOT(seek(qint64)));
}

void CTResultView::showMessage(bool b){
    if(b){statusBar->showMessage("Select an area for zooming in");}
    else{statusBar->showMessage("Press 'Zoom' for enabling the zooming "
                                "function on the graph");}
}

void CTResultView::generateReport(QFile *file)
{
    QXmlStreamReader reader(file);
    QString notes;
    bool read = false;
    bool readOutcome = false;
    QString type;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "outcome_measures")
        {
            read = true;
        }

        if(reader.isStartElement() && read == true)
        {

            if(reader.attributes().value("enabled") == "true")
            {
                readOutcome = true;
                notes += reader.attributes().value("Label").toString() + "\n";
                type = reader.attributes().value("type").toString();
            }
            if(reader.attributes().value("enabled") == "false")
            {
                readOutcome = false;
            }
        }
        if(reader.isStartElement() && read == true && readOutcome == true
                && reader.name() == "sensor")
        {
            notes += "\t" + type + " sensor " + reader.attributes().value("id").
                    toString() + ": ";
        }

        if(reader.isCharacters() && readOutcome == true && !reader.text().
                toString().trimmed().isEmpty())
        {
            notes += reader.text().toString() + "\n";
        }

        else if(reader.isEndElement() && reader.name() == "outcome_measures")
        {
            break;
        }
    }
    ui->notesText->setPlainText(notes);
}


void CTResultView::generateDataForPlotting(QFile *file2){

    QList<QStringList> listValues_of_sensors;
    QStringList values_sensor;
    QList<QVector<QPointF> > listData;
    QString sensor_type;
    int numberOfSensors = 0;

    QXmlStreamReader reader(file2);
    bool readSensor = false;
    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement() && reader.name() == "sensor_values")
        {
            readSensor = true;
            sensor_type = reader.attributes().value("type").toString();
        }
        if(reader.isCharacters() && readSensor == true && !reader.text().
                toString().trimmed().isEmpty())
        {
            values_sensor = reader.text().toString().split(" ");
        }
        if(reader.isStartElement() && readSensor == true && reader.name() == "sensor")
        {
            numberOfSensors++;
        }
        if(reader.isEndElement() && readSensor == true && reader.name() == "sensor")
        {
            listValues_of_sensors.append(values_sensor);
        }
    }

    if(!listValues_of_sensors.isEmpty())
    {
        double doubleListValues_sensor[listValues_of_sensors.size()]
                [listValues_of_sensors.at(0).size()];

        int k = 0;
        int size_of_data;
        foreach(QStringList values,listValues_of_sensors)
        {
            int i = 0;
            foreach(QString value, values)
            {
                if(!value.trimmed().isEmpty())
                {
                    doubleListValues_sensor[k][i] = value.toDouble();
                    i++;
                }

            }
            size_of_data = i;
            k++;
        }
        int num_rows = listValues_of_sensors.size();
        for (int var = 0; var < num_rows; ++var) {
            QVector<QPointF> _data;
            for(int j = 0; j < size_of_data ; j++)
            {
                const QPointF p((double)j/10,doubleListValues_sensor[var][j]);
                _data.append(p);
            }
            listData.append(_data);
        }

        QStringList s = QStringList();
        for (int var = 1; var <= numberOfSensors; ++var) {
            s << sensor_type + " sensor" + QString::number(var);
        }
//        s  << "UToy sensor1" << "UToy sensor2";
//        qDebug() << listData.at(0).size();

        ui->qwt_graphviewer->initGraph(2, s);
        ui->qwt_graphviewer->setCurveData(listData.at(0), 1);
        ui->qwt_graphviewer->setCurveData(listData.at(1), 2);
    }
    else
        return;
}


CTResultView::~CTResultView()
{
    delete ui;
}
