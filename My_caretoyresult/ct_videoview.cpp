#include "ct_videoview.h"
#include "ui_ct_videoview.h"
#include <QtGui>
#include <phonon>

CTVideoView::CTVideoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTVideoView)
{
    ui->setupUi(this);

    //For now I am playing an mp4 video file format. In this case the control of the existance of the MIME is done
    //for the .mp4
    mediaObject = new CTMediaObject();

    mediaObject->setTickInterval(100);

    QString filename("./Source/2013-01-31_11-02-41.mp4");
    if(!Phonon::BackendCapabilities::isMimeTypeAvailable("video/mpeg")){
        QMessageBox::warning(this, tr("There was an Error"),tr("A plugin is needed for playing the video!"));
    }else{mediaObject->setCurrentSource(filename);}

    //TODO get the precise dimension of the video, for now it fits to the test video
    videoWidget = new Phonon::VideoWidget(this);
//    videoWidget->setFixedSize(320,240);
    videoWidget->setMinimumSize(320,240);
    ui->verticalLayout_3->addWidget(videoWidget,0,Qt::AlignCenter);

    seekSlider = new CTSeekSlider(mediaObject,this->parentWidget());
    ui->verticalLayout_3->addWidget(seekSlider);

    ui->lcdNumber->display("00:00");

    Phonon::createPath(mediaObject,videoWidget);

    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(mediaObject,audioOutput);

    ui->volumeSlider->setAudioOutput(audioOutput);

    //the play and immediately pause thing helps in getting a first screenshot
    //and making the mediaObject seekable, since it cannot be so in StoppedState
    mediaObject->play();
    mediaObject->pause();

//    QStringList availableMimeTypes = Phonon::BackendCapabilities::availableMimeTypes();
//    foreach (QString availableMimeType,availableMimeTypes){
//    qDebug() << "The available mimeTypes are :" << availableMimeType;}


    //connection between the GUI commands and the video control sequences
    connect(ui->playButton,SIGNAL(clicked()),this, SIGNAL(videoPlaying()));
    connect(ui->playButton,SIGNAL(clicked()),mediaObject,SLOT(play()));
    connect(ui->pauseButton,SIGNAL(clicked()),mediaObject,SLOT(pause()));
    connect(ui->stopButton,SIGNAL(clicked()),this, SLOT(stopVideo()));

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick_(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),this, SLOT(stateChanged(Phonon::State, Phonon::State)));
    connect(ui->pauseButton,SIGNAL(clicked()),this,SIGNAL(pausePlaying()));
    connect(ui->stopButton,SIGNAL(clicked()),this, SIGNAL(stopPlaying()));
}

void CTVideoView::stopVideo(){
    mediaObject->stop();
    mediaObject->play();
    mediaObject->pause();
}

void CTVideoView::seek(qint64 time){

    //Wired with the clicking function on the plot widget.
    mediaObject->update(time);
}

void CTVideoView::tick_(qint64 time){

//    qDebug() << "CTVideoView::tick() time=" << time;
//    qDebug() << "CTVideoView::tick() totalTime=" << mediaObject->totalTime();

    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    ui->lcdNumber->display(displayTime.toString("mm:ss"));
    if(playingState && !pauseState){emit videoPlaying();}
}

void CTVideoView::stateChanged(Phonon::State newState, Phonon::State oldState) {

    qDebug() << "CTVideoView::stateChanged()";
    switch(newState) {

    case Phonon::ErrorState:
        if (mediaObject->errorType() == Phonon::FatalError) {
            QMessageBox::warning(this, tr("Fatal Error"),mediaObject->errorString());
        } else {
            QMessageBox::warning(this, tr("Error"),mediaObject->errorString());
        }
        break;

    case Phonon::PlayingState:
        qDebug() << "PlayingState newState=" << newState << "oldState=" << oldState;
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        playingState = true;
        pauseState = false;
        emit playing(true);
        break;

    case Phonon::StoppedState:
        qDebug() << "StoppedState newState=" << newState << "oldState=" << oldState;
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->lcdNumber->display("00:00");
        playingState = false;
        pauseState = false;
        emit stopPlaying();
        stopVideo();
        break;

    case Phonon::PausedState:
        qDebug() << "PausedState newState=" << newState << "oldState=" << oldState;
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        playingState = false;
        pauseState = true;
        break;

    case Phonon::LoadingState:
        qDebug() << "LoadingState newState=" << newState << "oldState=" << oldState;
        playingState = false;
        pauseState = false;
        break;

    case Phonon::BufferingState:

        qDebug() << "BufferingState newState=" << newState << "oldState=" << oldState;
        playingState = false;
        pauseState = false;
        break;

    default:
        qDebug() << "State? newState=" << newState << "oldState=" << oldState;

    }
}

CTVideoView::~CTVideoView()
{
    delete ui;
}

