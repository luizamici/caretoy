#include "ct_queryparser.h"

CTQueryParser::CTQueryParser()
{
}

QString CTQueryParser::statement(StatementType type, const QString &tableName,
                                 const QHash<QString,QString> &rec) const
{
    QString statement;
    QXmlStreamWriter stream(&statement);
    stream.setAutoFormatting(true);

    switch(type)
    {
    case SelectStatement:
        break;
    case InsertStatement:
        stream.writeStartElement("insert");
        stream.writeStartElement("table");
        stream.writeAttribute("name", tableName);
        stream.writeStartElement("fields");
        stream.writeAttribute("number", QString::number(rec.size()));
        foreach(QString key, rec.keys())
        {
            stream.writeStartElement("field");
            stream.writeAttribute("name",key);
            if(key == "xml_description")
                stream.writeCDATA(rec[key]);
            else
                stream.writeCharacters(rec[key]);
            stream.writeEndElement(); // end field
        }
        stream.writeEndElement();// end fields
        stream.writeEndElement();//end table
        stream.writeEndElement();// end insert
        break;
    case UpdateStatement:
        break;
    case DeleteStatement:
        break;
    case WhereStatement:
        break;
    default:
        break;
    }

    return statement;
}
