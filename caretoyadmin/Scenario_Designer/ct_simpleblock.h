#ifndef CT_SIMPLEBLOCK_H
#define CT_SIMPLEBLOCK_H

#include <QtGui>
#include <QtXml>
#include "ct_blockconfig.h"

namespace Ui { class CTSimpleBlock; }

class CTSimpleBlock : public QFrame
{
    Q_OBJECT

public:
    explicit CTSimpleBlock(int id, QWidget *parent = 0);
    ~CTSimpleBlock();
    QString getName();
    QPixmap getImage();
    QString getConfiguration(QString str);
    double getRuntime();
    void enableConfig(bool value);

    void setToolTip(const QString &toolTip);

private:
    Ui::CTSimpleBlock *ui;
    int id;
    QDomElement config;
    QString xml_config;
    bool configurable;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void setConfiguration(QString xml);
    void setConfigurations(QString xml, QString comment);

signals:
    void updateRuntime(QString xml_description);
};

#endif // CT_SIMPLEBLOCK_H
