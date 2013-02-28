#include "ct_admin.h"
#include "ct_xmldataparser.h"
#include "ct_viewofscenarios.h"
#include "Scenario_Designer/ct_scenarioconfig.h"
#include "SecureSocketClient/ct_sslclient.h"
#include "SecureSocketClient/ct_sslclientthread.h"
#include "ct_tablerecord.h"

CTAdmin::CTAdmin(QObject *parent) :
    QObject(parent)
{
//    sslClient = new CTSslClient();
    view = new CTViewOfScenarios();

    sslClientThread  = new CTSslClientThread();
    sslClientThread->run();
    config = new CTScenarioConfig();

    timeOuts = 0;

    /*Connection between the sslClient and the local table of scenarios*/
    connect(sslClientThread,SIGNAL(notConnected(QString)),this, SLOT(
                connectionLost(QString)));
    connect(sslClientThread,SIGNAL(encryptionStarted()),this, SLOT(initialize()));
    connect(sslClientThread,SIGNAL(proccessData(QByteArray)),this,SLOT(
                proccessData(QByteArray)));


    connect(view, SIGNAL(execParsedQuery(QString,QString,QString)), this, SLOT(
                execParsedQuery(QString,QString,QString)));

    /*Connections between viewOfScenarios and the scenario designer*/
    connect(view,SIGNAL(editScenario(QHash<QString,QString>)),config, SLOT(show()));
    connect(view, SIGNAL(editScenario(QHash<QString,QString>)),
            config,SLOT(openScenario(QHash<QString,QString>)));
    connect(view, SIGNAL(editScenario(QHash<QString,QString>)), config,
            SLOT(show()));

    connect(view, SIGNAL(newScenario()), config, SLOT(show()));
    connect(view, SIGNAL(newScenario()), config->scenarioCanvas,
            SLOT(clear()));
    connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                    view,SLOT(save(QHash<QString,QString>)));
    connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                    config,SLOT(close()));
}


void CTAdmin::initialize()
{
    view->statusBar->showMessage("Connection to server established ", 5000);
    requestTable();
}


void CTAdmin::execParsedQuery(QString query_type, QString initStmt,
                              QString whereStmt)
{
    if(sslClientThread->isConnected())
    {
        if(query_type != "select" & sslClientThread->requestForWrite(
                    CTQueryParser::prepareQuery(initStmt,whereStmt)))
            requestTable();
    }
    else
    {
        QMessageBox::critical(0, tr("CareToy Admin"),
                                       tr("Not connected to server! "
                           "Local changes cannot be saved in the DB."));
    }
}


void CTAdmin::proccessData(QByteArray table_data)
{
    view->init(CTXmlDataParser::parse_table(table_data));
}


void CTAdmin::connectionLost(QString mss)
{
    int i = QMessageBox::critical(0, tr("CareToy Admin"),mss);
    view->statusBar->showMessage("Not connected to server. "
                                 "Trying to reconnect ...");
    qDebug() << i;
    QTimer::singleShot(5000, this, SLOT(timerTimeout()));
}

void CTAdmin::timerTimeout()
{
    qDebug() << "CTAdmin::timerTimeout";
    if(timeOuts < 5)
        sslClientThread->initialize();
    else
        view->statusBar->showMessage("Impossible to establish a connection "
                                     "with the server!");
    timeOuts++;
}

/*Select on pre-known columns of the table test_scenario*/
void CTAdmin::requestTable()
{
    qDebug() << "Requesting the last version of the table!";
    QStringList fieldNames = QStringList() <<"id" << "execution_day" << "execution_order"
                                          << "creation_date" << "last_edited"
                                          <<  "description" << "xml_description";

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "test_scenario",rec);
    execParsedQuery("select",stmt, QString());
}
