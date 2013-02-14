#ifndef CT_ADMIN_H
#define CT_ADMIN_H

#include <QObject>
#include <QDebug>
#include <QTimer>

class CTSslClient;
class CTXmlDataParser;
class CTViewOfScenarios;
class CTScenarioConfig;
class CTSslClientThread;

class CTAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTAdmin(QObject *parent = 0);

private:
    void requestTable();

    CTSslClient *sslClient;
    CTSslClientThread *sslClientThread;
    CTXmlDataParser *dataParser;
    CTViewOfScenarios *view;
    CTScenarioConfig *config;

    int timeOuts;

public slots:
    void initialize();
    void execParsedQuery(QString query_type, QString initStmt, QString whereStmt);
    void proccessData(QByteArray table_data);
    void connectionLost(QString mss);

private slots:
    void timerTimeout();

};

#endif // CT_ADMIN_H
