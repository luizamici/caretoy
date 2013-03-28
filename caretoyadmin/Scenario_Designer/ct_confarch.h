#ifndef CT_CONFARCH_H
#define CT_CONFARCH_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfArch; }

class CTConfArch : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfArch(QWidget *parent = 0);
    ~CTConfArch();
    bool setParameters(QString xml);
    QString getParameters(QString value);

private:
    Ui::CTConfArch *ui;
    QList<CTLight *> light_stimuli;
    QList<CTSpeaker*> speaker_stimuli, speaker_actions;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFARCH_H

