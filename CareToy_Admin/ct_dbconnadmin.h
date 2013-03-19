#ifndef CT_CONNADMIN_H
#define CT_CONNADMIN_H

#include <QObject>
#include <QStringList>
#include "ct_dbconn.h"

class CTDBConnAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTDBConnAdmin(QObject *parent = 0);
    CTDBConn *conn;

    QSqlDatabase getDBConnection();
    void closeDBConnection();
    QMap<QPair<QString,QString>,QString> getWorkLogs(QString id_user);
    QStringList getScenariosForPatient(QString id_patient);
private:

    QHash<QString,QString> userCredentials;
    QString gen_random(const int len);

signals:
    void userAuthorized(QHash<QString,QString> userCredentials);
    void authorizationFailed();
    void refreshStaffModule(QString submodule);
public slots:
    void authenticate(const QString &nickname, const QString &password);
    void saveNewLog(QHash<QString,QString> newLogToSave);
    void updateLog(QHash<QString,QString> worklogToUpdate);
    void insertUser(QSqlRecord& rec);

};

#endif // CT_CONNADMIN_H
