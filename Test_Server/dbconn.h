#ifndef DBCONN_H
#define DBCONN_H

#include <QObject>
#include <QSqlDatabase>

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>

#include <QVariant>

#include "datatoxml.h"

class DBConn : public QObject
{
    Q_OBJECT
public:
    explicit DBConn(QObject *parent = 0);

    QSqlDatabase db;
    DataToXml *dataToXml;
    
signals:
    void requestToWrite(QString output_data);
public slots:
    bool exec(QHash<QString, QVariant> query);
};

#endif // DBCONN_H
