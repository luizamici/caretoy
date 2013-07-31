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
//    connect(view,SIGNAL(editScenario(QHash<QString,QString>)),config, SLOT(show()));
    connect(view, SIGNAL(editScenario()), config,SLOT(openScenario()));
    connect(view, SIGNAL(editScenario()), config,SLOT(show()));

    connect(view, SIGNAL(newScenario()), config, SLOT(show()));
//    connect(view, SIGNAL(newScenario()), config->scenarioCanvas,
//            SLOT(clear()));
    connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                    view,SLOT(save(QHash<QString,QString>)));
    connect(config->scenarioCanvas,SIGNAL(save(QHash<QString,QString>)),
                    config,SLOT(close()));
    view->statusBar->showMessage("Connection to server established ", 5000);
    requestTable();
}


void CTScenariosAdmin::execParsedQuery(QString initStmt, QString whereStmt)
{
    emit requestToWriteIntoSocket(CTQueryParser::prepareQuery(
                                      initStmt,whereStmt), CT_DBSDATA);
}


void CTScenariosAdmin::proccessData(QByteArray table_data)
{
    view->setEnabled(true);
    showMessage("Information retreived successfully.");
    view->init(CTXmlDataParser::parse_table(table_data));
}


/*Select on pre-known columns of the table test_scenario*/
void CTScenariosAdmin::requestTable()
{
    view->setEnabled(false);
    QStringList fieldNames = QStringList() <<"id"
                                          << "creation_date" << "last_edited"
                                          <<  "description"
                                          << "training_day"
                                          << "execution_order" << "image_description"
                                          << "xml_description" << "flag" << "position_image" ;

    QString stmt;
    QXmlStreamWriter writer(&stmt);
    writer.setAutoFormatting(true);

    writer.writeStartElement("select");
    writer.writeStartElement("table");
    writer.writeAttribute("name", "test_scenario");
    writer.writeStartElement("fields");
    writer.writeAttribute("number", QString::number(fieldNames.count()));

    foreach(QString fieldName, fieldNames)
    {
        writer.writeStartElement("field");
        writer.writeAttribute("name",fieldName);
        writer.writeEndElement(); // end field
    }

    writer.writeEndElement(); // end fields
    writer.writeEndElement(); // end table
    writer.writeEndElement(); // end select


//    CTTableRecord rec = CTTableRecord();
//    int i =0;
//    foreach(QString fieldName, fieldNames)
//    {
//        rec.insert(i, CTTableField(fieldName));
//        i++;
//    }
//    QString stmt = CTQueryParser::xmlStatement(CTQueryParser::SelectStatement,
//                                               "test_scenario",rec);
//    qDebug() << Q_FUNC_INFO << stmt;

    execParsedQuery(stmt, QString());
    view->statusBar->showMessage("Retreiving table from DB. Please wait...");
}

void CTScenariosAdmin::showMessage(QString mssg)
{
    view->statusBar->showMessage(mssg, 5000);
}
