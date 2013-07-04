#include "ct_conftoylring.h"
#include "ui_ct_conftoylring.h"
#include "ct_xmlhandler.h"

#define NUM_LIGHTS 1
#define NUM_SPEAKERS 1

CTConfToyLRing::CTConfToyLRing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTConfToyLRing)
{
    ui->setupUi(this);
    ui->tabFeedbackAction->setDisabled(true);
    block_duration = (double) 0.0;

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        CTConstLight *st_light = new CTConstLight(i+1, false);
        light_stimuli.append(st_light);
        ui->qgb_stimuli->layout()->addWidget(st_light);

        connect(st_light->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_light->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_light->duration_max, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));

        CTConstLight *ac_light = new CTConstLight(i+1, true);
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

    for (int i = 0; i < NUM_SPEAKERS; i++)
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


        CTSpeaker *ac_speaker = new CTSpeaker(i+1, true);
        speaker_actions.append(ac_speaker);
        ui->qgb_actions->layout()->addWidget(ac_speaker);

        /* Add separating lines */
        if (i+1 != NUM_SPEAKERS)
        {
            QFrame *qh_st_line = new QFrame();
            qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_stimuli->layout()->addWidget(qh_st_line);

            QFrame *qh_ac_line = new QFrame();
            qh_ac_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_actions->layout()->addWidget(qh_ac_line);
        }
    }

    /* Establish automatic enabling/disabling of related event controls */
    connect(ui->qrb_pressure_event, SIGNAL(toggled(bool)),
            ui->qsb_pressure, SLOT(setEnabled(bool)));
    connect(ui->qrb_position_event, SIGNAL(toggled(bool)),
            ui->qcb_position, SLOT(setEnabled(bool)));
    connect(ui->qrb_body_event,SIGNAL(toggled(bool)),
            ui->qcb_body,SLOT(setEnabled(bool)));
    connect(ui->qrb_head_event,SIGNAL(toggled(bool)),
            ui->qcb_head,SLOT(setEnabled(bool)));

    connect(ui->qsb_duration_min,SIGNAL(valueChanged(double)),
            ui->qsb_duration_max,SLOT(setMinimumValue(double)));
    connect(ui->qsb_duration_max,SIGNAL(valueChanged(double)),
            ui->qsb_duration_min,SLOT(setMaximumValue(double)));

}

CTConfToyLRing::~CTConfToyLRing()
{
    delete ui;
}


QString CTConfToyLRing::getComment()
{
    return ui->qte_comment->toPlainText();
}


/*!
 * \brief CTConfToyLRing::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML string.
 *
 * \param xml tree containing data of all configurable parameters.
 */
bool CTConfToyLRing::setParameters(QString xml)
{
    int num_stimuli = NUM_LIGHTS + NUM_SPEAKERS;
    int num_actions = NUM_LIGHTS + NUM_SPEAKERS;

    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource();
    source->setData(xml);

    CTXmlHandler *handler = new CTXmlHandler;
    /*
     *Passing pointer of the class to the xml parser handler,
     *in order to set the parsed values into it's input fields
     */
    handler->setWidget(2, this, num_stimuli, num_actions);
    QList<CTLight*> empty1;
    QList<CTScreen*> empty2;
    QList<CTBigLight*> empty3;
    QList<CTButton*> empty4;
    handler->setStimuli(light_stimuli, speaker_stimuli, empty1, empty2,empty3,empty4);
    handler->setActions(light_actions, speaker_actions, empty2,empty3,empty4, empty1);
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);

    bool ok = xmlReader.parse(source);
    block_duration = handler->getBlockDuration();
    if(ok)
    {
        updateBlockRuntime(1.0);
    }
    return true;
}


/*!
 * \brief CTConfToyRing::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML string.
 *
 * \return XML containing data of all configurable parameters.
 */
QString CTConfToyLRing::getParameters(QString value){

    value = "This function uses QXmlStreamWriter";

    QString parameters;
    QXmlStreamWriter stream(&parameters);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(4);

    stream.writeStartElement("block");
    stream.writeAttribute("id", "1");
    stream.writeAttribute("name", "ring");

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

    stream.writeStartElement("position");
    stream.writeStartElement("arch");
    if( ui->qrb_block_position_arch->isChecked())
        stream.writeAttribute("enabled","true");
    else
        stream.writeAttribute("enabled","false");
    stream.writeAttribute("value", ui->qcb_block_position_arch->currentText());
    stream.writeEndElement();//end arch
    stream.writeStartElement("left_wall");
    if(ui->qrb_block_position_left->isChecked())
        stream.writeAttribute("enabled", "true");
    else
        stream.writeAttribute("enabled", "false");
    stream.writeAttribute("value", ui->qcb_block_position_left->currentText());
    stream.writeEndElement();//end left wall
    stream.writeStartElement("right_wall");
    if(ui->qrb_block_position_right->isChecked())
        stream.writeAttribute("enabled","true");
    else
        stream.writeAttribute("enabled","false");
    stream.writeAttribute("value", ui->qcb_block_position_right->currentText());
    stream.writeEndElement();//end right wall
    stream.writeEndElement();//end position

    /* Insert block stimuli */
    stream.writeStartElement("stimuli");
    int num_stimuli = NUM_LIGHTS + NUM_SPEAKERS;
    stream.writeAttribute("number",QString::number(num_stimuli));
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        light_stimuli.at(i)->getParameters(stream);
    }
    for (int i = 0; i < NUM_SPEAKERS; i++)
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
    else if (ui->qrb_pressure_event->isChecked())
    {
        stream.writeAttribute("id", "1");
        stream.writeAttribute("name", "pressure");
        stream.writeStartElement("condition");
        stream.writeAttribute("type", "numerical");
//        stream.writeCharacters(ui->qsb_pressure->cleanText());
        stream.writeCharacters(CTUtility::getString(ui->qsb_pressure->value()));
        stream.writeEndElement(); // end condition
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
    stream.writeEndElement(); //end event

    /* Insert block feedback actions */
    stream.writeStartElement("actions");
    int num_actions = NUM_LIGHTS + NUM_SPEAKERS;
    stream.writeAttribute("number",QString::number(num_actions));

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

    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        speaker_actions.at(i)->getParameters(stream);

        /*
         *The content of the spin boxes of the min and max duration
         *are stored into each action tag
         */
        stream.writeStartElement("duration");
//        stream.writeTextElement("from",ui->qsb_duration_min->cleanText());
//        stream.writeTextElement("to", ui->qsb_duration_max->cleanText());
        stream.writeTextElement("from",CTUtility::getString(ui->qsb_duration_min->value()));
        stream.writeTextElement("to", CTUtility::getString(ui->qsb_duration_max->value()));
        stream.writeEndElement();//end duration

        stream.writeEndElement(); //end action or stimulus

    }
    stream.writeEndElement(); //end actions
    stream.writeEndElement(); //end feedback
    stream.writeEndElement(); // end block

    return parameters;
}

void CTConfToyLRing::on_qrb_null_event_toggled(bool checked)
{
    ui->tabFeedbackAction->setDisabled(checked);
}

/*!
 * \brief CTConfToyLRing::calculateRequiredTime
 *
 * Calculates the required block duration as the highest sum of the activation
 * time and the upper bound of the duration interval of all activated stimuli.
 *
 * \return The determined time.
 */
double CTConfToyLRing::calculateRequiredTime()
{
    double time_required = 0.0;

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        CTConstLight *st_light = light_stimuli.at(i);
        if (st_light->state->isChecked())
        {
            double sum = st_light->activation->value() +
                    st_light->duration_max->value();
            if (time_required < sum) { time_required = sum; }
        }
    }
    for (int i = 0; i < NUM_SPEAKERS; i++)
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
 * \brief CTConfToyLRing::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to the activation
 * or deactivation of a stimulus.
 *
 * \param value This parameter is unused.
 */
void CTConfToyLRing::updateBlockRuntime(bool value)
{
    value = false;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
}


/*!
 * \brief CTConfToyLRing::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to a value change
 * in either the activation time control element or the control element for the
 * upper bound of the duration interval.
 *
 * \param value This parameter is unused.
 */
void CTConfToyLRing::updateBlockRuntime(double value)
{
    value = 0;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
    /*Checks if the overall value of the block duration contains the pause*/
    if(calculateRequiredTime() < block_duration)
    {
        ui->qsb_pause->setValue(block_duration - calculateRequiredTime());
    }
}
