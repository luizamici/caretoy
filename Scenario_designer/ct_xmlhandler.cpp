#include "ct_xmlhandler.h"
#include <QDebug>

void CTXmlHandler::setWidget(QString id_widget, QWidget *widget, int stimuli, int actions)
{
    idWidget = id_widget;
    num_stimuli = stimuli;
    num_actions = actions;
    if(idWidget == "flower")
    {
        CTConfToyFlower *flower = static_cast<CTConfToyFlower*>(widget);

        /*Setting pointers to each ui component of the widget flower*/
        qte_comment = flower->findChild<QPlainTextEdit*>("qte_comment");
        qsb_block_repetitions = flower->findChild<CTSpinBox*>("qsb_block_repetitions");
        qrb_null_event = flower->findChild<QRadioButton*>("qrb_null_event");
        qrb_force_event = flower->findChild<QRadioButton*>("qrb_force_event");
        qcb_force = flower->findChild<QComboBox*>("qcb_force");
        qsb_force = flower->findChild<CTDoubleSpinBox*>("qsb_force");
        qrb_position_event = flower->findChild<QRadioButton*>("qrb_position_event");
        qcb_position = flower->findChild<QComboBox*>("qcb_position");
        qrb_body_event = flower->findChild<QRadioButton*>("qrb_body_event");
        qcb_body = flower->findChild<QComboBox*>("qcb_body");
        qcb_head = flower->findChild<QComboBox*>("qcb_head");
        qrb_head_event = flower->findChild<QRadioButton*>("qrb_head_event");

        qsb_duration_min = flower->findChild<CTDoubleSpinBox*>("qsb_duration_min");
        qsb_duration_max = flower->findChild<CTDoubleSpinBox*>("qsb_duration_max");
    }
}

void CTXmlHandler::setStimuli(QList<CTConstLight *> &lightStimuli,
                              QList<CTSpeaker *> &speakerStimuli)
{
    if(idWidget == "flower")
    {
        if(!lightStimuli.isEmpty() && !speakerStimuli.isEmpty())
        {
            light_stimuli = lightStimuli;
            speaker_stimuli = speakerStimuli;
        }else{qDebug() << "An error with the args detected: empty lists!";}
    }
}

void CTXmlHandler::setActions(QList<CTConstLight *> &lightAction,
                              QList<CTSpeaker *> &speakerAction)
{
    if(idWidget == "flower")
    {
        if(!lightAction.isEmpty() && !speakerAction.isEmpty())
        {
            light_actions = lightAction;
            speaker_actions = speakerAction;
        }else{qDebug() << "An error with the args detected: empty lists!";}
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
    indentationLevel = 0;

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
        /*Checking the block consistency*/
        if(att.value("name") != idWidget)
            return false;

//    qDebug() << "appending to the list: " << qName;
    elementName.append(qName);
    elementIndentation.append(indentationLevel);

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
            light = light_stimuli[att.value("id").toInt() -1 ];
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
    /* Actions block processing*/
    }if(qName == "event" && elementName.contains("feedback"))
    {
        attr["event_name"] = att.value("name");
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
    if(qName == "action" && elementName.contains("actions"))
    {
        /*Processing of one action*/
        if(att.value("enabled") != "false" )
            enabled = true;
        else
            enabled = false;
        if(att.value("name") == "light")
        {
            light = light_actions[att.value("id").toInt() -1 ];
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
    }

    indentationLevel += 1;
    return true;
}

bool CTXmlHandler::characters(const QString& ch)
{

    /* Set block comment */
    if(elementName.contains("comment"))
        qte_comment->setPlainText(ch);
    /* Set block runtime */
    if(elementName.contains("runtime") && elementName.contains("duration"))
    {
        block_duration = ch.toDouble();
        qDebug() << "parsed block duration" << block_duration;}
    if(elementName.contains("runtime") && elementName.contains("repetitions"))
        qsb_block_repetitions->setValue(ch.toInt());

    /* Set block stimuli/actions attributes*/

    if(elementName.contains("stimulus") && elementName.contains("activation"))
        attr["val_activation"] = ch;
    if(elementName.contains("volume") && elementName.contains("from"))
    {
        attr["val_volume_min"] = ch;
    }
    if(elementName.contains("volume") && elementName.contains("to"))
    {
        attr["val_volume_max"] = ch;
    }
    if(elementName.contains("sound"))
    {
        attr["sound"] = ch;
    }
    if(elementName.contains("duration") && elementName.contains("from"))
        attr["val_duration_min"] = ch;
    if(elementName.contains("duration") && elementName.contains("to"))
    {
        attr["val_duration_max"] = ch;
        if(attr["type"] == "light_action" || attr["type"] == "speaker_action")
        {
            qsb_duration_min->setMinimum(attr["val_duration_min"].toDouble());
            qsb_duration_min->setValue(attr["val_duration_min"].toDouble());
            qsb_duration_max->setValue(attr["val_duration_max"].toDouble());
        }
        if(attr["type"] == "light_stimulus" || attr["type"] == "light_action")
        {
            light->setParameters(enabled, attr);
            attr.clear();
        }
        if(attr["type"] == "speaker_stimulus" || attr["type"] == "speaker_action")
        {
            speaker->setParameters(enabled,attr);
            attr.clear();
        }
    }


    /* Set block feedback events */
    if(elementName.contains("feedback") && elementName.contains("event")
            && elementName.contains("condition"))
    {
        if("null" == attr["event_name"])
        {
           qrb_null_event->setChecked(true);
        }
        else if("force" == attr["event_name"])
        {
            qrb_force_event->setChecked(true);
            qcb_force->setCurrentIndex(qcb_force->findText(
                                           attr["sensor"].toString()));
            qsb_force->setValue(ch.toDouble());
        }
        else if("position" == attr["event_name"])
        {
            qrb_position_event->setChecked(true);
            qcb_position->setCurrentIndex(qcb_position->findText(ch));
        }
        else if("body" == attr["event_name"])
        {
            qrb_body_event->setChecked(true);\
            qcb_body->setCurrentIndex(qcb_body->findText(ch));
        }
        else if("head" == attr["event_name"])
        {
            qrb_head_event->setChecked(true);
            qcb_head->setCurrentIndex(qcb_head->findText(ch));
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
//    qDebug() << "removing from the list: " << elementName.last();
    elementName.removeLast();
    indentationLevel -= 1;

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

/*!
    Return the list of element names found.
*/

QStringList& CTXmlHandler::names ()
{
    return elementName;
}

/*!
    Return the list of indentations used for each element found.
*/

QList<int>& CTXmlHandler::indentations ()
{
    return elementIndentation;
}

double CTXmlHandler::getBlockDuration()
{
    return this->block_duration;
}
