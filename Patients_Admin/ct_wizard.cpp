#include "ct_wizard.h"
#include <QtGui>

CTWizard::CTWizard(QWidget *parent) :
    QWizard(parent)
{

    QList<QWizard::WizardButton> button_layout;
    button_layout << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2;
    this->setButtonLayout(button_layout);

    setOption(QWizard::HaveCustomButton1,true);
    setButtonText(QWizard::CustomButton1,"Update");
    button(QWizard::CustomButton1)->setEnabled(false);

    setOption(QWizard::HaveCustomButton2,true);
    setButtonText(QWizard::CustomButton2,"Undo");

    newPatientWizard = new CTNewPatientWizardPage();
    editPatientWizard = new CTEditPatientWizardPage();
    setPage(1,newPatientWizard);
    setPage(2,editPatientWizard);

    connect(this,SIGNAL(customButtonClicked(int)), this, SLOT(onClick(int)));
    connect(newPatientWizard, SIGNAL(completeChanged()),this, SLOT(completeChangedNew()));
    connect(editPatientWizard,SIGNAL(completeChanged()),this, SLOT(completeChangedEdit()));
}

/*
 *This shows the first page of the wizard
*/
void CTWizard::initializeNewPatient(){
    restart();
    button(QWizard::CustomButton1)->setEnabled(false);
}

/*
 *This shows the second page of the wizard
 *after updating it
 */
void CTWizard::editPatient(QHash<QString, QString> patient){
    restart();
//    editPatientWizard->initializePageWithData(patient);
    editPatientWizard->setPatient(patient);
    button(QWizard::CustomButton1)->setEnabled(true);
    next();
}

/*
 *In the same way that 'next' and 'finish' buttons of a wizard
 *are enabled and disabled, based on the completeChanged() signal,
 *the same has been implemented for the custom button 1- 'Update'
 */
void CTWizard::completeChangedNew(){

    if(newPatientWizard->isComplete()){
        button(QWizard::CustomButton1)->setEnabled(true);
    }else{
        button(QWizard::CustomButton1)->setEnabled(false);
    }
}
void CTWizard::completeChangedEdit(){

    if(editPatientWizard->isComplete()){
        button(QWizard::CustomButton1)->setEnabled(true);
    }else{
        button(QWizard::CustomButton1)->setEnabled(false);
    }
}


/*
 *This is called each time the customButtonClicked(int) signal is emitted
 */
void CTWizard::onClick(int id){

    QHash<QString,QString> patient;
    /*Update button has been pressed*/
    if(id == 6){
        if(currentId() == 1){
            /*
             *The next page of the wizard needs to be shown with the updated values
             */
            patient = getPatient(currentId());
            editPatientWizard->initializePageWithData(patient);
            editPatientWizard->initializePage();
            next();
        }else if(currentId() == 2){
            patient = getPatient(currentId());
        }
        emit updateRequested(patient);
    }
    /*Undo button has been pressed*/
    else if(id == 7){
        if(currentId() == 1){
            newPatientWizard->cleanupPage();
        }else{
            editPatientWizard->cleanupPage();
        }
    }
}

void CTWizard::goBack(){
    /*Go back button has been pressed*/
    if(currentId() == 1){
        if(!getPatient(currentId()).isEmpty())
            emit patientUnsaved();
    }
    else if(currentId() == 2){
        emit changesUnsaved(getPatient(currentId()));
    }
}

QHash<QString,QString> CTWizard::getPatient(){
     QHash<QString,QString> patient = getPatient(currentId());
     if(currentId() == 1){
         if(patient["date_of_birth"] == "2012-01-01"){
             patient["date_of_birth"] = " ";
         }
         if(patient["gest_age"] == "Choose..."){
             patient["gest_age"] = " ";
         }
         if(patient["sex"] == "Choose..."){
             patient["sex"] = " ";
         }
     }
     return patient;
}



QHash<QString,QString> CTWizard::getPatient(int currentId){
        QHash<QString,QString> _patient;

        switch(currentId){
        case 1:
            _patient["firstname"] = field("qleFirstname").toString();
            _patient["lastname"] = field("qleLastname").toString();
            _patient["parent_1"] = field("qleParent_1").toString();
            _patient["parent_2"] = field("qleParent_2").toString();
            _patient["address"] = field("qleAddress").toString();
            _patient["zip_code"] = field("qleZip_code").toString();
            _patient["phone"] = field("qlePhone").toString();
            _patient["city"] = field("qleCity").toString();
            _patient["email"] = field("qleEmail").toString();
            _patient["id"] = field("qleId").toString();
            _patient["attendant"] = field("qleAttendant").toString();
            _patient["notes"] = field("qteNotes").toString();
            if(editPatientWizard->qcb_getCurrentText(
                        field("qcbSex").toInt(),"qcbSex") == QString("Male")){_patient["sex"] = "M";}
            else if(editPatientWizard->qcb_getCurrentText(
                        field("qcbSex").toInt(),"qcbSex") == QString("Female")){_patient["sex"] = "F";}
            _patient["gest_age"] = editPatientWizard->qcb_getCurrentText(
                        field("qcbGest_age").toInt(),"qcbGest_age");
            _patient["date_of_birth"] = field("qdeDate_of_birth").toDate().toString("yyyy-MM-dd");
            _patient["row"] = " ";
            return _patient;
        case 2:
            _patient["firstname"] = field("Firstname").toString();
            _patient["lastname"] = field("Lastname").toString();
            _patient["parent_1"] = field("Parent_1").toString();
            _patient["parent_2"] = field("Parent_2").toString();
            _patient["address"] = field("Address").toString();
            _patient["zip_code"] = field("Zip_code").toString();
            _patient["phone"] = field("Phone").toString();
            _patient["city"] = field("City").toString();
            _patient["email"] = field("Email").toString();
            _patient["id"] = field("Id").toString();
            _patient["attendant"] = field("Attendant").toString();
            _patient["notes"] = field("Notes").toString();
            if(editPatientWizard->qcb_getCurrentText(
                        field("Sex").toInt(),"qcbSex") == QString("Male")){_patient["sex"] = "M";}
            else if(editPatientWizard->qcb_getCurrentText(
                        field("Sex").toInt(),"qcbSex") == QString("Female")){_patient["sex"] = "F";}
            _patient["gest_age"] = editPatientWizard->qcb_getCurrentText
                    (field("Gest_age").toInt(),"qcbGest_age");
            _patient["date_of_birth"] = field("Date_of_birth").toDate().toString("yyyy-MM-dd");
            _patient["row"] = editPatientWizard->getRow();
            return _patient;
        }
}

void CTWizard::updateRow(QString row){
    qDebug() << "row being updated : " << row;
    editPatientWizard->setRow(row);
}


CTNewPatientWizardPage::CTNewPatientWizardPage(QWidget *parent):
    QWizardPage(parent)
{

    wl = new CTWizardLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(wl);
    setLayout(layout);

    /******************************************************/
    /*Setting madatory fields
    /******************************************************/
    foreach(QLineEdit *lineEdit, wl->findChildren<QLineEdit*>()){
        if(lineEdit->objectName() == "qleFirstname" || lineEdit->objectName() == "qleLastname"
                || lineEdit->objectName() == "qleId" || lineEdit->objectName() == "qleAttendant")
        {

            QString object_name = lineEdit->objectName() + "*";
            registerField(object_name,lineEdit);
        }
    }
    foreach(QComboBox *qcb, wl->findChildren<QComboBox*>()){
        QString object_name = qcb->objectName() + "*";
        registerField(object_name,qcb);
    }

    QDateEdit *qde = wl->findChild<QDateEdit*>();
    QString object_name = qde->objectName() + "*";
    registerField(object_name,qde);
    /********************************************************/

    /*Registering other fields also*/
    registerField("qleParent_1",wl->findChild<QLineEdit*>(tr("qleParent_1")));
    registerField("qleParent_2",wl->findChild<QLineEdit*>(tr("qleParent_2")));
    registerField("qleAddress",wl->findChild<QLineEdit*>(tr("qleAddress")));
    registerField("qleZip_code",wl->findChild<QLineEdit*>(tr("qleZip_code")));
    registerField("qlePhone",wl->findChild<QLineEdit*>(tr("qlePhone")));
    registerField("qleCity",wl->findChild<QLineEdit*>(tr("qleCity")));
    registerField("qleEmail",wl->findChild<QLineEdit*>(tr("qleEmail")));
    registerField("qteNotes",wl->findChild<QTextEdit*>(tr("qteNotes")));
}

CTEditPatientWizardPage::CTEditPatientWizardPage(QWidget *parent):
    QWizardPage(parent)
{

    wl = new CTWizardLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(wl);
    setLayout(layout);

    const QString s("qleId");
    QLineEdit *qleReference = wl->findChild<QLineEdit*>(s);
    qleReference->setEnabled(false);

    /******************************************************/
    /*Setting madatory fields
    /******************************************************/
    foreach(QLineEdit *lineEdit, wl->findChildren<QLineEdit*>()){
        if(lineEdit->objectName() == "qleFirstname" || lineEdit->objectName() == "qleLastname"
                || lineEdit->objectName() == "qleId" || lineEdit->objectName() == "qleAttendant")
        {

            QString object_name = lineEdit->objectName().remove(0,3) + "*";
            registerField(object_name,lineEdit);
        }
    }
    foreach(QComboBox *qcb, wl->findChildren<QComboBox*>()){
        QString object_name = qcb->objectName().remove(0,3) + "*";
        registerField(object_name,qcb,"currentIndex");
    }
    QDateEdit *qde = wl->findChild<QDateEdit*>();
    QString object_name = qde->objectName().remove(0,3) + "*";
    registerField(object_name,qde);
    /********************************************************/

    registerField("Parent_1",wl->findChild<QLineEdit*>(tr("qleParent_1")));
    registerField("Parent_2",wl->findChild<QLineEdit*>(tr("qleParent_2")));
    registerField("Address",wl->findChild<QLineEdit*>(tr("qleAddress")));
    registerField("Zip_code",wl->findChild<QLineEdit*>(tr("qleZip_code")));
    registerField("Phone",wl->findChild<QLineEdit*>(tr("qlePhone")));
    registerField("City",wl->findChild<QLineEdit*>(tr("qleCity")));
    registerField("Email",wl->findChild<QLineEdit*>(tr("qleEmail")));
    registerField("Notes",wl->findChild<QTextEdit*>(tr("qteNotes")),"plainText");

}

void CTEditPatientWizardPage::initializePageWithData(QHash<QString, QString> patient){

    if(patient["row"] != " "){
        row = patient["row"];
    }

/*Doing in this way the fields are set and the page doesn't have to be destroyed
 *and reset again in the wizard, but the cleanupPage() doesn't work correctly.
 */
//    foreach(QString key, patient.keys()){
//        QString fieldName = key;
//        fieldName = key.at(0).toUpper() + fieldName.remove(0,1);
//        if(field(fieldName).typeName() == QString(tr("QString")) ){
//            QString value = patient[key];
//            setField(fieldName,value);
//        }
//        if(fieldName == "Notes"){
//            setField(fieldName,patient[key]);
//        }
//        if(fieldName == "Sex"){
//            if(patient[key] == "M"){
//                setField(fieldName,1);
//            }else if(patient[key] == "F"){
//                setField(fieldName,2);
//            }else{setField(fieldName,0);}
//        }
//        if(fieldName == "Date_of_birth"){
//            setField(fieldName,QDate::fromString(
//                         patient["date_of_birth"], "yyyy-MM-dd"));
//        }
//        if(fieldName == "Gest_age"){
//            QComboBox *qcbGestationalAge = wl->findChild<QComboBox*>(
//                        tr("qcbGest_age"));
//            int index = qcbGestationalAge->findText(patient[key]);
//            setField(fieldName,index);
//        }
//    }
/***********************************************************************************/

    /*Doing also in this way the fields are set and the page doesn't have to be destroyed
     *and reset again in the wizard, but the cleanupPage() doesn't work correctly neither!
     */
        foreach(QLineEdit *lineEdit, wl->findChildren<QLineEdit*>()){
            QString key = lineEdit->objectName().remove(0,3).toLower();
            lineEdit->setText(patient[key]);
        }
        QTextEdit *notes = wl->findChild<QTextEdit*>();
        notes->setPlainText(patient["notes"]);
        QComboBox *qcb_sex = wl->findChild<QComboBox*>("qcbSex");
        if(patient["sex"] == "F"){
            qcb_sex->setCurrentIndex(qcb_sex->findText("Female"));
        }else if(patient["sex"] == "M"){
            qcb_sex->setCurrentIndex(qcb_sex->findText("Male"));
        }
        QComboBox *qcb_gestAge = wl->findChild<QComboBox*>("qcbGest_age");
        qcb_gestAge->setCurrentIndex(qcb_gestAge->findText(patient["gest_age"]));
        QDateEdit *qde_date = wl->findChild<QDateEdit*>();
        qde_date->setDate(QDate::fromString(patient["date_of_birth"],"yyyy-MM-dd"));

}

void CTEditPatientWizardPage::setPatient( QHash<QString,QString> patient){
    localPatient = patient;
}


void CTEditPatientWizardPage::initializePage(){

   if(!localPatient.isEmpty()){
       initializePageWithData(localPatient);
    }else{qDebug() << "***********";}
}

QString CTEditPatientWizardPage::qcb_getCurrentText(int index, QString name){
    QComboBox *qcbName = wl->findChild<QComboBox*>(name);
    return(qcbName->itemText(index));
}

QString CTEditPatientWizardPage::getRow(){
    return row;
}

void CTEditPatientWizardPage::setRow(QString savedPatientRow){
    this->row = savedPatientRow;
}
