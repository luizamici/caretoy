#include "ct_sqltable.h"
#include <QSqlRecord>

CTSqlTable::CTSqlTable(QWidget *parent) :
    QTableView(parent)
{
    p_logger = Log4Qt::Logger::logger("CTSqlTable");
    p_logger->info("Table model of scenarios being created...");

    db = new CTDBConn();
    tableModel = new CTSqlModel(this->parentWidget(),db->getConnection());
    tableModel->setTable("test_scenario");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    /*Selecting columns the desired columns of the DB table in the desired order*/
    QSqlQuery query(tr("SELECT id,execution_day,execution_order,creation_date, "
                       "last_edited,description,xml_description "
                       "FROM test_scenario"),db->getConnection());
    tableModel->setQuery(query);
//    tableModel->select();

    tableModel->setHeaderData(1,Qt::Horizontal,tr("Execution day"));
    tableModel->setHeaderData(2,Qt::Horizontal,tr("Execution order"));
    tableModel->setHeaderData(3,Qt::Horizontal,tr("Creation date"));
    tableModel->setHeaderData(4,Qt::Horizontal,tr("Last edited"));
    tableModel->setHeaderData(5,Qt::Horizontal,tr("Description"));

    this->setModel(tableModel);

    /*disabling the editing through the table view*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /*enablig the sorting of the table by clicked column*/
    this->setSortingEnabled(true);

    this->horizontalHeader()->resizeSections(QHeaderView :: ResizeToContents);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->hide();

    /*columns obtained by the DB's table 'patient' which are not wished to be viewd */
    this->setColumnHidden(tableModel->fieldIndex("id"),true);
    this->setColumnHidden(tableModel->fieldIndex("xml_description"), true);

    /*activates selection of whole row*/
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*initialize sorting by execution day*/
    this->sortByColumn(1,Qt::AscendingOrder);
    p_logger->info("Table model of scenarios ready!");
}

bool CTSqlTable::save(QHash<QString, QString> scenario)
{
    if(scenario["id"] == ""){
        QSqlRecord newScenario = tableModel->record();

        foreach(QString key,scenario.keys()){
            newScenario.setValue(key,scenario[key]);
        }
        insertIntoTable(newScenario);
        return true;
    }else{
        QModelIndex index = getIndex(scenario["id"]);
        int row = index.row();
        foreach(QString key, scenario.keys()){
            int fieldIndex = tableModel->fieldIndex(key);
            QModelIndex indexOfCell = tableModel->index(row, fieldIndex);
            /*
             *The changes are saved locally in the SQL model
             */
            tableModel->setData(indexOfCell,scenario[key],Qt::EditRole);
        }
        submitAll();
        return true;
    }
    return false;
}

/*Inserts a new record in the table*/
void CTSqlTable::insertIntoTable(QSqlRecord scenarioRecord)
{
    /*removing id from the record since the DBMS should set it*/
    scenarioRecord.remove(0);
    tableModel->insertRecord(tableModel->rowCount(QModelIndex()),
                             scenarioRecord);

    /*Putting attention on the inserted record*/
    selectRow(tableModel->rowCount(QModelIndex()) - 1);
    submitAll();
}


void CTSqlTable::copyIntoTable(QSqlRecord scenarioRecord)
{
    /*adding the (copy) suffix to the description*/
    scenarioRecord.setValue("description",scenarioRecord.value("description").
                            toString() + "(copy)");
    insertIntoTable(scenarioRecord);
}

void CTSqlTable::deleteFromTable(QString id_scenario)
{
    QModelIndex index = getIndex(id_scenario);
    int row = index.row();
    if(tableModel->index(row,0).isValid())
        tableModel->removeRows(row,1);
    submitAll();
}

/*retreives the selected as hashmap, comfortable as input for other windows*/
QHash<QString,QString> CTSqlTable::getSelected()
{
    QSqlRecord scenarioRecord = getSelectedRecord();
    /*
     *Preparing the selected patient's data as hashMap for then returning it
     */
    QHash<QString,QString> scenario;

    scenario["id"] = scenarioRecord.value("id").toString();
    scenario["execution_day"] = scenarioRecord.value("execution_day").toString();
    scenario["execution_order"] = scenarioRecord.value("execution_order").toString();
    scenario["description"] = scenarioRecord.value("description").toString();
    scenario["xml_description"] = scenarioRecord.value("xml_description").toString();
    return scenario;
}


/*retreives the selected as sqlRecord for copying it*/
QSqlRecord CTSqlTable::getSelectedRecord()
{
    /*
     *Getting the selected row's index through the QTableView::currentIndex()
     *function, which returns an internal index,different from the one in the DB
     */
    QModelIndex index = currentIndex();
    int row = index.row();

    /*
     *Getting the selected row of the table model
     */
    QSqlRecord scenarioRecord = tableModel->record(row);
    return scenarioRecord;
}

QModelIndex CTSqlTable::getIndex(QString id_scenario)
{
    QModelIndex index;
    for(int i = 0; i < tableModel->rowCount(); i++)
    {
        if(tableModel->data(tableModel->index(i,0)).toString() == id_scenario)
        {
            index = tableModel->index(i,0);
        }
    }
    return index;
}


/*submits all cached changes*/
bool CTSqlTable::submitAll()
{
    tableModel->database().transaction();

    if(tableModel->submitAll()){
        tableModel->database().commit();
        emit success();
        return true;}
    else{
        tableModel->database().rollback();
        emit error("An error occurred: " + tableModel->lastError().text());
        return false;
    }
}
