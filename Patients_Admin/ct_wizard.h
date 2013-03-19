#ifndef CT_WIZARD_H
#define CT_WIZARD_H

#include <QWizard>
#include "ct_wizardlayout.h"
#include <QtGui>

class CTWizard;

class CTNewPatientWizardPage: public QWizardPage
{
    Q_OBJECT
public:
    CTNewPatientWizardPage(QWidget *parent = 0);

    CTWizardLayout *wl;
};

class CTEditPatientWizardPage: public QWizardPage
{
    Q_OBJECT
public:
    CTEditPatientWizardPage(QWidget *parent = 0);

    CTWizardLayout *wl;

    void initializePageWithData(QHash<QString, QString> patient);
    QString qcb_getCurrentText(int index, QString name);
    QString getRow();
    void setRow(QString savedPatientRow);

    void setPatient( QHash<QString,QString> patient);
    void initializePage();
private:
    QString row;

    QHash<QString,QString> localPatient;
};

class CTWizard : public QWizard
{
    Q_OBJECT
public:
    explicit CTWizard(QWidget *parent = 0);

    void initializeNewPatient();
    void editPatient(QHash<QString, QString> patient);
    void updateRow(QString row);

//    void initializePage(int id);

    QHash<QString,QString> getPatient();
    CTNewPatientWizardPage *newPatientWizard;
    CTEditPatientWizardPage *editPatientWizard;
private:
    QHash<QString, QString> getPatient(int currentId);
    QHash<QString, QString> localPatient;

signals:
    void updateRequested(QHash<QString, QString> patientEdited);
    void changesUnsaved(QHash<QString, QString> patientEdited);
    void patientUnsaved();

public slots:
    void completeChangedNew();
    void completeChangedEdit();
    void onClick(int id);

    void goBack();

};



#endif // CT_WIZARD_H
