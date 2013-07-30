#include "ct_viewofscenarios.h"
#include "ct_scenariodata.h"

CTViewOfScenarios::CTViewOfScenarios(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setWindowTitle("CareToy Scenario Manager");
    this->setLayout(mainLayout);
    this->setFixedSize(1200, 500);

    table = new QTableView();

    /*disabling the editing through the table view*/
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();

    /*activates selection of whole row*/
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSortingEnabled(true);

    connect(table->horizontalHeader(),SIGNAL(sectionClicked(int)),this,
            SLOT(sortByColumn(int)));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    statusBar = new QStatusBar();
    mainLayout->addWidget(statusBar);

    edit = new QPushButton(tr("Edit"));
    QPushButton *add = new QPushButton(tr("Add"));
    copy = new QPushButton(tr("Copy"));
    remove = new QPushButton(tr("Remove"));
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(edit);
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(copy);
    buttonLayout->addWidget(remove);

    edit->setEnabled(false);
    remove->setEnabled(false);
    copy->setEnabled(false);

    connect(edit, SIGNAL(clicked()),this, SLOT(on_edit_clicked()));
    connect(add, SIGNAL(clicked()),this, SLOT(on_add_clicked()));
    connect(copy, SIGNAL(clicked()),this, SLOT(on_copy_clicked()));
    connect(remove, SIGNAL(clicked()),this, SLOT(on_remove_clicked()));

    /*Alternative way for editing a scenario*/
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(
                on_edit_clicked()));

    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2, desktop.height()/2 - this->height()/2);

    show();
}

void CTViewOfScenarios::init(CTTableData *table_data)
{
    xmlTable = new CTTableModel(table_data,this->parentWidget());
    connect(xmlTable, SIGNAL(execParsedQuery(QString,QString)),this,
            SIGNAL(execParsedQuery(QString,QString)));
    filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(xmlTable);
    table->setModel(filterModel);

    xmlTable->setHeader(0, "ID");
    xmlTable->setHeader(1,"Creation date");
    xmlTable->setHeader(2, "Last edited");
    xmlTable->setHeader(3, "Description");
    xmlTable->setHeader(4,"Execution day");
    xmlTable->setHeader(5,"Execution order");

    /*columns which are not wished to be viewd */
    table->setColumnHidden(6,true);
    table->setColumnHidden(7, true); // xml_description hidden
    table->setColumnHidden(8, true);
    table->setColumnHidden(9,true);

    /*
     *Signals are emitted if valid rows are selected from the table
     */
    connect(table->selectionModel(),SIGNAL(
                selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(tableSelectionChanged(QItemSelection,QItemSelection)));
}


void CTViewOfScenarios::tableSelectionChanged(const QItemSelection &selected,
                                              const QItemSelection &deselected)
{
    /*Checks if any rows on the table have been selected*/
    if(!selected.indexes().isEmpty())
    {
        edit->setEnabled(true);
        remove->setEnabled(true);
        copy->setEnabled(true);
        tableSelected = true;
    }
    else{
        edit->setEnabled(false);
        remove->setEnabled(false);
        copy->setEnabled(false);
        tableSelected = false;}
}


void CTViewOfScenarios::sortByColumn(int column)
{

}


void CTViewOfScenarios::on_edit_clicked()
{
    if(tableSelected)
    {
        QModelIndex index = filterModel->mapToSource(table->currentIndex());
        xmlTable->record(index);
        editScenario();
    }
}

void CTViewOfScenarios::on_add_clicked()
{
    newScenario();
}

void CTViewOfScenarios::on_copy_clicked()
{
    if(tableSelected)
    {
        QModelIndex index = filterModel->mapToSource(table->currentIndex());
        xmlTable->copyRecord(index);
    }
}

void CTViewOfScenarios::on_remove_clicked()
{
    if(tableSelected)
    {
        QModelIndex index = filterModel->mapToSource(table->currentIndex());
        xmlTable->deleteRecord(index);
    }
}


QHash<QString,QString> CTViewOfScenarios::getSelected()
{
//    QModelIndex index = filterModel->mapToSource(table->currentIndex());
//    return xmlTable->record(index);
}


void CTViewOfScenarios::save(QHash<QString,QString> scenario)
{
    xmlTable->save(scenario);
}
