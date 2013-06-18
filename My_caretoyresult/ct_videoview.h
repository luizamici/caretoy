 #ifndef CT_VIDEOVIEW_H
#define CT_VIDEOVIEW_H

#include <QWidget>
#include <phonon>
#include "ct_mediaobject.h"
#include "ct_seekslider.h"

namespace Ui {
class CTVideoView;
}

class CTVideoView : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTVideoView(QWidget *parent = 0);
    ~CTVideoView();


public slots:
    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void tick_(qint64 time);
    void seek(qint64 time);
    void stopVideo();
    
private:
    Ui::CTVideoView *ui;
    Phonon::VideoWidget *videoWidget;
    Phonon::AudioOutput *audioOutput;

//    CTSeekSlider *seekSlider;
    CTMediaObject *mediaObject;
    bool playingState;
    bool pauseState;
signals:
    void videoPlaying();
    void stopPlaying();
    void pausePlaying();
    void playing(bool b); //signal emitted when the stat changes from any to Playing
};

#endif // CT_VIDEOVIEW_H
