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
    bool setParameters(QDomElement root);
    bool setParameters(QString xml);
    QDomElement getParameters();
    QString getParameters(QString value);

private:
    Ui::CTConfArch *ui;
    QList<CTLight *> light_stimuli;
    double calculateRequiredTime();
    double block_duration;

private slots:
    void updateBlockRuntime(bool value);
    void updateBlockRuntime(double value);
};

#endif // CT_CONFARCH_H

