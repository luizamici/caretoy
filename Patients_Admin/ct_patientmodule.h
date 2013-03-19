#ifndef CT_PATIENTMODULE_H
#define CT_PATIENTMODULE_H

#include <QWidget>
#include <QtGui>
#include "ct_wizard.h"
#include "ct_tableofscenarios.h"
#include "ct_tableofresults.h"
#include "ct_patientsform.h"

namespace Ui {
class CTPatientModule;
}

class CTPatientModule : public QWidget
{
    Q_OBJECT
public:

    explicit  CTPatientModule(QWidget *parent = 0);
    ~CTPatientModule();

   QMessageBox *warningMessage;

protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::CTPatientModule *ui;


    QStatusBar *statusBar;
    QMessageBox *warningMessage2;
    QMessageBox *confirmationMessage;


signals:
    void updateRequested();
    void undoChanges();

    void beforeGoBack();
    void addButtonClicked();


    void newScenario();

public slots:

    void setPatientWizard(CTPatientsForm *patientForm);

    void setTableOfScenarios(CTTableOfScenarios *tableOfScenarios);
    void setTableOfResults(CTTableOfResults *tableOfResults);

    void showOkMessage(QString message);
    void showWarningMessage(QString message);
    void showWarningMessage1();
    void showErrorMessage(QString errorMessage);

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_copyButton_clicked();
    void on_removeButton_clicked();
};

#endif // CT_PATIENTMODULE_H
