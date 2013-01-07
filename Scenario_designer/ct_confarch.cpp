#include "ct_confarch.h"
#include "ui_ct_confarch.h"

#define NUM_LIGHTS 12

CTConfArch::CTConfArch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTConfArch)
{
    ui->setupUi(this);

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        CTLight *st_light = new CTLight(i+1, false);
        light_stimuli.append(st_light);
        ui->qgb_stimuli->layout()->addWidget(st_light);

        connect(st_light->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_light->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_light->duration_max, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));

        /* Add separating lines */
        if (i+1 != NUM_LIGHTS)
        {
            QFrame *qh_st_line = new QFrame();
            qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_stimuli->layout()->addWidget(qh_st_line);
        }
    }
}

CTConfArch::~CTConfArch()
{
    delete ui;
}


/*!
 * \brief CTConfArch::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML string.
 *
 * \param xml containing data of all configurable parameters.
 */
bool CTConfArch::setParameters(QString xml)
{
    qDebug() << xml;

    int num_stimuli = NUM_LIGHTS;

    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource();
    source->setData(xml);

    CTXmlHandler *handler = new CTXmlHandler;
    /*
     *Passing pointer of the class to the xml parser handler,
     *in order to set the parsed values into it's input fields
     */
    handler->setWidget(9, this, num_stimuli, 0);
    QList<CTConstLight*> empty1;
    QList<CTSpeaker*> empty2;
    handler->setStimuli(empty1, empty2, light_stimuli);

    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);

    bool ok = xmlReader.parse(source);
    qDebug() << "The parsing went ok? " << ok;
    block_duration = handler->getBlockDuration();
    if(ok)
    {
        updateBlockRuntime(1.0);
    }
    return true;
}

/*!
 * \brief CTConfArch::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML tree.
 *
 * \param root XML tree containing data of all configurable parameters.
 */
bool CTConfArch::setParameters(QDomElement root)
{
    /* Check if the supplied configuration is applicable */
    if ("block" != root.tagName() || "arch" != root.attribute("name"))
    {
        return false;
    }

    /* Set block comment */
    QString comment = root.namedItem("comment").toElement().text();
    ui->qte_comment->setPlainText(comment);

    /* Set block runtime */
    QDomElement runtime = root.namedItem("runtime").toElement();
    double duration =
            runtime.namedItem("duration").toElement().text().toDouble();
    int repetitions =
            runtime.namedItem("repetitions").toElement().text().toInt();
    ui->qsb_block_duration->setValue(duration);
    ui->qsb_block_repetitions->setValue(repetitions);

    /* Set block stimuli */
    int num_stimuli = NUM_LIGHTS;
    QDomElement stimuli_block = root.namedItem("stimuli").toElement();
    int stimuli_count = stimuli_block.attribute("number").toInt();
    QDomNodeList stimuli = stimuli_block.childNodes();
    /* Check consistency */
    if (num_stimuli != stimuli_count || num_stimuli != stimuli.size())
    {
        return false;
    }

    for (int i = 0; i < num_stimuli; i++)
    {
        QDomElement stimulus = stimuli.at(i).toElement();
        /* Do not process disabled stimuli */
        if ("false" == stimulus.attribute("enabled")) { continue; }

        light_stimuli[i]->setParameters(stimulus);
    }

    ui->qsb_pause->setValue(duration - calculateRequiredTime());
    return true;
}



/*!
 * \brief CTConfArch::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML string.
 *
 * \return XML containing data of all configurable parameters.
 */
QString CTConfArch::getParameters(QString value){

    value = "This function uses QXmlStreamWriter";

    QString parameters;
    QXmlStreamWriter stream(&parameters);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("block");
    stream.writeAttribute("id", "1");
    stream.writeAttribute("name", "arch");

    /* Insert block comment */
    stream.writeStartElement("comment");
    stream.writeCharacters(ui->qte_comment->toPlainText());
    stream.writeEndElement(); //end comment

    /* Insert block runtime */
    stream.writeStartElement("runtime");
    double duration_calculated = ui->qsb_block_duration->cleanText().toDouble()
            + ui->qsb_pause->cleanText().toDouble();
    stream.writeTextElement("duration",QString::number(duration_calculated));
    stream.writeTextElement("repetitions", ui->qsb_block_repetitions->cleanText());
    stream.writeEndElement(); //end runtime

    /* Insert block stimuli */
    stream.writeStartElement("stimuli");
    int num_stimuli = NUM_LIGHTS;
    stream.writeAttribute("number",QString::number(num_stimuli));
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        light_stimuli.at(i)->getParameters(stream);
    }
    stream.writeEndElement(); // end stimuli
    stream.writeEndElement(); // end block

    return parameters;
}

/*!
 * \brief CTConfArch::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML tree.
 *
 * \return XML tree containing data of all configurable parameters.
 */
//TOBE deprecated
QDomElement CTConfArch::getParameters()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("block");
    root.setAttribute("id", 1);
    root.setAttribute("name", "arch");

    /* Insert block comment */
    QDomElement comment = doc.createElement("comment");
    root.appendChild(comment);
    comment.appendChild(doc.createTextNode(ui->qte_comment->toPlainText()));

    /* Insert block runtime */
    QDomElement runtime = doc.createElement("runtime");
    root.appendChild(runtime);
    QDomElement duration = doc.createElement("duration");
    runtime.appendChild(duration);
    double duration_calculated = ui->qsb_block_duration->cleanText().toDouble()
            + ui->qsb_pause->cleanText().toDouble();
    duration.appendChild(doc.createTextNode(QString::number(duration_calculated)));
    QDomElement repetitions = doc.createElement("repetitions");
    runtime.appendChild(repetitions);
    repetitions.appendChild(doc.createTextNode(
                                ui->qsb_block_repetitions->cleanText()));

    /* Insert block stimuli */
    QDomElement stimuli = doc.createElement("stimuli");
    root.appendChild(stimuli);
    int num_stimuli = NUM_LIGHTS;
    stimuli.setAttribute("number", num_stimuli);

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        QDomElement parameters = light_stimuli.at(i)->getParameters();
        stimuli.appendChild(parameters);
    }

    return root;
}

/*!
 * \brief CTConfArch::calculateRequiredTime
 *
 * Calculates the required block duration as the highest sum of the activation
 * time and the upper bound of the duration interval of all activated stimuli.
 *
 * \return The determined time.
 */
double CTConfArch::calculateRequiredTime()
{
    double time_required = 0.0;

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        CTLight *st_light = light_stimuli.at(i);
        if (st_light->state->isChecked())
        {
            double sum = st_light->activation->value() +
                    st_light->duration_max->value();
            if (time_required < sum) { time_required = sum; }
        }
    }

    return time_required;
}

/*!
 * \brief CTConfArch::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to the activation
 * or deactivation of a stimulus.
 *
 * \param value This parameter is unused.
 */
void CTConfArch::updateBlockRuntime(bool value)
{
    value = false;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
}

/*!
 * \brief CTConfArch::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to a value change
 * in either the activation time control element or the control element for the
 * upper bound of the duration interval.
 *
 * \param value This parameter is unused.
 */
void CTConfArch::updateBlockRuntime(double value)
{
    value = 0;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
    /*Checks if the overall value of the block duration contains the pause*/
    if(calculateRequiredTime() < block_duration)
    {
//        qDebug() << "calculateRequiredTime() < block_duration";
        ui->qsb_pause->setValue(block_duration - calculateRequiredTime());
    }
}
