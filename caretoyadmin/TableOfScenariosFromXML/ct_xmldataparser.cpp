#include "ct_xmldataparser.h"
//#include "ct_tabledata.h"

CTXmlDataParser::CTXmlDataParser()
{
}

CTTableData *CTXmlDataParser::parse_table(const QByteArray &table_data)
{
    qDebug() << table_data;
    bool tableEmpty = true;
    QXmlStreamReader reader(table_data);
    CTTableData *tableData;
    CTTableRecord rec;
    CTTableRecord _rec;
    QList<CTTableField> fields;
    QStringList fieldNames;
    QList< QPair< QString, CTTableField> > _constraints;
    int i;

    while(!reader.atEnd())
    {
        reader.readNext();

        /*When a new block is found*/
        if(reader.isStartElement() && reader.name() == "table")
        {
            tableData = new CTTableData(reader.attributes().value("name").toString());
        }
        if(reader.isStartElement() && reader.name() == "field")
        {
            CTTableField field;
            field.setName(reader.attributes().value("name").toString());
            field.setType(reader.attributes().value("type").toString());

            if(reader.attributes().value("is_nullable").toString() == "YES")
                field.setRequiredStatus(CTTableField::Required);
            else
                field.setRequiredStatus(CTTableField::Optional);

            field.setLength(reader.attributes().value("char_max_length").
                            toString().toInt());
            field.setPrecision(reader.attributes().value("numeric_precision").
                               toString().toInt());

            reader.attributes().value("auto").toString() == "true"?
                        field.setAutoValue(true): field.setAutoValue(false);

            field.setDefaultValue(reader.attributes().value("column_default").toString());

            if(!reader.attributes().value("constraint_type").toString().isEmpty())
            {
                QPair<QString, CTTableField> p;
                p.first = reader.attributes().value("constraint_type").toString();
                p.second = field;
                _constraints.append(p);
            }
            if(!fieldNames.contains(field.name()))
            {
                fieldNames << field.name();
                fields.append(field);
            }
        }
        if(reader.isEndElement() && reader.name() == "fields")
        {
            /*
             *When finished reading fields from the xml a record is created
             *with default values
             */
            rec = CTTableRecord();
            for(int i = 0; i< fields.size(); i++)
                rec.append(fields.at(i));
        }
        if(reader.isStartElement() && reader.name() == "row")
        {
//            _rec = rec;
            tableEmpty = false;
            i = 0;
        }

        if(reader.isCharacters())
        {
            if(!reader.text().toString().trimmed().isEmpty())
            {
//                qDebug() << Q_FUNC_INFO << i << reader.text().toString();
                rec.setValue(i,reader.text().toString());
                i++;
            }
        }
        if(reader.isEndElement() && reader.name() == "row")
        {
//            qDebug()<< rec.count();
            tableData->insertRecord(-1,rec);
        }
        if(reader.isEndElement() && reader.name() == "table" && tableEmpty == true)
        {
//            qDebug()<< rec.count();
            tableData->insertRecord(-1, rec);
            tableData->constraints = _constraints;
        }
        if(reader.isEndElement() && reader.name() == "table" && tableEmpty != true)
        {
            tableData->constraints = _constraints;
        }
    }
//    qDebug() << Q_FUNC_INFO << tableData->numColumns;
    return tableData;
}
