#ifndef CT_XMLPARSER_H
#define CT_XMLPARSER_H


#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

class CTTableData;
class CTRowData;

class CTXmlDataParser
{
public:
    CTXmlDataParser();

    void initialize();
    CTTableData *parseTable(const QString &tableName);

    static void execParsedQuery(const QString &parsedQuery);

private:
    QFile* file;
};

class CTTableData
{
public:
    CTTableData(const QString &tableName);

    QString table_name;
    int numRows;
    int numColumns;
    QList<QStringList> data;
    QList <QPair<QString,QString> > field_names;
    QHash<QString,QString> constraints;
};

//TOBE implemented for a better local structure of the DB table
class CTRowData
{
public:
    CTRowData();
};

#endif // CT_XMLPARSER_H
