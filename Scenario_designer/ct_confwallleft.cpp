#include "ct_confwallleft.h"
#include "ui_ct_confwallleft.h"

#define NUM_BIGLIGHTS 3
#define NUM_BUTTONS 4
#define NUM_LIGHTS 6
#define NUM_STIMULI_SPEAKERS 2
#define NUM_ACTION_SPEAKERS 1

CTConfWallLeft::CTConfWallLeft(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTConfWallLeft)
{
    ui->setupUi(this);

    /*Initialy disables the tab for the feedback-action*/
    ui->tabFeedbackAction->setDisabled(true);

    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        CTBigLight *st_biglight = new CTBigLight(i+1, false);
        biglight_stimuli.append(st_biglight);
        ui->qgb_stimuli->layout()->addWidget(st_biglight);

        connect(st_biglight->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_biglight->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_biglight->duration_max, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));

        CTBigLight *ac_biglight = new CTBigLight(i+1, true);
        biglight_actions.append(ac_biglight);
        ui->qgb_actions->layout()->addWidget(ac_biglight);

        /* Add separating lines */
        QFrame *qh_st_line = new QFrame();
        qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_stimuli->layout()->addWidget(qh_st_line);

        QFrame *qh_ac_line = new QFrame();
        qh_ac_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_actions->layout()->addWidget(qh_ac_line);
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        CTButton *st_button = new CTButton(i+1, false);
        button_stimuli.append(st_button);
        ui->qgb_stimuli->layout()->addWidget(st_button);

        connect(st_button->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_button->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_button->duration_max, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));

        CTButton *ac_button = new CTButton(i+1, true);
        button_actions.append(ac_button);
        ui->qgb_actions->layout()->addWidget(ac_button);

        /* Add separating lines */
        QFrame *qh_st_line = new QFrame();
        qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_stimuli->layout()->addWidget(qh_st_line);

        QFrame *qh_ac_line = new QFrame();
        qh_ac_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_actions->layout()->addWidget(qh_ac_line);
    }

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

        CTLight *ac_light = new CTLight(i+1, true);
        light_actions.append(ac_light);
        ui->qgb_actions->layout()->addWidget(ac_light);

        /* Add separating lines */
        QFrame *qh_st_line = new QFrame();
        qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_stimuli->layout()->addWidget(qh_st_line);

        QFrame *qh_ac_line = new QFrame();
        qh_ac_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        ui->qgb_actions->layout()->addWidget(qh_ac_line);
    }

    for (int i = 0; i < NUM_STIMULI_SPEAKERS; i++)
    {
        CTSpeaker *st_speaker = new CTSpeaker(i+1, false);
        speaker_stimuli.append(st_speaker);
        ui->qgb_stimuli->layout()->addWidget(st_speaker);

        connect(st_speaker->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_speaker->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_speaker->duration_max,SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));

        /* Add separating lines */
        if (i+1 != NUM_STIMULI_SPEAKERS)
        {
            QFrame *qh_st_line = new QFrame();
            qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_stimuli->layout()->addWidget(qh_st_line);

            QFrame *qh_ac_line = new QFrame();
            qh_ac_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_actions->layout()->addWidget(qh_ac_line);
        }
    }

    CTSpeaker *ac_speaker = new CTSpeaker(1,true);
    speaker_actions.append(ac_speaker);
    ui->qgb_actions->layout()->addWidget(ac_speaker);

    /*
     *Forcing the activation moment of the speakers to be consecutive
     */
    for(int i=0; i < speaker_stimuli.size() -1 ;i++)
    {
        connect(speaker_stimuli.at(i)->activation,SIGNAL(valueChanged(double)),
                speaker_stimuli.at(i + 1)->activation,SLOT(setMinimumValue(double)));
        connect(speaker_stimuli.at(i + 1)->activation,SIGNAL(valueChanged(double)),
                speaker_stimuli.at(i)->activation,SLOT(setMaximumValue(double)));
    }

    /* Establish automatic enabling/disabling of related event controls */
    connect(ui->qrb_position_event, SIGNAL(toggled(bool)),
            ui->qcb_position, SLOT(setEnabled(bool)));
    connect(ui->qrb_button_pressed_event,SIGNAL(toggled(bool)),
            ui->qcb_button,SLOT(setEnabled(bool)));
    connect(ui->qrb_body_event,SIGNAL(toggled(bool)),
            ui->qcb_body,SLOT(setEnabled(bool)));
    connect(ui->qrb_head_event,SIGNAL(toggled(bool)),
            ui->qcb_head,SLOT(setEnabled(bool)));

    connect(ui->qsb_duration_min,SIGNAL(valueChanged(double)),
            ui->qsb_duration_max,SLOT(setMinimumValue(double)));
    connect(ui->qsb_duration_max,SIGNAL(valueChanged(double)),
            ui->qsb_duration_min,SLOT(setMaximumValue(double)));


    /* Fill feedback control elements with content */
    ui->qcb_position->addItems(QStringList() << "prone" << "supine" << "left side" << "right side");
}

CTConfWallLeft::~CTConfWallLeft()
{
    delete ui;
}


/*!
 * \brief CTConfToyFlower::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML string.
 *
 * \param xml containing data of all configurable parameters.
 */
bool CTConfWallLeft::setParameters(QString xml)
{
//    qDebug() << xml;

    int num_stimuli = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS +
            NUM_STIMULI_SPEAKERS;
    int num_actions = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS +
            NUM_ACTION_SPEAKERS;

    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource();
    source->setData(xml);

    CTXmlHandler *handler = new CTXmlHandler;
    /*
     *Passing pointer of the class to the xml parser handler,
     *in order to set the parsed values into it's input fields
     */
    handler->setWidget(6, this, num_stimuli, num_actions);
    QList<CTConstLight*> empty1;
    QList<CTScreen*> empty2;
    handler->setStimuli(empty1, speaker_stimuli,light_stimuli,empty2,biglight_stimuli,button_stimuli);
    handler->setActions(empty1, speaker_actions,empty2,biglight_actions,button_actions,light_actions);

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
 * \brief CTConfWallLeft::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML tree.
 *
 * \param root XML tree containing data of all configurable parameters.
 */
//TOBE deprecated
bool CTConfWallLeft::setParameters(QDomElement root)
{
    /* Check if the supplied configuration is applicable */
    if ("block" != root.tagName() || "wall_left" != root.attribute("name"))
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
    int num_stimuli = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_STIMULI_SPEAKERS;
    QDomElement stimuli_block = root.namedItem("stimuli").toElement();
    int stimuli_count = stimuli_block.attribute("number").toInt();
    QDomNodeList stimuli = stimuli_block.childNodes();
    /* Check consistency */
    if (num_stimuli != stimuli_count || num_stimuli != stimuli.size())
    {
        qDebug() << "num_stimuli != stimuli_count || num_stimuli != stimuli.size() : " << num_stimuli << stimuli_count << stimuli.size();
        return false;
    }

    for (int i = 0; i < num_stimuli; i++)
    {
        QDomElement stimulus = stimuli.at(i).toElement();
        /* Do not process disabled stimuli */
        if ("false" == stimulus.attribute("enabled")) { continue; }

        if (i < NUM_BIGLIGHTS)
        {
            biglight_stimuli[i]->setParameters(stimulus);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS))
        {
            int j = i - NUM_BIGLIGHTS;
            button_stimuli[j]->setParameters(stimulus);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS))
        {
            int j = i - (NUM_BIGLIGHTS + NUM_BUTTONS);
            light_stimuli[j]->setParameters(stimulus);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_STIMULI_SPEAKERS))
        {
            int j = i - (NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS);
            speaker_stimuli[j]->setParameters(stimulus);
        }
    }

    /* Set block feedback events */
    QDomElement feedback_block = root.namedItem("feedback").toElement();

    QDomElement event = feedback_block.namedItem("event").toElement();
    QString event_name = event.attribute("name");
    if ("null" == event_name)
    {
        ui->qrb_null_event->setChecked(true);
    }
    else if ("position" == event_name)
    {
        ui->qrb_position_event->setChecked(true);
        QString value = event.namedItem("condition").toElement().text();
        ui->qcb_position->setCurrentIndex(ui->qcb_position->findText(value));
    }
    else if("button" == event_name)
    {
        ui->qrb_button_pressed_event->setChecked(true);
        QString value = event.attribute("sensor");
        ui->qcb_button->setCurrentIndex(ui->qcb_button->findText(value));
    }
    else if ("body" == event_name)
    {
        ui->qrb_body_event->setChecked(true);
        QString value = event.namedItem("condition").toElement().text();
        ui->qcb_body->setCurrentIndex(ui->qcb_body->findText(value));
    }
    else if ("head" == event_name)
    {
        ui->qrb_head_event->setChecked(true);
        QString value = event.namedItem("condition").toElement().text();
        ui->qcb_head->setCurrentIndex(ui->qcb_head->findText(value));
    }

    /* Set block feedback actions */
    int num_actions = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_ACTION_SPEAKERS;
    QDomElement actions_block = feedback_block.namedItem("actions").toElement();
    int actions_count = actions_block.attribute("number").toInt();
    QDomNodeList actions = actions_block.childNodes();
    /* Check consistency */
    if (num_actions != actions_count || num_actions != actions.size())
    {
        return false;
    }

    for (int i = 0; i < num_actions; i++)
    {
        QDomElement action = actions.at(i).toElement();
        /* Do not process disabled actions */
        if ("false" == action.attribute("enabled")) { continue; }

        if (i < NUM_BIGLIGHTS)
        {
            biglight_actions[i]->setParameters(action);
            QDomElement xml_duration = action.namedItem("duration").toElement();
            double val_duration_min =
                    xml_duration.namedItem("from").toElement().text().toDouble();
            double val_duration_max =
                    xml_duration.namedItem("to").toElement().text().toDouble();
            ui->qsb_duration_min->setMinimum(val_duration_min);
            ui->qsb_duration_min->setValue(val_duration_min);
            ui->qsb_duration_max->setValue(val_duration_max);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS))
        {
            int j = i - NUM_BIGLIGHTS;
            button_actions[j]->setParameters(action);
            QDomElement xml_duration = action.namedItem("duration").toElement();
            double val_duration_min =
                    xml_duration.namedItem("from").toElement().text().toDouble();
            double val_duration_max =
                    xml_duration.namedItem("to").toElement().text().toDouble();
            ui->qsb_duration_min->setMinimum(val_duration_min);
            ui->qsb_duration_min->setValue(val_duration_min);
            ui->qsb_duration_max->setValue(val_duration_max);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS))
        {
            int j = i - (NUM_BIGLIGHTS + NUM_BUTTONS);
            light_actions[j]->setParameters(action);
            QDomElement xml_duration = action.namedItem("duration").toElement();
            double val_duration_min =
                    xml_duration.namedItem("from").toElement().text().toDouble();
            double val_duration_max =
                    xml_duration.namedItem("to").toElement().text().toDouble();
            ui->qsb_duration_min->setMinimum(val_duration_min);
            ui->qsb_duration_min->setValue(val_duration_min);
            ui->qsb_duration_max->setValue(val_duration_max);
        }
        else if (i < (NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_ACTION_SPEAKERS))
        {
            speaker_actions[0]->setParameters(action);
            QDomElement xml_duration = action.namedItem("duration").toElement();
            double val_duration_min =
                    xml_duration.namedItem("from").toElement().text().toDouble();
            double val_duration_max =
                    xml_duration.namedItem("to").toElement().text().toDouble();
            ui->qsb_duration_min->setMinimum(val_duration_min);
            ui->qsb_duration_min->setValue(val_duration_min);
            ui->qsb_duration_max->setValue(val_duration_max);
        }
    }
    ui->qsb_pause->setValue(duration - calculateRequiredTime());
    return true;
}



/*!
 * \brief CTConfWallLeft::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML string.
 *
 * \return XML containing data of all configurable parameters.
 */
QString CTConfWallLeft::getParameters(QString value){

    value = "This function uses QXmlStreamWriter";

    QString parameters;
    QXmlStreamWriter stream(&parameters);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(4);

    stream.writeStartElement("block");
    stream.writeAttribute("id", "1");
    stream.writeAttribute("name", "wall_left");

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
    int num_stimuli = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS +
            NUM_STIMULI_SPEAKERS;
    stream.writeAttribute("number",QString::number(num_stimuli));


    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        biglight_stimuli.at(i)->getParameters(stream);
    }
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        button_stimuli.at(i)->getParameters(stream);
    }
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        light_stimuli.at(i)->getParameters(stream);
    }
    for (int i = 0; i < NUM_STIMULI_SPEAKERS; i++)
    {
        speaker_stimuli.at(i)->getParameters(stream);
    }

    stream.writeEndElement(); // end stimuli

    /* Insert block feedback events */
    stream.writeStartElement("feedback");
    stream.writeStartElement("event");
    if (ui->qrb_null_event->isChecked())
    {
        stream.writeAttribute("id", "0");
        stream.writeAttribute("name", "none");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","none");
        stream.writeCharacters("");
        stream.writeEndElement();//end condition
    }
    else if (ui->qrb_position_event->isChecked())
    {
        stream.writeAttribute("id", "2");
        stream.writeAttribute("name", "position");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_position->currentText());
        stream.writeEndElement();//end condition
    }
    else if (ui->qrb_body_event->isChecked())
    {
        stream.writeAttribute("id", "3");
        stream.writeAttribute("name", "body");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_body->currentText());
        stream.writeEndElement();//end condition
    }
    else if (ui->qrb_head_event->isChecked())
    {
        stream.writeAttribute("id", "4");
        stream.writeAttribute("name", "head");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_head->currentText());
        stream.writeEndElement();//end condition
    }
    else if(ui->qrb_button_pressed_event->isChecked())
    {
        stream.writeAttribute("id","5");
        stream.writeAttribute("name", "button");
        stream.writeAttribute("sensor",ui->qcb_button->currentText());
        stream.writeStartElement("condition");
        stream.writeAttribute("type", "numerical");
        stream.writeCharacters("1");
        stream.writeEndElement();//end condition
     }

    stream.writeEndElement(); //end event

    /* Insert block feedback actions */
    stream.writeStartElement("actions");
    int num_actions = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_ACTION_SPEAKERS;
    stream.writeAttribute("number",QString::number(num_actions));


    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        biglight_actions.at(i)->getParameters(stream);

        /*
         *The content of the spin boxes of the min and max duration
         *are stored into each action tag
         */
        stream.writeStartElement("duration");
        stream.writeTextElement("from",ui->qsb_duration_min->cleanText());
        stream.writeTextElement("to", ui->qsb_duration_max->cleanText());
        stream.writeEndElement();//end duration

        stream.writeEndElement(); //end action or stimulus
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        button_actions.at(i)->getParameters(stream);

        /*
         *The content of the spin boxes of the min and max duration
         *are stored into each action tag
         */
        stream.writeStartElement("duration");
        stream.writeTextElement("from",ui->qsb_duration_min->cleanText());
        stream.writeTextElement("to", ui->qsb_duration_max->cleanText());
        stream.writeEndElement();//end duration

        stream.writeEndElement(); //end action or stimulus
    }

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        light_actions.at(i)->getParameters(stream);

        /*
         *The content of the spin boxes of the min and max duration
         *are stored into each action tag
         */
        stream.writeStartElement("duration");
        stream.writeTextElement("from",ui->qsb_duration_min->cleanText());
        stream.writeTextElement("to", ui->qsb_duration_max->cleanText());
        stream.writeEndElement();//end duration

        stream.writeEndElement(); //end action or stimulus
    }

    speaker_actions[0]->getParameters(stream);

    stream.writeStartElement("duration");
    stream.writeTextElement("from",ui->qsb_duration_min->cleanText());
    stream.writeTextElement("to", ui->qsb_duration_max->cleanText());
    stream.writeEndElement();//end duration

    stream.writeEndElement(); //end action or stimulus
    stream.writeEndElement(); //end actions
    stream.writeEndElement(); //end feedback
    stream.writeEndElement(); // end block

    return parameters;
}

/*!
 * \brief CTConfWallLeft::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML tree.
 *
 * \return XML tree containing data of all configurable parameters.
 */
QDomElement CTConfWallLeft::getParameters()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("block");
    root.setAttribute("id", 1);
    root.setAttribute("name", "wall_left");

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
    int num_stimuli = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_STIMULI_SPEAKERS;
    stimuli.setAttribute("number", num_stimuli);

    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        QDomElement parameters = biglight_stimuli.at(i)->getParameters();
        stimuli.appendChild(parameters);
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        QDomElement parameters = button_stimuli.at(i)->getParameters();
        stimuli.appendChild(parameters);
    }

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        QDomElement parameters = light_stimuli.at(i)->getParameters();
        stimuli.appendChild(parameters);
    }

    for (int i = 0; i < NUM_STIMULI_SPEAKERS; i++)
    {
        QDomElement parameters = speaker_stimuli.at(i)->getParameters();
        stimuli.appendChild(parameters);
    }

    /* Insert block feedback events */
    QDomElement feedback = doc.createElement("feedback");
    root.appendChild(feedback);

    QDomElement event = doc.createElement("event");
    feedback.appendChild(event);
    if (ui->qrb_null_event->isChecked())
    {
        event.setAttribute("id", 0);
        event.setAttribute("name", "none");
        QDomElement condition = doc.createElement("condition");
        event.appendChild(condition);
        condition.setAttribute("type", "none");
        condition.appendChild(doc.createTextNode("null"));
    }
    else if (ui->qrb_position_event->isChecked())
    {
        event.setAttribute("id", 3);
        event.setAttribute("name", "position");
        QDomElement condition = doc.createElement("condition");
        event.appendChild(condition);
        condition.setAttribute("type", "textual");
        condition.appendChild(
                    doc.createTextNode(ui->qcb_position->currentText()));
    }
    else if (ui->qrb_body_event->isChecked())
    {
        event.setAttribute("id", 4);
        event.setAttribute("name", "body");
        QDomElement condition = doc.createElement("condition");
        event.appendChild(condition);
        condition.setAttribute("type", "textual");
        condition.appendChild(
                    doc.createTextNode(ui->qcb_body->currentText()));
    }
    else if (ui->qrb_head_event->isChecked())
    {
        event.setAttribute("id", 5);
        event.setAttribute("name", "head");
        QDomElement condition = doc.createElement("condition");
        event.appendChild(condition);
        condition.setAttribute("type", "textual");
        condition.appendChild(
                    doc.createTextNode(ui->qcb_head->currentText()));
    }
    else if(ui->qrb_button_pressed_event->isChecked())
    {
        event.setAttribute("id", 6);
        event.setAttribute("name","button");
        event.setAttribute("sensor",ui->qcb_button->currentText());
        QDomElement condition = doc.createElement("condition");
        event.appendChild(condition);
        condition.setAttribute("type","numerical");
        condition.appendChild(doc.createTextNode(QString::number(1)));
    }

    /* Insert block feedback actions */
    QDomElement actions = doc.createElement("actions");
    feedback.appendChild(actions);
    int num_actions = NUM_BIGLIGHTS + NUM_BUTTONS + NUM_LIGHTS + NUM_ACTION_SPEAKERS;
    actions.setAttribute("number", num_actions);

    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        QDomElement parameters = biglight_actions.at(i)->getParameters();

        QDomDocument doc;
        QDomElement xml_duration = doc.createElement("duration");
        parameters.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(ui->qsb_duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(ui->qsb_duration_max->cleanText()));

        actions.appendChild(parameters);
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        QDomElement parameters = button_actions.at(i)->getParameters();

        QDomDocument doc;
        QDomElement xml_duration = doc.createElement("duration");
        parameters.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(ui->qsb_duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(ui->qsb_duration_max->cleanText()));

        actions.appendChild(parameters);
    }

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        QDomElement parameters = light_actions.at(i)->getParameters();

        QDomDocument doc;
        QDomElement xml_duration = doc.createElement("duration");
        parameters.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(ui->qsb_duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(ui->qsb_duration_max->cleanText()));

        actions.appendChild(parameters);
    }

        QDomElement parameters = speaker_actions[0]->getParameters();

        QDomDocument doc2;
        QDomElement xml_duration = doc2.createElement("duration");
        parameters.appendChild(xml_duration);
        QDomElement xml_duration_min = doc2.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc2.createTextNode(ui->qsb_duration_min->cleanText()));
        QDomElement xml_duration_max = doc2.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc2.createTextNode(ui->qsb_duration_max->cleanText()));

        actions.appendChild(parameters);

    return root;
}

/*!
 * \brief CTConfWallLeft::calculateRequiredTime
 *
 * Calculates the required block duration as the highest sum of the activation
 * time and the upper bound of the duration interval of all activated stimuli.
 *
 * \return The determined time.
 */
double CTConfWallLeft::calculateRequiredTime()
{
    double time_required = 0.0;

    for (int i = 0; i < NUM_BIGLIGHTS; i++)
    {
        CTBigLight *st_biglight = biglight_stimuli.at(i);
        if (st_biglight->state->isChecked())
        {
            double sum = st_biglight->activation->value() +
                    st_biglight->duration_max->value();
            if (time_required < sum) { time_required = sum; }
        }
    }

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        CTButton *st_button = button_stimuli.at(i);
        if (st_button->state->isChecked())
        {
            double sum = st_button->activation->value() +
                    st_button->duration_max->value();
            if (time_required < sum) { time_required = sum; }
        }
    }

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

    for (int i = 0; i < NUM_STIMULI_SPEAKERS; i++)
    {
        CTSpeaker *st_speaker = speaker_stimuli.at(i);
        if (st_speaker->state->isChecked())
        {
            double sum = st_speaker->activation->value() +
                    st_speaker->duration_max->value();
            if (time_required < sum){time_required = sum;}
        }
    }
    return time_required;
}

/*!
 * \brief CTConfWallLeft::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to the activation
 * or deactivation of a stimulus.
 *
 * \param value This parameter is unused.
 */
void CTConfWallLeft::updateBlockRuntime(bool value)
{
    value = false;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
}

/*!
 * \brief CTConfWallLeft::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to a value change
 * in either the activation time control element or the control element for the
 * upper bound of the duration interval.
 *
 * \param value This parameter is unused.
 */
void CTConfWallLeft::updateBlockRuntime(double value)
{
    value = 0;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
    /*Checks if the overall value of the block duration contains the pause*/
    if(calculateRequiredTime() < block_duration)
    {
        ui->qsb_pause->setValue(block_duration - calculateRequiredTime());
    }
}

void CTConfWallLeft::on_qrb_null_event_toggled(bool checked)
{
    ui->tabFeedbackAction->setDisabled(checked);
}
