#include "ct_scenariosadmin.h"
#include "ct_tablerecord.h"
#include "CareToy_Admin/ct_defs.h"

CTScenariosAdmin::CTScenariosAdmin(QObject *parent) :
    QObject(parent)
{
}


void CTScenariosAdmin::initialize()
{
    view = new CTViewOfScenarios();
    config = new CTScenarioConfig();

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
    view->statusBar->showMessage("Connection to server established ", 5000);
    requestTable();
}


void CTScenariosAdmin::execParsedQuery(QString query_type, QString initStmt,
                              QString whereStmt)
{
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
    if(query_type != "select")
        requestTable();
}


void CTScenariosAdmin::proccessData(QByteArray table_data)
{
    view->init(CTXmlDataParser::parse_table(table_data));
}


/*Select on pre-known columns of the table test_scenario*/
void CTScenariosAdmin::requestTable()
{
    QStringList fieldNames = QStringList() <<"id" << "execution_day"
                                          << "execution_order"
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
