#include "ct_wizard.h"
#include "ui_ct_wizard.h"
#include "ct_scenariodata.h"

CTWizard::CTWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CTWizard)
{
    ui->setupUi(this);
    this->setWindowTitle("Save scenario");
    /*
      Setting mandatory fields for the wizard
     */
    ui->CTWizardPageInput->registerFields("qle_description*", ui->qle_description);
    ui->CTWizardPageInput->registerFields("qde_execution*", ui->qde_execution);
    ui->CTWizardPageInput->registerFields("qsb_execution*", ui->qsb_execution);
    ui->CTWizardPageInput->registerFields("qcb_image",ui->qcb_image);
    ui->CTWizardPageInput->registerFields("qcb_image_position",ui->qcb_image_position);

    connect(this->button(FinishButton), SIGNAL(clicked()),this, SLOT(accepted()));
}

void CTWizard::initialize(bool newScenario)
{
    if(!newScenario)
    {
        ui->qle_description->setText(CTScenarioData::instance().data()->
                                     description);
        QDate date(QDate::fromString(CTScenarioData::instance().data()->
                                     execution_day,"yyyy-MM-dd"));
        ui->qde_execution->setDate(date);
        ui->qsb_execution->setValue(CTScenarioData::instance().data()->
                                    execution_order.toInt());
        ui->qcb_image->setCurrentIndex(ui->qcb_image->findText(
                                           CTScenarioData::instance().data()->
                                           image_description));
        ui->qcb_image_position->setCurrentIndex(ui->qcb_image_position->
                                                findText(CTScenarioData::instance().
                                                         data()->position_image));
        setOutcomeMeasures(CTScenarioData::instance().data()->outcome_measures);
    }
}


CTWizard::~CTWizard()
{
    delete ui;
}

/*!
  If dialog accepted emit signal containing inputData and Outcome measures
  */
void CTWizard::accepted()
{
//    QStringList _out;
    saveData();
//    _out << getInputData() << getOutcomeMeasures();

    emit save();
}

/*!
  Return the outcome measures as an xml string
  */
QString CTWizard::getOutcomeMeasures()
{
    QString outcomeMeasures;
    QXmlStreamWriter stream(&outcomeMeasures);
    stream.writeStartElement("outcome_measures");
    QList<QCheckBox*> outcomeM = this->page(1)->findChildren<QCheckBox*>();
    foreach(QCheckBox* _out,outcomeM)
    {
        stream.writeStartElement("outcome_data");
        stream.writeAttribute("name", _out->objectName().remove(0,5));
        stream.writeAttribute("type", "bool");
        _out->isChecked() ? stream.writeCharacters("true") :
                           stream.writeCharacters("false");
        stream.writeEndElement();//end outcome_data
    }
    stream.writeEndElement();//end outcome_measures
    return outcomeMeasures;
}

/*!
  Return the input data as a QStringList
  */
QStringList CTWizard::getInputData()
{
    QStringList _data;
//    _data << ui->qle_description->text();
//    _data << ui->qcb_image->currentText();
//    _data << ui->qde_execution->date().toString("yyyy-MM-dd");
//    _data << ui->qsb_execution->text();
//    _data << ui->qcb_image_position->currentText();
    return _data;
}


void CTWizard::setInputData(QString description, QString execution_day,
                            QString execution_order, QString image_name,
                            QString position_image)
{
    ui->qle_description->setText(description);
    QDate date(QDate::fromString(execution_day,"yyyy-MM-dd"));
    ui->qde_execution->setDate(date);
    ui->qsb_execution->setValue(execution_order.toInt());
    ui->qcb_image->setCurrentIndex(ui->qcb_image->findText(image_name));
    ui->qcb_image_position->setCurrentIndex(ui->qcb_image_position->findText(position_image));
}

void CTWizard::setOutcomeMeasures(QString outcomeM)
{
    QXmlStreamReader reader(outcomeM);
    QCheckBox* chb = new QCheckBox();
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "outcome_data")
        {
            QXmlStreamAttributes attr = reader.attributes();
            QString checkBoxName = "qchb_" + attr.value("name").toString();
            chb = this->page(1)->findChild<QCheckBox*>(checkBoxName);
        }
        if(reader.isEndElement() && reader.name()== "outcome_data")
            chb  = new QCheckBox();
        if(reader.isCharacters())
        {
            if("true" == reader.text().toString()){ chb->setChecked(true);}
            else if("false" == reader.text()){ chb->setChecked(false);}
        }
    }
}

void CTWizard::on_qcb_image_currentIndexChanged(const QString &arg1)
{
    QImage image = QImage(":/manual/" + arg1);
    if (image.isNull())
    {
        ui->qle_image->clear();
        ui->qle_image->setText("..preview of scenario document here");
    }
    else
    {
        ui->qle_image->setPixmap(QPixmap::fromImage(image));
    }
    return;
}

void CTWizard::saveData()
{
    CTScenarioData::instance().data()->description = ui->qle_description->text();
    CTScenarioData::instance().data()->image_description = ui->qcb_image->currentText();
    CTScenarioData::instance().data()->execution_day = ui->qde_execution->date().
            toString("yyyy-MM-dd");
    CTScenarioData::instance().data()->execution_order = ui->qsb_execution->text();
    CTScenarioData::instance().data()->position_image = ui->qcb_image_position->
            currentText();
    CTScenarioData::instance().data()->outcome_measures = getOutcomeMeasures();
}
