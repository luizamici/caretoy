#ifndef CT_CONFTOYMICKEY_H
#define CT_CONFTOYMICKEY_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfToyMickey; }

class CTConfToyMickey : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfToyMickey(QWidget *parent = 0);
    ~CTConfToyMickey();
    bool setParameters(QString xml);
    QString getParameters(QString value);

    QString getComment();

private:
    Ui::CTConfToyMickey *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFTOYMICKEY_H
