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
    QDomElement getConfiguration();
    QString getConfiguration(QString str);
    void enableConfig(bool value);

private:
    Ui::CTSimpleBlock *ui;
    int id;
    QDomElement config;
    QString xml_config;
    bool configurable;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void setConfiguration(QDomElement root);
    void setConfiguration(QString xml);
};

#endif // CT_SIMPLEBLOCK_H
