#include "ct_sqltable.h"
#include <QSqlRecord>

CTSqlTable::CTSqlTable(QWidget *parent) :
    QTableView(parent)
{
    db = new CTDBConn();
    tableModel = new CTSqlModel(this->parentWidget(),db->getConnection());
    tableModel->setTable("test_scenario");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    /*Independent select from the column order of the db*/
    QSqlQuery query(tr("SELECT id,execution_day,execution_order,creation_date,last_edited,description,xml_description "
                       "FROM test_scenario ORDER BY execution_day"),db->getConnection());
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
//    this->setSortingEnabled(true);
    this->horizontalHeader()->resizeSections(QHeaderView :: ResizeToContents);
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->hide();

    /*columns obtained by the DB's table 'patient' which are not wished to be viewd */
    this->setColumnHidden(tableModel->fieldIndex("id"),true);
    this->setColumnHidden(tableModel->fieldIndex("xml_description"), true);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CTSqlTable::save(QHash<QString, QString> scenario)
{
    if(scenario["id"] == ""){
        QSqlRecord newScenario = tableModel->record();

        foreach(QString key,scenario.keys()){
            newScenario.setValue(key,scenario[key]);
        }
        /*removing id from the record since the DBMS should set it*/
        newScenario.remove(0);

        tableModel->insertRecord(tableModel->rowCount(QModelIndex()),newScenario);
    }else{
        QModelIndex index;
        int row;
        for(int i = 0; i < tableModel->rowCount(); i++)
        {
            if(tableModel->data(tableModel->index(i,0)).toString() == scenario["id"])
            {
                index = tableModel->index(i,0);
                row = index.row();
            }
         }
         foreach(QString key, scenario.keys()){
            int fieldIndex = tableModel->fieldIndex(key);
            index = tableModel->index(row, fieldIndex);
                    /*
                     *The changes are saved locally in the SQL model
                     */
            tableModel->setData(index,scenario[key],Qt::EditRole);
          }
    }
}

QHash<QString,QString> CTSqlTable::getSelected()
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
    /*
     *Preparing the selected patient's data for then returning it
     */
    QHash<QString,QString> scenario;

    scenario["id"] = scenarioRecord.value("id").toString();
    scenario["execution_day"] = scenarioRecord.value("execution_day").toString();
    scenario["execution_order"] = scenarioRecord.value("execution_order").toString();
    scenario["description"] = scenarioRecord.value("description").toString();
    scenario["xml_description"] = scenarioRecord.value("xml_description").toString();

    return scenario;
}

void CTSqlTable::submitAll()
{
    tableModel->database().transaction();

    if(tableModel->submitAll()){
        tableModel->database().commit();
        qDebug() << "SUCCESS";}
    else{
        tableModel->database().rollback();
        qDebug() << "there was an error " << tableModel->lastError().text();
    }
}
