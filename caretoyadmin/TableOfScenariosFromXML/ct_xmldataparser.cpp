#include "ct_xmldataparser.h"
//#include "ct_tabledata.h"

CTXmlDataParser::CTXmlDataParser()
{
}

CTTableData *CTXmlDataParser::parse_table(const QByteArray &table_data)
{
    QXmlStreamReader reader(table_data);
    CTTableData *tableData;
    CTTableRecord rec;
    CTTableRecord _rec;
    QList<CTTableField> fields;
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

            if(!reader.attributes().value("constraint_type").toString().isEmpty())
            {
                QPair<QString, CTTableField> p;
                p.first = reader.attributes().value("constraint_type").toString();
                p.second = field;
                _constraints.append(p);
            }
            fields.append(field);
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
            _rec = rec;
            i = 0;
        }

        if(reader.isCharacters())
        {
            if(!reader.text().toString().trimmed().isEmpty())
            {
                _rec.setValue(i,reader.text().toString());
                i++;
            }
        }
        if(reader.isEndElement() && reader.name() == "row")
        {
            tableData->insertRecord(-1,_rec);
        }

        if(reader.isEndElement() && reader.name() == "table")
        {
            tableData->constraints = _constraints;
        }
    }
    return tableData;
}
