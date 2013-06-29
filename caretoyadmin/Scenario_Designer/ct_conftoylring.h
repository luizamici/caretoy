#ifndef CT_CONFTOYLRING_H
#define CT_CONFTOYLRING_H

#include <QWidget>
#include "ct_customwidgets.h"

namespace Ui {
class CTConfToyLRing;
}

class CTConfToyLRing : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTConfToyLRing(QWidget *parent = 0);
    ~CTConfToyLRing();

    bool setParameters(QString xml);
    QString getParameters(QString value);

    QString getComment();
    
private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);

private:
    Ui::CTConfToyLRing *ui;
    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    double block_duration;
    double calculateRequiredTime();
};

#endif // CT_CONFTOYLRING_H
