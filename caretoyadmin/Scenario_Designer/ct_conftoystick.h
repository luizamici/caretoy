#ifndef CT_CONFTOYSTICK_H
#define CT_CONFTOYSTICK_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfToyStick; }

class CTConfToyStick : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfToyStick(QWidget *parent = 0);
    ~CTConfToyStick();
    bool setParameters(QString xml);
    QString getParameters(QString value);

private:
    Ui::CTConfToyStick *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli,  speaker_actions;
    double calculateRequiredTime();
    double block_duration;
private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYSTICK_H