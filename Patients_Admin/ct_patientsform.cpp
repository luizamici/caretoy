#include "ct_patientsform.h"
#include <QLayout>
#include <QtGui>

CTPatientsForm::CTPatientsForm(QWidget *parent) :
    QWidget(parent)
{
    /*
     *Initialize class containing ui
     */
    patientsForm = new CTWizardLayout();
    newPatient = true;

    /*
     *Initialize the local patient data with empty fields
     */
    clearLocalData();

    /**************************************************/
    /*Setting widgets in mainLayout
    /**************************************************/
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(patientsForm);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    updateButton = new QPushButton(tr("Update"));
    QPushButton *undo = new QPushButton(tr("Undo"));
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(updateButton);
    buttonsLayout->addWidget(undo);

    mainLayout->addLayout(buttonsLayout);
    this->setLayout(mainLayout);
    /**************************************************/


    /**************************************************/
    /*Setting madatory fields
    /**************************************************/
    group = new MandatoryFieldGroup(parent);
    foreach(QLineEdit *lineEdit,
            patientsForm->findChildren<QLineEdit*>()){
        if(lineEdit->objectName() == "qleFirstname" || lineEdit->objectName()
                == "qleLastname" || lineEdit->objectName() == "qleId" ||
                lineEdit->objectName() == "qleAttendant" ||
                lineEdit->objectName() == "qleParent_1" ||
                lineEdit->objectName() == "qleParent_2")
        {
            group->add(lineEdit);
        }
    }
    foreach(QComboBox *comboBox, patientsForm->findChildren<QComboBox*>()){
        group->add(comboBox);
    }
    group->add(patientsForm->findChild<QDateEdit*>());
    /***************************************************/

    group->setOkButton(updateButton);
    connect(updateButton,SIGNAL(clicked()),this, SLOT(check()));
    connect(undo,SIGNAL(clicked()),this, SLOT(reset()));

}

/*
 *Initialize form with patient's data
 */
void CTPatientsForm::initializeFormWithPatient(
        QHash<QString,QString> patient)
{
    localPatient = patient;
    qDebug() << "CTPatientsForm::initializeFormWithPatient : " << patient;
    patientsForm->clear();
    patientsForm->setPatient(patient);
    patientsForm->validReferenceEditing(false);
}

/*
 *Initialize empty form for new  patient
 */
void CTPatientsForm::initializeNewPatient()
{
    patientsForm->clear();
    clearLocalData();
    patientsForm->validReferenceEditing(true);
}

/*
 *This is called whenever the 'undo' button is pressed
 */
void CTPatientsForm::reset()
{
    patientsForm->resetPatient(localPatient);
}


/*
 *This is called whenever the 'update' button is pressed
 */
void CTPatientsForm::check()
{
    QHash<QString,QString> patientFromUI = patientsForm->getPatient();

    /*
     *Checking if the inserted reference(id) already exists
     */
    if(idList.contains(patientFromUI["id"].trimmed())){
        updateButton->setEnabled(false);
        QMessageBox::critical(0, tr("Patient Admin"),"Inserted Reference already exists!\n"
                              "Please specify a unique reference number for each patient.");
    }
    else{
        if(newPatient)
        {
            qDebug() << "new patient";
            localPatient = patientFromUI;
            emit insert(localPatient);
            newPatient = false;
            patientsForm->validReferenceEditing(false);
        }
        else
        {
            if(areChanges(patientFromUI))
            {
                qDebug() << "update patient";
                updateLocalData(patientFromUI);
                emit update(localPatient);
            }
            else
            {
                emit nothingToSave("No changes found, data already up to date!");
            }
        }
    }
}


/*
 *Checks for changes inserted to the ui, comparing it to the data
 *used for the population of the page before showing it
 */
bool CTPatientsForm::areChanges(QHash<QString,QString> patientFromUI)
{

    bool changesDetected = false;
    foreach(QString key, localPatient.keys()){
        if(localPatient[key] != patientFromUI[key] && key != "id"){
            qDebug() << localPatient[key] << " " <<  patientFromUI[key];
            changesDetected =  true;
        }
    }
    return changesDetected;
}

void CTPatientsForm::clearLocalData()
{
    localPatient = patientsForm->getPatient();
}

void CTPatientsForm::updateLocalData(QHash<QString, QString> patientFromUI)
{
    foreach(QString key,localPatient.keys())
    {
        if(key != "id")
            localPatient[key] = patientFromUI[key];
    }
}

/*
 *Before escape it is checked for unsaved changes
 */
bool CTPatientsForm::unsavedChanges()
{
    if(areChanges(patientsForm->getPatient()))
        return true;
    else
        return false;
}

bool CTPatientsForm::updateButtonEnabled()
{
    return updateButton->isEnabled();
}


void CTPatientsForm::setIdList(QStringList id_list){
    /*The list of Id is saved in order to double check the inserted Id */
    this->idList = id_list;
    /*It is used as a completer for the Id LineEdit*/
    patientsForm->setCompleter(id_list);
}


/******************************************************************************/
void MandatoryFieldGroup::add(QWidget *widget)
{
    if (!widgets.contains(widget)) {
        if (widget->inherits("QComboBox")) {
            connect(widget, SIGNAL(currentIndexChanged(int)),
                        this, SLOT(changed()));
        } else if (widget->inherits("QLineEdit")) {
            connect(widget,
                        SIGNAL(textChanged(const QString &)),
                        this, SLOT(changed()));
        } else if(widget->inherits("QDateEdit")){
            connect(widget,SIGNAL(dateChanged (QDate)),
                    this, SLOT(changed()));
        } else {
                qWarning("%s unhandled", widget);
                return;
        }
        widget->setProperty("mandatoryField",true);
        widgets.append(widget);
        changed();
     }
}

void MandatoryFieldGroup::setOkButton(QPushButton *button)
{
    if (okButton && okButton != button)
        okButton->setEnabled(true);
    okButton = button;
    changed();
}


void MandatoryFieldGroup::changed()
{
    if (!okButton)
            return;
    bool enable = true;
    QList<QWidget *>::const_iterator i;
    for (i = widgets.begin(); i != widgets.end(); ++i) {
        QWidget *widget = *i;
        if (widget->inherits("QComboBox")) {
            if ((dynamic_cast<QComboBox*>(widget))->currentIndex() == 0) {
                enable = false;
                break;
            }
        }
        if (widget->inherits("QLineEdit")) {
            if ((dynamic_cast<QLineEdit*>(widget))->text().isEmpty()) {
                enable = false;
                break;
            }
        }
        if(widget->inherits("QDateEdit")){
            if((dynamic_cast<QDateEdit*>(widget)->date() == QDate::fromString(
                    "2012-01-01","yyyy-MM-dd"))){
                enable = false;
                break;
            }
        }
    }
    okButton->setEnabled(enable);
}
