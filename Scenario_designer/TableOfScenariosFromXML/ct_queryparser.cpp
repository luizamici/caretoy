#include "ct_queryparser.h"

CTQueryParser::CTQueryParser()
{
}


/*Prepares xml statements for query execution*/
QString CTQueryParser::xmlStatement(StatementType type, const QString &tableName,
                                 const QHash<QString,QString> &rec)
{
    QString statement;
    QXmlStreamWriter stream(&statement);
    stream.setAutoFormatting(true);

    switch(type)
    {
    case SelectStatement:
        stream.writeStartElement("select");
        stream.writeStartElement("table");
        stream.writeAttribute("name", tableName);
        stream.writeStartElement("fields");
        stream.writeAttribute("number", QString::number(rec.size()));
        foreach(QString field_name, rec.keys())
        {
            stream.writeStartElement("field");
            stream.writeAttribute("name", field_name);
            stream.writeEndElement();//end field
        }
        stream.writeEndElement();//end fields
        stream.writeEndElement();//end table
        stream.writeEndElement();//end select
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
        stream.writeStartElement("update");
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
    case DeleteStatement:
        stream.writeStartElement("delete");
        stream.writeStartElement("table");
        stream.writeAttribute("name" , tableName);
        stream.writeEndElement();//end table
        stream.writeEndElement();//end delete
        break;
    case WhereStatement:
        stream.writeStartElement("where");
        stream.writeStartElement("condition");
        foreach(QString fieldName, rec.keys())
        {
            stream.writeStartElement("field");
            stream.writeAttribute("name", fieldName);
            stream.writeEndElement();//end field
            stream.writeStartElement("op");
            stream.writeAttribute("type", "eq");
            stream.writeEndElement();//end op
            stream.writeStartElement("value");
            stream.writeCharacters(rec[fieldName]);
            stream.writeEndElement();//end value
        }
        stream.writeEndElement();//end condition
        stream.writeEndElement();//end where
        break;
    default:
        break;
    }

    return statement;
}
