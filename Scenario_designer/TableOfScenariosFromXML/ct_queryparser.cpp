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
        foreach(QString field_name, rec.keys())
        {
            stream.writeStartElement("field");
            stream.writeAttribute("name",field_name);
            if(field_name.contains("xml"))
                stream.writeCDATA(rec[field_name]);
            else
                stream.writeCharacters(rec[field_name]);
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
