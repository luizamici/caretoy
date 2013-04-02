#ifndef CT_CONFTOYSRING_H
#define CT_CONFTOYSRING_H

#include <QWidget>
#include "ct_customwidgets.h"

namespace Ui {
class CTConfToySRing;
}

class CTConfToySRing : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTConfToySRing(QWidget *parent = 0);
    ~CTConfToySRing();

    bool setParameters(QString xml);
    QString getParameters(QString value);
private:
    Ui::CTConfToySRing *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double block_duration;

    double calculateRequiredTime();
private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYSRING_H
