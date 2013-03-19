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
	~DBConn() {}

    QSqlDatabase db;
    DataToXml *dataToXml;

    void initialize();
    void openConnection();
    void closeConnection();
    QString authenticate(QHash<QString, QVariant> query);
    QString requestScenario(QHash<QString,QVariant> query);
    
signals:
    void requestToWrite(QString output_data);
public slots:
    QString exec(QHash<QString, QVariant> query);
};

#endif // DBCONN_H
