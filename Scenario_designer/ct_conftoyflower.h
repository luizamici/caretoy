#ifndef CT_CONFTOYFLOWER_H
#define CT_CONFTOYFLOWER_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfToyFlower; }

class CTConfToyFlower : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfToyFlower(QWidget *parent = 0);
    ~CTConfToyFlower();
    bool setParameters(QDomElement root);
    bool setParameters(QString xml);
    QDomElement getParameters();
    QString getParameters(QString value);

private:
    Ui::CTConfToyFlower *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double calculateRequiredTime();

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYFLOWER_H
