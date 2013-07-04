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

    void initialize(bool newScenario);


    void setInputData(QString description, QString execution_day,
                                QString execution_order, QString image_name, QString position_image);
    void setOutcomeMeasures(QString outcomeM);
    
private slots:
    void accepted();
    void on_qcb_image_currentIndexChanged(const QString &arg1);

private:
    Ui::CTWizard *ui;

    QString getOutcomeMeasures();
    QStringList getInputData();
    void saveData();

signals:
    void save();
};


#endif // CT_WIZARD_H
