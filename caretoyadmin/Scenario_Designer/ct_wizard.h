#ifndef CT_WIZARD_H
#define CT_WIZARD_H

#include <QWizard>
#include <QtGui>
#include <QDebug>
#include <QXmlStreamWriter>


namespace Ui {
class CTWizard;
}

class CTWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit CTWizard(QWidget *parent = 0);
    ~CTWizard();

    void setInputData(QString description, QString execution_day,
                                QString execution_order);
    void setOutcomeMeasures(QString outcomeM);
    
private slots:
    void accepted();

private:
    Ui::CTWizard *ui;

    QString getOutcomeMeasures();
    QStringList getInputData();

signals:
    void accepted(QStringList data);
};


#endif // CT_WIZARD_H
