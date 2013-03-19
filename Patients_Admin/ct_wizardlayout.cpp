#include "ct_wizardlayout.h"
#include "ui_ct_wizardlayout.h"
#include <QDebug>

CTWizardLayout::CTWizardLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTWizardLayout)
{
    ui->setupUi(this);
}


/*
 *Sets the patients's data into the ui
 */
void CTWizardLayout::setPatient(QHash<QString, QString> patient){

    foreach(QLineEdit *lineEdit, findChildren<QLineEdit*>()){
        QString key = lineEdit->objectName().remove(0,3).toLower();
        lineEdit->setText(patient[key]);
    }
    QTextEdit *notes = findChild<QTextEdit*>();
    notes->setPlainText(patient["notes"]);
    QComboBox *qcb_sex = findChild<QComboBox*>("qcbSex");
    if(patient["sex"] == "F"){
        qcb_sex->setCurrentIndex(qcb_sex->findText("Female"));
    }else if(patient["sex"] == "M"){
        qcb_sex->setCurrentIndex(qcb_sex->findText("Male"));
    }
    QComboBox *qcb_gestAge = findChild<QComboBox*>("qcbGest_age");
    qcb_gestAge->setCurrentIndex(qcb_gestAge->findText(patient["gest_age"]));
    QDateEdit *qde_date = findChild<QDateEdit*>();
    qde_date->setDate(QDate::fromString(patient["date_of_birth"],"yyyy-MM-dd"));
}

/*
 *Gets the patient's data from the ui
 */
QHash<QString,QString> CTWizardLayout::getPatient(){
    QHash<QString,QString> _patient;
    _patient["firstname"] = ui->qleFirstname->text();
    _patient["lastname"] = ui->qleLastname->text();
    _patient["parent_1"] = ui->qleParent_1->text();
    _patient["parent_2"] = ui->qleParent_2->text();
    _patient["address"] = ui->qleAddress->text();
    _patient["zip_code"] = ui->qleZip_code->text();
    _patient["phone"] = ui->qlePhone->text();
    _patient["city"] = ui->qleCity->text();
    _patient["email"] = ui->qleEmail->text();
    _patient["id"] = ui->qleId->text();
    _patient["attendant"] = ui->qleAttendant->text();
    _patient["notes"] = ui->qteNotes->toPlainText();
    if(ui->qcbSex->currentIndex() == 1){_patient["sex"] = "M";}
    else if(ui->qcbSex->currentIndex() == 2){_patient["sex"] = "F";}
    _patient["gest_age"] = ui->qcbGest_age->currentText();
    _patient["date_of_birth"] = ui->qdeDate_of_birth->date().toString("yyyy-MM-dd");
    return _patient;
}

/*
 *Clears all the input data
 */
void CTWizardLayout::clear(){
        /*
         *Clearing all the edit lines on the form
         */
        foreach(QLineEdit *widget, findChildren<QLineEdit*>()) {
            widget->clear();
        }
        /*
         *Clearing the notes editor, the comboboxes and date editor
         */
        ui->qteNotes->clear();
        ui->qcbGest_age->setCurrentIndex(0);
        ui->qcbSex->setCurrentIndex(0);
        ui->qdeDate_of_birth->setDate(QDate::fromString("2012-01-01","yyyy-MM-dd"));
}

/*
 *Enables/disables the reference line editing
 */
void CTWizardLayout::validReferenceEditing(bool b){
    ui->qleId->setEnabled(b);
}


void CTWizardLayout::setCompleter(QStringList id_list){
    qDebug() << "CTWizardLayout::setCompleter";
    QCompleter *completer = new QCompleter(id_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->qleId->setCompleter(completer);
}

CTWizardLayout::~CTWizardLayout()
{
    delete ui;
}
