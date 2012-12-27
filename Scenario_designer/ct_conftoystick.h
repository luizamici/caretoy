#ifndef CT_CONFTOYSTICK_H
#define CT_CONFTOYSTICK_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"

namespace Ui { class CTConfToyStick; }

class CTConfToyStick : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfToyStick(QWidget *parent = 0);
    ~CTConfToyStick();
    bool setParameters(QDomElement root);
    QDomElement getParameters();

private:
    Ui::CTConfToyStick *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli,  speaker_actions;
    double calculateRequiredTime();

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYSTICK_H
