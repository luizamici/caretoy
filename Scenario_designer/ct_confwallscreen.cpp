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
 * supplied XML tree.
 *
 * \param root XML tree containing data of all configurable parameters.
 */
bool CTConfWallScreen::setParameters(QDomElement root)
{
    /* Check if the supplied configuration is applicable */
    if ("block" != root.tagName() || "wall_screen" != root.attribute("name"))
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
    int num_stimuli = NUM_SCREENS;
    QDomElement stimuli_block = root.namedItem("stimuli").toElement();
    int stimuli_count = stimuli_block.attribute("number").toInt();
    QDomNodeList stimuli = stimuli_block.childNodes();

    /* Check consistency */
    if (num_stimuli != stimuli_count || num_stimuli != stimuli.size())
    {
        qDebug() << "num_stimuli != stimuli_count || num_stimuli != stimuli.size() :" << num_stimuli << stimuli_count << stimuli.size();
        return false;
    }

    for (int i = 0; i < num_stimuli; i++)
    {
        QDomElement stimulus = stimuli.at(i).toElement();
        /* Do not process disabled stimuli */
        if ("false" == stimulus.attribute("enabled")) { continue; }

        screen_stimuli[i]->setParameters(stimulus);
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
    int num_actions = NUM_SCREENS;
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

        if (i < NUM_SCREENS)
        {
            screen_action[i]->setParameters(action);

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
 * \brief CTConfWallScreen::getParameters
 *
 * Retrieves values of all configurable block parameters and returns them as
 * an XML tree.
 *
 * \return XML tree containing data of all configurable parameters.
 */
QDomElement CTConfWallScreen::getParameters()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("block");
    root.setAttribute("id", 1);
    root.setAttribute("name", "wall_screen");

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
    int num_stimuli = NUM_SCREENS;
    stimuli.setAttribute("number", num_stimuli);

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        QDomElement parameters = screen_stimuli.at(i)->getParameters();
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
    }else if (ui->qrb_position_event->isChecked())
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

    /* Insert block feedback actions */
    QDomElement actions = doc.createElement("actions");
    feedback.appendChild(actions);
    int num_actions = NUM_SCREENS;
    actions.setAttribute("number", num_actions);

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        QDomElement parameters = screen_action.at(i)->getParameters();

        QDomDocument doc;
        QDomElement xml_duration = doc.createElement("duration");
        parameters.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(ui->qsb_duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(ui->qsb_duration_max->cleanText()));
//        parameters.appendChild(xml_duration);
        actions.appendChild(parameters);
    }

    return root;
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
}

void CTConfWallScreen::on_qrb_null_event_toggled(bool checked)
{
    ui->tabFeedbackAction->setDisabled(checked);
}
