#include "ct_confwallscreen.h"
#include "ui_ct_confwallscreen.h"

#define NUM_SCREENS 1

CTConfWallScreen::CTConfWallScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTConfWallScreen)
{
    ui->setupUi(this);

    /*Initialy disables the tab for the feedback-action*/
    ui->tabFeedbackAction->setDisabled(true);
    block_duration = (double) 0.0;

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        CTScreen *st_screen = new CTScreen(i+1, false);
        screen_stimuli.append(st_screen);
        ui->qgb_stimuli->layout()->addWidget(st_screen);

        connect(st_screen->state, SIGNAL(toggled(bool)),
                this, SLOT(updateBlockRuntime(bool)));
        connect(st_screen->activation, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));
        connect(st_screen->duration_max, SIGNAL(valueChanged(double)),
                this, SLOT(updateBlockRuntime(double)));


        CTScreen *ac_screen = new CTScreen(i+1,true);
        screen_action.append(ac_screen);
        ui->qgb_actions->layout()->addWidget(ac_screen);


        /* Add separating lines */
        if (i+1 != NUM_SCREENS)
        {
            QFrame *qh_st_line = new QFrame();
            qh_st_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            ui->qgb_stimuli->layout()->addWidget(qh_st_line);
        }
    }

     /* Establish automatic enabling/disabling of related event controls */
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

    /* Fill feedback control elements with content */
    ui->qcb_position->addItems(QStringList() << "prone" << "supine" << "left side" << "right side");
}

CTConfWallScreen::~CTConfWallScreen()
{
    delete ui;
}


/*!
 * \brief CTConfWallScreen::setParameters
 *
 * Sets values of all configurable block parameters based on the content of the
 * supplied XML string.
 *
 * \param xml containing data of all configurable parameters.
 */
bool CTConfWallScreen::setParameters(QString xml)
{
    int num_stimuli = NUM_SCREENS;
    int num_actions = NUM_SCREENS;

    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource();
    source->setData(xml);

    CTXmlHandler *handler = new CTXmlHandler;
    /*
     *Passing pointer of the class to the xml parser handler,
     *in order to set the parsed values into it's input fields
     */
    handler->setWidget(8, this, num_stimuli, num_actions);
    QList<CTConstLight*> empty1;
    QList<CTSpeaker*> empty2;
    QList<CTLight*> empty3;
    QList<CTBigLight*> empty4;
    QList<CTButton*> empty5;
    handler->setStimuli(empty1, empty2, empty3, screen_stimuli,empty4, empty5);
    handler->setActions(empty1, empty2, screen_action,empty4, empty5, empty3);

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
 * \brief CTConfWallScreen::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML string.
 *
 * \return XML containing data of all configurable parameters.
 */
QString CTConfWallScreen::getParameters(QString value){

    value = "This function uses QXmlStreamWriter";

    QString parameters;
    QXmlStreamWriter stream(&parameters);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(4);

    stream.writeStartElement("block");
    stream.writeAttribute("id", "1");
    stream.writeAttribute("name", "wall_screen");

    /* Insert block comment */
    stream.writeStartElement("comment");
    stream.writeCharacters(ui->qte_comment->toPlainText());
    stream.writeEndElement(); //end comment

    /* Insert block runtime */
    stream.writeStartElement("runtime");
    double duration_calculated = ui->qsb_block_duration->cleanText().toDouble()
            + ui->qsb_pause->cleanText().toDouble();
    stream.writeTextElement("duration",QString::number(duration_calculated));
    stream.writeTextElement("repetitions",ui->qsb_block_repetitions->cleanText());
    stream.writeEndElement(); //end runtime

    /* Insert block stimuli */
    stream.writeStartElement("stimuli");
    int num_stimuli = NUM_SCREENS;
    stream.writeAttribute("number",QString::number(num_stimuli));
    for (int i = 0; i < NUM_SCREENS; i++)
    {
        screen_stimuli.at(i)->getParameters(stream);
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
        stream.writeAttribute("id", "1");
        stream.writeAttribute("name", "position");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_position->currentText());
        stream.writeEndElement();//end condition
    }
    else if (ui->qrb_body_event->isChecked())
    {
        stream.writeAttribute("id", "2");
        stream.writeAttribute("name", "body");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_body->currentText());
        stream.writeEndElement();//end condition
    }
    else if (ui->qrb_head_event->isChecked())
    {
        stream.writeAttribute("id", "3");
        stream.writeAttribute("name", "head");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters(ui->qcb_head->currentText());
        stream.writeEndElement();//end condition
    }
    else if(ui->qrb_headlifted_event->isChecked())
    {
        stream.writeAttribute("id", "6");
        stream.writeAttribute("name", "head_lift");
        stream.writeStartElement("condition");
        stream.writeAttribute("type","textual");
        stream.writeCharacters("head up");
        stream.writeEndElement();//end condition
    }
    stream.writeEndElement(); //end event

    /* Insert block feedback actions */
    stream.writeStartElement("actions");
    int num_actions = NUM_SCREENS;
    stream.writeAttribute("number",QString::number(num_actions));

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        screen_action.at(i)->getParameters(stream);

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

    stream.writeEndElement(); //end actions
    stream.writeEndElement(); //end feedback
    stream.writeEndElement(); // end block

    return parameters;
}


/*!
 * \brief CTConfWallScreen::calculateRequiredTime
 *
 * Calculates the required block duration as the highest sum of the activation
 * time and the upper bound of the duration interval of all activated stimuli.
 *
 * \return The determined time.
 */
double CTConfWallScreen::calculateRequiredTime()
{
    double time_required = 0.0;

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        CTScreen *st_screen = screen_stimuli.at(i);
        if (st_screen->state->isChecked())
        {
            double sum = st_screen->activation->value() +
                    st_screen->duration_max->value();
            if (time_required < sum) { time_required = sum; }
        }
    }

    return time_required;
}

/*!
 * \brief CTConfWallScreen::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to the activation
 * or deactivation of a stimulus.
 *
 * \param value This parameter is unused.
 */
void CTConfWallScreen::updateBlockRuntime(bool value)
{
    value = false;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
}

/*!
 * \brief CTConfWallScreen::updateBlockRuntime
 *
 * Triggers recalculation of the required block duration due to a value change
 * in either the activation time control element or the control element for the
 * upper bound of the duration interval.
 *
 * \param value This parameter is unused.
 */
void CTConfWallScreen::updateBlockRuntime(double value)
{
    value = 0;
    ui->qsb_block_duration->setValue(calculateRequiredTime());
    /*Checks if the overall value of the block duration contains the pause*/
    if(calculateRequiredTime() < block_duration)
    {
        ui->qsb_pause->setValue(block_duration - calculateRequiredTime());
    }
}

void CTConfWallScreen::on_qrb_null_event_toggled(bool checked)
{
    ui->tabFeedbackAction->setDisabled(checked);
}
