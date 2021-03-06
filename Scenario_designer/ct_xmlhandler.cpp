#include "ct_xmlhandler.h"
#include <QDebug>

bool CTXmlHandler::setSubWidgets(int id_widget, QWidget *widget)
{
    /*Setting pointers to each ui component of the widget*/
    qte_comment = widget->findChild<QPlainTextEdit*>("qte_comment");
    qsb_block_repetitions = widget->findChild<CTSpinBox*>("qsb_block_repetitions");
    if(id_widget != CT_BLOCK_ARCH){
        qrb_null_event = widget->findChild<QRadioButton*>("qrb_null_event");
        qrb_position_event = widget->findChild<QRadioButton*>("qrb_position_event");
        qcb_position = widget->findChild<QComboBox*>("qcb_position");
        qrb_body_event = widget->findChild<QRadioButton*>("qrb_body_event");
        qcb_body = widget->findChild<QComboBox*>("qcb_body");
        qcb_head = widget->findChild<QComboBox*>("qcb_head");
        qrb_head_event = widget->findChild<QRadioButton*>("qrb_head_event");

        qsb_duration_min = widget->findChild<CTDoubleSpinBox*>("qsb_duration_min");
        qsb_duration_max = widget->findChild<CTDoubleSpinBox*>("qsb_duration_max");
    }

    if(id_widget == CT_BLOCK_FLOWER || id_widget == CT_BLOCK_MICKEY)
    {
        qrb_force_event = widget->findChild<QRadioButton*>("qrb_force_event");
        qsb_force = widget->findChild<CTDoubleSpinBox*>("qsb_force");
    }
    if(id_widget == CT_BLOCK_MICKEY || id_widget == CT_BLOCK_RING
            || id_widget == CT_BLOCK_STICK || id_widget == CT_BLOCK_U)
    {
        qrb_pressure_event = widget->findChild<QRadioButton*>("qrb_pressure_event");
        qsb_pressure = widget->findChild<CTDoubleSpinBox*>("qsb_pressure");
    }
    if(id_widget == CT_BLOCK_FLOWER || id_widget == CT_BLOCK_MICKEY)
    {
        qcb_force = widget->findChild<QComboBox*>("qcb_force");
    }
    if(id_widget == CT_BLOCK_RING || id_widget == CT_BLOCK_U)
    {
        qcb_pressure = widget->findChild<QComboBox*>("qcb_pressure");
    }
    if(id_widget == CT_BLOCK_WALL_LEFT || id_widget == CT_BLOCK_WALL_RIGHT)
    {
        qrb_button_pressed_event = widget->findChild<QRadioButton*>("qrb_button_pressed_event");
        qcb_button = widget->findChild<QComboBox*>("qcb_button");
    }
    return true;

}

void CTXmlHandler::setWidget(int id_widget, QWidget *widget, int stimuli,
                             int actions)
{
    idWidget = id_widget;
    num_stimuli = stimuli;
    num_actions = actions;
    setSubWidgets(idWidget,widget);
}

void CTXmlHandler::setStimuli(QList<CTConstLight *> &lightStimuli,
                              QList<CTSpeaker *> &speakerStimuli,
                              QList<CTLight *> &archLightStimuli,
                              QList<CTScreen *> &screenStimuli,
                              QList<CTBigLight*> &bigLightStimuli,
                              QList<CTButton *> &buttonStimuli)
{

    if(idWidget == CT_BLOCK_FLOWER || idWidget == CT_BLOCK_MICKEY
            || idWidget == CT_BLOCK_RING || idWidget == CT_BLOCK_STICK
            || idWidget == CT_BLOCK_U)
    {
        if(!lightStimuli.isEmpty() && !speakerStimuli.isEmpty())
        {
            light_stimuli = lightStimuli;
            speaker_stimuli = speakerStimuli;
         }else{
            qDebug() << "An error with the args detected: empty lists!";
         }
    }
    if(idWidget == CT_BLOCK_ARCH)
    {
        if(!archLightStimuli.isEmpty())
            arch_light_stimuli = archLightStimuli;
        else
            qDebug() << "An error with the args detected: empty lists!";
    }
    if(idWidget == CT_BLOCK_WALL_SCREEN)
    {
        if(!screenStimuli.isEmpty())
            screen_stimuli = screenStimuli;
        else
            qDebug() << "An error with the args detected: empty lists!";
    }
    if(idWidget == CT_BLOCK_WALL_LEFT || idWidget == CT_BLOCK_WALL_RIGHT)
    {
        if(!bigLightStimuli.isEmpty() && !buttonStimuli.isEmpty()
                && !archLightStimuli.isEmpty() && !speakerStimuli.isEmpty())
        {
            biglight_stimuli = bigLightStimuli;
            button_stimuli = buttonStimuli;
            arch_light_stimuli = archLightStimuli;
            speaker_stimuli = speakerStimuli;
        }
        else
            qDebug() << "An error with the args detected: empty lists!";
    }
}


void CTXmlHandler::setActions(QList<CTConstLight *> &lightAction,
                              QList<CTSpeaker *> &speakerAction,
                              QList<CTScreen *> &screenAction,
                              QList<CTBigLight *> &bigLightAction,
                              QList<CTButton *> &buttonAction,
                              QList<CTLight *> &archLightAction)
{
    if(idWidget == CT_BLOCK_FLOWER || idWidget == CT_BLOCK_MICKEY
            || idWidget == CT_BLOCK_RING || idWidget == CT_BLOCK_STICK
            || idWidget == CT_BLOCK_U)
    {
        if(!lightAction.isEmpty() && !speakerAction.isEmpty())
        {
            light_actions = lightAction;
            speaker_actions = speakerAction;
        }else{
            qDebug() << "An error with the args detected: empty lists!";
        }
    }
    if(idWidget == CT_BLOCK_WALL_SCREEN)
    {
        if(!screenAction.isEmpty())
            screen_actions = screenAction;
        else
            qDebug() << "An error with the args detected: empty lists!";
    }
    if(idWidget == CT_BLOCK_WALL_LEFT || idWidget == CT_BLOCK_WALL_RIGHT)
    {
        if(!bigLightAction.isEmpty() && !buttonAction.isEmpty()
                && !archLightAction.isEmpty() && !speakerAction.isEmpty())
        {
            biglight_actions = bigLightAction;
            button_actions = buttonAction;
            arch_light_actions = archLightAction;
            speaker_actions = speakerAction;
        }
        else
            qDebug() << "An error with the args detected: empty lists!";
    }
}

/*!
    Reset the state of the handler to ensure that new documents are
    read correctly.

    We return true to indicate that parsing should continue.
*/

bool CTXmlHandler::startDocument()
{
    elementName.clear();
    elementIndentation.clear();

    return true;
}

/*!
    Process each starting element in the XML document.

    Append the element name to the list of elements found; add its
    corresponding indentation level to the list of indentation levels.

    Increase the level of indentation by one column.

    We return true to indicate that parsing should continue.
*/

bool CTXmlHandler::startElement(const QString &, const QString &,
    const QString & qName, const QXmlAttributes &att)
{
    if(qName == "block")
    {
        block_name = att.value("name");
    }

    elementName.append(qName);

    /* Stimuli block processing*/
    if(qName == "stimuli")
    {
        /*check consistency*/
        if(num_stimuli != att.value("number").toInt())
            return false;
    }
    if(qName == "stimulus" && elementName.contains("stimuli"))
    {
        /*Processing of one stimulus*/
        if(att.value("enabled") != "false" )
            enabled = true;
        else if(att.value("enabled") == "false")
            enabled = false;
        if(att.value("name") == "light")
        {
            if(block_name != "arch" && block_name != "wall_left"
                    && block_name != "wall_right")
                light = light_stimuli[att.value("id").toInt() -1 ];
            else
                archLight = arch_light_stimuli[att.value("id").toInt() - 1];
            attr["type"] = "light_stimulus";

        }
        if(att.value("name") == "speaker")
        {
            speaker = speaker_stimuli[att.value("id").toInt() -1 ];
            attr["type"] = "speaker_stimulus";
            attr["left_front"] = att.value("left_front");
            attr["left_rear"] = att.value("left_rear");
            attr["right_front"] = att.value("right_front");
            attr["right_rear"] = att.value("right_rear");
        }
        if(att.value("name") == "screen")
        {
            screen = screen_stimuli[att.value("id").toInt() - 1];
            attr["type"] = "screen_stimulus";
        }
        if(att.value("name") == "light_cluster")
        {
            bigLight = biglight_stimuli[att.value("id").toInt() -1];
            attr["type"] = "biglight_stimulus";
        }
        if(att.value("name") == "light_button")
        {
            button = button_stimuli[att.value("id").toInt() -1];
            attr["type"] = "button_stimulus";
        }

    /* Actions block processing*/
    }if(qName == "event" && elementName.contains("feedback"))
    {
        event_name = att.value("name");
        if(!att.value("sensor").isNull())
        {
            attr["sensor"] = att.value("sensor");
        }
    }
    if(qName == "actions")
    {
        /*check consistency*/
        if(num_actions != att.value("number").toInt())
            return false;
    }
    if(qName == "action" && elementName.contains("actions") &&
            event_name != "null")
    {
        /*Processing of one action*/
        if(att.value("enabled") != "false" )
            enabled = true;
        else
            enabled = false;
        if(att.value("name") == "light")
        {
            if(block_name != "wall_left" && block_name != "wall_right")
                light = light_actions[att.value("id").toInt() -1 ];
            else
                archLight = arch_light_actions[att.value("id").toInt() -1];
            attr["type"] = "light_action";

        }
        if(att.value("name") == "speaker")
        {
            speaker = speaker_actions[att.value("id").toInt() -1 ];
            attr["type"] = "speaker_action";
            attr["left_front"] = att.value("left_front");
            attr["left_rear"] = att.value("left_rear");
            attr["right_front"] = att.value("right_front");
            attr["right_rear"] = att.value("right_rear");
        }
        if(att.value("name") == "screen")
        {
            screen = screen_actions[att.value("id").toInt() - 1];
            attr["type"] = "screen_action";
        }
        if(att.value("name") == "light_cluster")
        {
            bigLight = biglight_actions[att.value("id").toInt() -1];
            attr["type"] = "biglight_action";
        }
        if(att.value("name") == "light_button")
        {
            button = button_actions[att.value("id").toInt() -1];
            attr["type"] = "button_action";
        }
    }
    return true;
}

bool CTXmlHandler::characters(const QString& ch)
{

    /* Set block comment */
    if(elementName.contains("comment"))
        qte_comment->setPlainText(ch);
    /* Set block runtime */
    else if(elementName.contains("runtime") && elementName.contains("duration"))
    {
        block_duration = ch.toDouble();
        qDebug() << "parsed block duration" << block_duration;}
    else if(elementName.contains("runtime") && elementName.contains("repetitions"))
        qsb_block_repetitions->setValue(ch.toInt());

    /* Set block stimuli/actions attributes*/
    else if(elementName.contains("stimulus") && elementName.contains("activation"))
    {
        attr["val_activation"] = ch;
    }
    else if(elementName.contains("volume") && elementName.contains("from"))
    {
        attr["val_volume_min"] = ch;
    }
    else if(elementName.contains("volume") && elementName.contains("to"))
    {
        attr["val_volume_max"] = ch;
    }
    else if(elementName.contains("sound"))
    {
        attr["sound"] = ch;
    }
    else if(elementName.contains("intensity") && elementName.contains("from"))
    {
        attr["val_intensity_min"] = ch;
    }
    else if(elementName.contains("intensity") && elementName.contains("to"))
    {
        attr["val_intensity_max"] = ch;
    }
    else if(elementName.contains("area") && elementName.contains("from"))
    {
        attr["val_rings_min"] = ch;
    }
    else if(elementName.contains("area") && elementName.contains("to"))
    {
        attr["val_rings_max"] = ch;
    }
    else if(elementName.contains("color"))
    {
        attr["val_color"] = ch;
    }
    else if(elementName.contains("video"))
    {
        attr["val_video"] = ch;
    }
    else if(elementName.contains("duration") && elementName.contains("from"))
    {
        attr["val_duration_min"] = ch;
    }
    else if(elementName.contains("duration") && elementName.contains("to"))
    {
        attr["val_duration_max"] = ch;
        if(event_name.isNull() || event_name != "none"){
            if(attr["type"] == "light_action" || attr["type"] == "speaker_action"
                    || attr["type"] == "screen_action")
            {
                qsb_duration_min->setMinimum(attr["val_duration_min"].toDouble());
                qsb_duration_min->setValue(attr["val_duration_min"].toDouble());
                qsb_duration_max->setValue(attr["val_duration_max"].toDouble());
            }
            if((attr["type"] == "light_stimulus" || attr["type"] == "light_action")
                    && block_name != "arch" && block_name != "wall_left"
                    && block_name != "wall_right")
            {
                light->setParameters(enabled, attr);
                attr.clear();
            }
            if(attr["type"] == "light_stimulus" && block_name == "arch")
            {
                archLight->setParameters(enabled,attr);
                attr.clear();
            }
            if((attr["type"] == "light_stimulus" || attr["type"] == "light_action")
                    && ((block_name == "wall_left") || block_name == "wall_right"))
            {
                archLight->setParameters(enabled,attr);
            }
            if(attr["type"] == "speaker_stimulus" || attr["type"] == "speaker_action")
            {
                speaker->setParameters(enabled,attr);
                attr.clear();
            }
            if(attr["type"] == "screen_stimulus" || attr["type"] == "screen_action")
            {
                screen->setParameters(enabled,attr);
                attr.clear();
            }
            if(attr["type"] == "biglight_stimulus" || attr["type"] == "biglight_action")
            {
                bigLight->setParameters(enabled,attr);
                attr.clear();
            }
            if(attr["type"] == "button_stimulus" || attr["type"] == "button_action")
            {
                button->setParameters(enabled,attr);
                attr.clear();
            }
        }
    }


    /* Set block feedback events */
    else if(elementName.contains("feedback") && elementName.contains("event")
            && elementName.contains("condition"))
    {
        if("null" == event_name)
        {
           qrb_null_event->setChecked(true);
        }
        else if("pressure" == event_name)
        {
            qrb_pressure_event->setChecked(true);
            qsb_pressure->setValue(ch.toDouble());
            if(!attr["sensor"].isNull())
            {
                qcb_pressure->setCurrentIndex(qcb_pressure->findText(
                                                  attr["sensor"].toString()));
            }
        }
        else if("force" == event_name)
        {
            qrb_force_event->setChecked(true);
            if(!attr["sensor"].isNull())
            {
                qcb_force->setCurrentIndex(qcb_force->findText(
                                           attr["sensor"].toString()));
            }
            qsb_force->setValue(ch.toDouble());
        }
        else if("position" == event_name)
        {
            qrb_position_event->setChecked(true);
            qcb_position->setCurrentIndex(qcb_position->findText(ch));
        }
        else if("body" == event_name)
        {
            qrb_body_event->setChecked(true);
            qcb_body->setCurrentIndex(qcb_body->findText(ch));
        }
        else if("head" == event_name)
        {
            qrb_head_event->setChecked(true);
            qcb_head->setCurrentIndex(qcb_head->findText(ch));
        }
        else if("button" == event_name)
        {
            qrb_button_pressed_event->setChecked(true);
            qcb_button->setCurrentIndex(qcb_button->findText(attr["sensor"].toString()));
        }
    }
    return true;
}

/*!
    Process each ending element in the XML document.
    Decrease the level of indentation by one column.
    We return true to indicate that parsing should continue.
*/

bool CTXmlHandler::endElement(const QString &, const QString &,
    const QString &)
{
    elementName.removeLast();

    return true;
}

/*!
    Report a fatal parsing error, and return false to indicate to the reader
    that parsing should stop.
*/

bool CTXmlHandler::fatalError (const QXmlParseException & exception)
{
    qWarning() << QString("Fatal error on line %1, column %2: %3").arg(
        exception.lineNumber()).arg(exception.columnNumber()).arg(
        exception.message());

    return false;
}


double CTXmlHandler::getBlockDuration()
{
    return this->block_duration;
}
