#ifndef CT_DATA_H
#define CT_DATA_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>

class CTDBConn : public QObject
{
    Q_OBJECT
public:
    explicit CTDBConn(QObject *parent = 0);
    bool initializeDB(QString username, QString password);
    void closeConnection();
    //bool show();
    QSqlDatabase db;
    QString patientsId;
    QString idScenario;

    QSqlDatabase getConnection();
    QString getWorkLog(QStringList sessionData);
    QString getPatientsId();
    QSqlRecord getPatientsData(QString id);
    QString getIdScenario();
    QStringList getScenarios(QString id);
    QString getLastScenarioId();
    QString getScenarioXML(QString id_scenario);
    QStringList getListofPatientsIds();

    void insertUser(QHash<QString,QString> newUser);
    void insertQ(const QStringList &stringList, const QString &addworklog);
    bool savePatientsData(QHash<QString, QString> hashValuesLineEdit);
    bool deletePatient(QString id);
    bool savePatient(QHash<QString, QString> hashValuesLineEdit);
    bool saveScenario(QHash <QString,QString> hashValuesScenario);
    void setPatientsId(QString id);
    bool updateScenario(QHash <QString,QString> hashValuesScenario);
    bool deleteScenario(QString patientId, QString scenario_id);



    bool saveNewWorkLog(QString id_user,QHash<QString,QString> worklogToSave);
    bool updateWorklog(QHash<QString, QString> worklogToUpdate);
    QMap<QPair<QString, QString>, QString> getWorkLogs(QString id_user);
    QStringList getScenariosForPatient(QString id_patient);

private:

signals:
//    void userAuthorized(const QString &id_user,const QString &firstName,const QString &lastName);
    
public slots:
    QHash<QString, QString> authenticate(const QString &nickname, const QString &password);

};

#endif // CT_DATA_H
