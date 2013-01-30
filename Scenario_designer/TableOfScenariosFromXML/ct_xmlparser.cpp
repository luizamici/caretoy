#include "ct_xmlparser.h"

CTXmlParser::CTXmlParser()
{
}

/*
 *Open xml file containing the table of scenarios
 */
void CTXmlParser::initialize()
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
QList<QStringList> CTXmlParser::parseTable()
{
    QXmlStreamReader reader(file);
    QList<QStringList> table;
    QStringList record;
    int num_row = 0;

    while(!reader.atEnd())
    {
        reader.readNext();

        /*When a new block is found*/
        if(reader.isStartElement() && reader.name() == "table")
        {
            if("test_scenario" != reader.attributes().value("name").toString())
            {
                qDebug() << "Table not recognized!";
                return QList<QStringList>();
            }

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
                record.append(reader.text().toString());
            }
        }

        if(reader.isEndElement() && reader.name() == "row")
        {
            table.append(record);
            record.clear();
            num_row ++;
        }
    }
    numRows = num_row;
    numColumns = 7;
    return table;
}



