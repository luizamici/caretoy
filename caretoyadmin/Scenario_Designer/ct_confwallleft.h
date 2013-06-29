#ifndef CT_CONFWALLLEFT_H
#define CT_CONFWALLLEFT_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfWallLeft; }

class CTConfWallLeft : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfWallLeft(QWidget *parent = 0);
    ~CTConfWallLeft();
    bool setParameters(QString xml);
    QString getParameters(QString value);

    QString getComment();

private:
    Ui::CTConfWallLeft *ui;
    QList<CTBigLight *> biglight_stimuli, biglight_actions;
    QList<CTButton *> button_stimuli, button_actions;
    QList<CTLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli , speaker_actions;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFWALLLEFT_H

