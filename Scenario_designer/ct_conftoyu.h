#ifndef CT_CONFTOYU_H
#define CT_CONFTOYU_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"

namespace Ui { class CTConfToyU; }

class CTConfToyU : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfToyU(QWidget *parent = 0);
    ~CTConfToyU();
    bool setParameters(QDomElement root);
    QDomElement getParameters();

private:
    Ui::CTConfToyU *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double calculateRequiredTime();

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYU_H
