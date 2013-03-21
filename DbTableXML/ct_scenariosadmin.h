#ifndef CT_CTSCENARIOSADMIN_H
#define CT_CTSCENARIOSADMIN_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "ct_xmldataparser.h"
#include "ct_viewofscenarios.h"
#include "Scenario_Designer/ct_scenarioconfig.h"


class CTAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTAdmin(QObject *parent = 0);

    CTXmlDataParser *dataParser;
    CTViewOfScenarios *view;
    CTScenarioConfig *config;

    void showMessage(QString mssg);
    void requestTable();

signals:
    void requestToWriteIntoSocket(const QString &parsedQuery,
                                  const quint32 &type);

public slots:
    void initialize();
    void execParsedQuery(QString initStmt, QString whereStmt);
    void proccessData(QByteArray table_data);

};

#endif // CT_CTSCENARIOSADMIN_H
