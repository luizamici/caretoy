#ifndef CT_CONFWALLSCREEN_H
#define CT_CONFWALLSCREEN_H

#include <QtGui>
#include <QtXml>
#include "ct_customwidgets.h"
#include "ct_xmlhandler.h"

namespace Ui { class CTConfWallScreen; }

class CTConfWallScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CTConfWallScreen(QWidget *parent = 0);
    ~CTConfWallScreen();
    bool setParameters(QDomElement root);
    bool setParameters(QString xml);
    QDomElement getParameters();
    QString getParameters(QString value);

private:
    Ui::CTConfWallScreen *ui;
    QList<CTScreen *> screen_stimuli, screen_action;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
    void on_qrb_null_event_toggled(bool checked);
};

#endif // CT_CONFWALLSCREEN_H
