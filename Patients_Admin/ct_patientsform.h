#ifndef CT_PATIENTSFORM_H
#define CT_PATIENTSFORM_H

#include <QWidget>
#include <QtGui>
#include "ct_wizardlayout.h"

class MandatoryFieldGroup;

class CTPatientsForm : public QWidget
{
    Q_OBJECT
public:
    explicit CTPatientsForm(QWidget *parent = 0);

    void initializeFormWithPatient(QHash<QString,QString> patient);
    void initializeNewPatient();

    void setRow(QString row);
    void setIdList(QStringList id_list);
    bool unsavedChanges();
    bool updateButtonEnabled();

    bool newPatient;
private:
    MandatoryFieldGroup *group;

    CTWizardLayout *patientsForm;
    QPushButton *updateButton;
    QHash<QString,QString> localPatient;
    QString row;
    QStringList idList;

    QString getRow();
    bool areChanges(QHash<QString,QString> patientFromUI);

    void clearLocalData();


signals:
    void nothingToSave(QString message);
    void insert(QHash<QString,QString> newPatient);
    void update(QHash<QString,QString> patientToUpdate);


public slots:
    void reset();
    void check();
    
};


class MandatoryFieldGroup : public QObject
{
    Q_OBJECT
public:
    MandatoryFieldGroup(QObject *parent)
           : QObject(parent), okButton(0) {}

    void add(QWidget *widget);
    void remove(QWidget *widget);
    void setOkButton(QPushButton *button);

private slots:
    void changed();

private:
    QList<QWidget *> widgets;
    QPushButton *okButton;
};

#endif // CT_PATIENTSFORM_H
