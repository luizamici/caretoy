#include "ct_queryparser.h"
//#include "ct_tabledata.h"

CTQueryParser::CTQueryParser()
{
}


QString CTQueryParser::xmlStatement(StatementType type, const QString &tableName,
                                    const CTTableRecord &row)
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
        stream.writeAttribute("number", QString::number(row.count()));
        for(int i = 0; i< row.count(); i++)
        {
            stream.writeStartElement("field");
            stream.writeAttribute("name", row.value(i));
            stream.writeEndElement();
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
        stream.writeAttribute("number", QString::number(row.count()));
        for(int i = 0; i< row.count(); i++)
        {
            /*the auto increment types of the DB are skipped*/
            if(row.field(i).defaultValue().isEmpty() && !row.value(i).trimmed().isEmpty())
            {
                stream.writeStartElement("field");
                stream.writeAttribute("name", row.fieldName(i));
                if(row.fieldName(i).contains("xml"))
                    stream.writeCDATA(row.value(i));
                else
                    stream.writeCharacters(row.value(i));
                stream.writeEndElement();//end field
            }
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
        stream.writeAttribute("number", QString::number(row.count()));
        for(int i = 0; i< row.count(); i++)
        {
            if(row.field(i).defaultValue().isEmpty() && !row.value(i).trimmed().isEmpty())
            {
                stream.writeStartElement("field");
                stream.writeAttribute("name",row.fieldName(i));
                if(row.fieldName(i).contains("xml"))
                    stream.writeCDATA(row.value(i));
                else
                    stream.writeCharacters(row.value(i));
                stream.writeEndElement();//end field
            }
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
        for(int i = 0; i< row.count(); i++)
        {
            /*Only nn emtpy fields of the rec are prim keys*/
            if(row.value(i) != row.field(i).type())
            {
                stream.writeStartElement("field");
                stream.writeAttribute("name",row.fieldName(i));
                stream.writeEndElement();//end field
                stream.writeStartElement("op");
                stream.writeAttribute("type", "eq");
                stream.writeEndElement();//end op
                stream.writeStartElement("value");
                stream.writeCharacters(row.value(i));
                stream.writeEndElement();//end value
            }
        }
        stream.writeEndElement();//end condition
        stream.writeEndElement();//end where
        break;
    default:
        break;
    }
    return statement;
}

/*Wraps the initStms and the whereStmt into one xml query statement*/
QString CTQueryParser::prepareQuery(QString initStmt, QString whereStmt)
{
    QByteArray arr;
    arr.append(initStmt);
    if(!whereStmt.trimmed().isEmpty())
        arr.append(whereStmt);

    QString stmt;
    QXmlStreamWriter stream(&stmt);
    stream.setAutoFormatting(true);
    stream.writeStartElement("query");

    QXmlStreamReader reader(arr);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.tokenType() != QXmlStreamReader::StartDocument)
            stream.writeCurrentToken(reader);
        /*
         *Workaround for not stopping to read the xml when the initial
         *element finished(case xml concatenated)
         */
        if(reader.tokenType() == QXmlStreamReader::Invalid)
        {
            reader.clear();
            reader.addData(whereStmt);
            //in case the invalid state is not due to the concatenated xml
            if(reader.isStartElement() && reader.name() != "where")
                break;
        }
    }
    stream.writeEndElement();//end query
    return stmt;
}
