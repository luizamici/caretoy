#ifndef CT_XMLPARSER_H
#define CT_XMLPARSER_H


#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>


class CTXmlParser
{
public:
    CTXmlParser();

    void initialize();
    QList<QStringList> parseTable();

    int getNumRows(){return numRows;}
    int getNumColumns(){return numColumns;}
    static void execParsedQuery(const QString &parsedQuery);

private:
    QFile* file;
    int numRows;
    int numColumns;
};

#endif // CT_XMLPARSER_H
