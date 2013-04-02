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
        if(patient[key] != lineEdit->placeholderText())
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

    foreach(QLineEdit *lineEdit, findChildren<QLineEdit*>()){
        QString key = lineEdit->objectName().remove(0,3).toLower();
        if(!lineEdit->text().trimmed().isEmpty())
            _patient[key] = lineEdit->text();
        else
            _patient[key] = lineEdit->placeholderText();
    }
    _patient["notes"] = ui->qteNotes->toPlainText();
    if(ui->qcbSex->currentIndex() == 1){_patient["sex"] = "M";}
    else if(ui->qcbSex->currentIndex() == 2){_patient["sex"] = "F";}
    _patient["gest_age"] = ui->qcbGest_age->currentText();
    _patient["date_of_birth"] = ui->qdeDate_of_birth->date().toString("yyyy-MM-dd");
    return _patient;
}

void CTWizardLayout::resetPatient(QHash<QString, QString> patient)
{
    foreach(QLineEdit *lineEdit, findChildren<QLineEdit*>()){
        QString key = lineEdit->objectName().remove(0,3).toLower();
        if(patient[key] != lineEdit->placeholderText())
            lineEdit->setText(patient[key]);
        if(patient[key] == lineEdit->placeholderText()
                && lineEdit->text() != lineEdit->placeholderText())
            lineEdit->clear();
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
 *Clears all the input data
 */
void CTWizardLayout::clear(){
        /*
         *Clearing all the edit lines on the form
         *For now the clearing consists in setting the default value
         */
        foreach(QLineEdit *widget, findChildren<QLineEdit*>())
        {
            widget->clear();
        }
        ui->qteNotes->setPlainText("Here notes can be added");

        /*
         *Clearing the comboboxes and date editor
         */
        ui->qcbGest_age->setCurrentIndex(0);
        ui->qcbSex->setCurrentIndex(0);
        ui->qdeDate_of_birth->setDate(QDate::fromString("2012-01-01","yyyy-MM-dd"));
}

/*
 *Enables/disables the reference line editing
 */
void CTWizardLayout::validReferenceEditing(bool b){
    ui->qleRef_key->setEnabled(b);
}


void CTWizardLayout::setCompleter(QStringList id_list){
    qDebug() << "CTWizardLayout::setCompleter";
    QCompleter *completer = new QCompleter(id_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->qleRef_key->setCompleter(completer);
}

CTWizardLayout::~CTWizardLayout()
{
    delete ui;
}
