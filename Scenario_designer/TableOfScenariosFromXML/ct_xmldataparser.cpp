#include "ct_xmldataparser.h"

CTXmlDataParser::CTXmlDataParser()
{

}

/*
 *Open xml file containing the table of scenarios
 */
void CTXmlDataParser::initialize()
{

    QString name = QFileDialog::getOpenFileName(0, "Load table of scenarios",
                                                QDir::currentPath());
    if (!name.isNull())
    {
        file = new QFile(name);
        if (!file->open(QIODevice::ReadWrite | QIODevice::Text)) {
            QMessageBox::critical(0,"CTXmlParser::initialize",
                                     "Couldn't open " + name,
                                     QMessageBox::Ok);
            return;
        }
    }
}

/*
 *Binding the file containing the table of scenarios to the xml reader
 */
CTTableData* CTXmlDataParser::parseTable(const QString &tableName)
{
    CTTableData *tableData = new CTTableData(tableName);

    QXmlStreamReader reader(file);
    QList<QStringList> _table;
    QList <QPair<QString,QString> > _fields;
    QPair<QString,QString> _field;
    QHash<QString, QString> _constraints;
    QStringList _record;
    int num_row = 0;

    while(!reader.atEnd())
    {
        reader.readNext();

        /*When a new block is found*/
        if(reader.isStartElement() && reader.name() == "table")
        {
            if(tableName != reader.attributes().value("name").toString())
            {
                qDebug() << "Table not recognized!";
                return new CTTableData("");
            }
        }
        if(reader.isStartElement() && reader.name() == "field")
        {
            _field.first = reader.attributes().value("name").toString();
            _field.second = reader.attributes().value("type").toString();
            _fields.append(_field);
        }

        if(reader.isStartElement() && reader.name() == "constraint")
        {
            /*_constraints['fieldname'] = type_of_constraint*/
            _constraints[reader.attributes().value("name").toString()] =
                    reader.attributes().value("type").toString();
        }

        if(reader.isEndElement() && reader.name() == "fields")
        {
            tableData->field_names = _fields;
        }
        if(reader.isEndElement() && reader.name() == "constraints")
        {
            tableData->constraints= _constraints;
        }
        /*
         *The characters in the xml are read in the following sequence:
         *id,execution_day,execution_order,creation_date,last_edited,
         *description, xml_description
         */
        if(reader.isCharacters())
        {
            if(!reader.text().toString().trimmed().isEmpty())
            {
                _record.append(reader.text().toString());
            }
        }

        if(reader.isEndElement() && reader.name() == "row")
        {
            _table.append(_record);
            _record.clear();
            num_row ++;
        }
    }
    tableData->numRows = num_row;
    tableData->numColumns = 7;
    tableData->data = _table;
    return tableData;
}


void CTXmlDataParser::execParsedQuery(const QString &parsedQuery)
{
    qDebug() << parsedQuery;
}


CTTableData::CTTableData(const QString &tableName)
    :table_name(tableName)
{
}

CTRowData::CTRowData()
{
}
