#ifndef CT_DBCONN_H
#define CT_DBCONN_H

#include <QObject>
#include <QSqlDatabase>
#include <QHash>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

#include "ct_logger.h"

class CTDBConn : public QObject
{
    Q_OBJECT
public:
    explicit CTDBConn(QObject *parent = 0);

    QSqlDatabase db;
    QSqlDatabase getConnection();
    
signals:
    
public slots:
    
};

#endif // CT_DBCONN_H
