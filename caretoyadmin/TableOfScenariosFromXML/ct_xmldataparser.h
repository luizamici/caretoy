#ifndef CT_XMLPARSER_H
#define CT_XMLPARSER_H

#include <QXmlStreamReader>
#include <QDebug>
#include <QObject>


//class CTTableData;
#include "ct_tabledata.h"

class CTXmlDataParser : public QObject
{
    Q_OBJECT
public:
    CTXmlDataParser();

    static CTTableData* parse_table(const QByteArray &table_data);
};


#endif // CT_XMLPARSER_H
