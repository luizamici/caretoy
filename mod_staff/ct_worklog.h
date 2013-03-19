#ifndef CT_WORKLOG_H
#define CT_WORKLOG_H

#include <QWidget>
#include <QtGui>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include "CareToy_Admin/ct_dbconn.h"

class CTWorkLog : public QTextEdit
{
    Q_OBJECT
public:
    explicit CTWorkLog(QStringList sessionData, QWidget *parent);
    QString getLog(const int &itemIndex);

    
signals:
    void emitDates(const QStringList &listOfDates);
    void emitLog(const QString &log, int index);
    void worklogChanged();
    void error();
    void doNothing();

private:
    CTDBConn *conn;
    QStringList openSessionData;
    QStringList stringlistLog;
    QStringList dates;
    QStringList logs;
    QString stringWorklog;
    void refreshLists();

public slots:
    void refresh();
    void sendDates();
    void sendLog(const int &itemIndex);
    void saveChanges(const QString &changedText, int index);
    
};

#endif // CT_WORKLOG_H
