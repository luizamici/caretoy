#include "datatoxml.h"

DataToXml::DataToXml(QObject *parent) :
    QObject(parent)
{
}


QString DataToXml::tableToXml(QString tableName,QList<QSqlRecord> tableInfo,
                              QList<QSqlRecord> recs)
{
    qDebug() << "DataToXml::tableToXml";
    QList<QSqlRecord> _fields = tableInfo;

    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.setAutoFormatting(true);
    stream.writeStartElement("table");
    stream.writeAttribute("name", tableName);
    stream.writeStartElement("fields");

    if(!recs.isEmpty())
    {
        QSqlRecord _rec = recs.at(0);
        _fields.clear();
        for (int var = 0; var < _rec.count(); ++var) {
            foreach(QSqlRecord column, tableInfo)
            {
                if(_rec.fieldName(var) == column.value("column_name").toString())
                    _fields.append(column);
            }
        }
    }
    foreach(QSqlRecord column, _fields)
        {
                stream.writeStartElement("field");
                stream.writeAttribute("name", column.value("column_name").
                                      toString());
                stream.writeAttribute("type", column.value("data_type").
                                      toString());
                stream.writeAttribute("is_nullable", column.value("is_nullable").
                                      toString());
                stream.writeAttribute("char_max_length", column.value(
                                      "character_maximum_length").toString());
                stream.writeAttribute("numeric_precision", column.value(
                                          "numeric_precision").toString());
                //Setting auto value to true in case of serial
                if(column.value("data_type").toString()=="integer" &
                        column.value("column_default").toString().startsWith("nextval"))
                    stream.writeAttribute("auto", "true");
                else
                    stream.writeAttribute("auto", "false");
                stream.writeAttribute("constraint_type", column.value(
                                          "constraint_type").toString());
                stream.writeAttribute("column_default", column.value(
                                          "column_default").toString());
                stream.writeEndElement();//end field
            }
    stream.writeEndElement();//end fields

    foreach(QSqlRecord record, recs)
    {
        stream.writeStartElement("row");
        for(int i = 0; i < record.count(); i ++)
        {
            stream.writeStartElement(record.fieldName(i));
            if(record.fieldName(i).contains("xml"))
                stream.writeCDATA(record.value(i).toString());
            else
                stream.writeCharacters(record.value(i).toString());
            stream.writeEndElement();//end fieldName
        }
        stream.writeEndElement();//end row
    }
    stream.writeEndElement();//end table
    return xml;
}
