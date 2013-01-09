#ifndef CT_CONFWALLRIGHT_H
#define CT_CONFWALLRIGHT_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfWallRight; }

class CTConfWallRight : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfWallRight(QWidget *parent = 0);
    ~CTConfWallRight();
    bool setParameters(QDomElement root);
    bool setParameters(QString xml);
    QDomElement getParameters();
    QString getParameters(QString value);

private:
    Ui::CTConfWallRight *ui;
    QList<CTBigLight *> biglight_stimuli, biglight_actions;
    QList<CTButton *> button_stimuli, button_actions;
    QList<CTLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFWALLRIGHT_H

