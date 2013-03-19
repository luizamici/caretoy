#ifndef DATATOXML_H
#define DATATOXML_H

#include <QObject>

#include <QDebug>

#include <QSqlRecord>
#include <QSqlResult>
#include <QXmlStreamWriter>

class DataToXml : public QObject
{
    Q_OBJECT
public:
    explicit DataToXml(QObject *parent = 0);
    
    static QString tableToXml(QString tableName,QList<QSqlRecord> tableInfo,
                              QList<QSqlRecord> recs);
    
};

#endif // DATATOXML_H
