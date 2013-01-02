#ifndef CT_XMLHANDLER_H
#define CT_XMLHANDLER_H

#include <QXmlDefaultHandler>
#include "ct_conftoyflower.h"

class CTXmlHandler : public QXmlDefaultHandler
{
public:

    bool startDocument();
    bool startElement(const QString &, const QString &, const QString &qName,
                         const QXmlAttributes & att);
    bool endElement(const QString &, const QString &, const QString &);

    bool fatalError(const QXmlParseException &exception);

    QStringList& names();
    QList<int>& indentations();
    void setWidget(QString id, QWidget *widget);

    QPlainTextEdit *qte_comment;

private:
    int indentationLevel;
    QStringList elementName;
    QList<int> elementIndentation;
};

#endif // CT_XMLHANDLER_H
