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

    connect(view, SIGNAL(execParsedQuery(QString,QString)), this, SLOT(
                execParsedQuery(QString,QString)));

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
            this, SLOT(storeImage(QHash<QString,QString>)));
    connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                    config,SLOT(close()));
    view->statusBar->showMessage("Connection to server established ", 5000);
    requestTable();
}


void CTScenariosAdmin::storeImage(QHash<QString, QString> scenario)
{

//    QString statement;
//    QXmlStreamWriter stream(&statement);
//    stream.setAutoFormatting(true);

//    stream.writeStartElement("insert_image");
//    stream.writeStartElement("id_scenario");
//    stream.writeCharacters(scenario["id"]);
//    stream.writeEndElement();// end id_scenario
//    stream.writeStartElement("scenario_image");
//    QString data_base64;
//    QFile *file = new QFile(scenario["image_description"]);
//    if (file->open(QIODevice::ReadOnly))
//    {
//        QByteArray data = file->readAll();
//        data_base64 = data.toBase64();
//    }
//    stream.writeCharacters(data_base64);
//    stream.writeEndElement();// end scenario_image
//    stream.writeEndElement();//end insert_image
//    qDebug() << statement;
//    if(!data_base64.isEmpty())
//        requestToWriteIntoSocket(statement, CT_DBSDATA);
}

void CTScenariosAdmin::execParsedQuery(QString initStmt, QString whereStmt)
{
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
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
                                          <<  "description" << "image_description"
                                           << "xml_description" << "flag" ;

    CTTableRecord rec = CTTableRecord();
    int i =0;
    foreach(QString fieldName, fieldNames)
    {
        rec.insert(i, CTTableField(fieldName, fieldName));
        i++;
    }
    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
                                               "test_scenario",rec);
    execParsedQuery(stmt, QString());
    showMessage("Retreiving table from DB...");
}

void CTScenariosAdmin::showMessage(QString mssg)
{
    view->statusBar->showMessage(mssg, 5000);
}
