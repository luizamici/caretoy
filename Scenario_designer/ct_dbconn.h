#ifndef CT_DBCONN_H
#define CT_DBCONN_H

#include <QObject>
#include <QSqlDatabase>
#include <QHash>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

class CTDBConn : public QObject
{
    Q_OBJECT
public:
    explicit CTDBConn(QObject *parent = 0);

    void insertScenario(QHash<QString,QString> scenario);
    QSqlDatabase db;
    QSqlDatabase getConnection();

//    void checkScenarioTable();
    
signals:
    
public slots:
    
};

#endif // CT_DBCONN_H
