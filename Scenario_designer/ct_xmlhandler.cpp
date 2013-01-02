#include "ct_xmlhandler.h"
#include <QDebug>

void CTXmlHandler::setWidget(QString id, QWidget *widget)
{
    if(id == "flower")
    {
        CTConfToyFlower *flower = static_cast<CTConfToyFlower*>(widget);
        qte_comment = flower->findChild<QPlainTextEdit*>("qte_comment");
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

    //TODO implementation of the values setting in the form
    //that will be passed by reference to the class constructor

    if(qName == "comment"){
        qte_comment->setPlainText(att.value(0));
    }
    elementName.append(qName);
    elementIndentation.append(indentationLevel);
    indentationLevel += 1;

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
