#include "ct_viewofscenarios.h"

CTViewOfScenarios::CTViewOfScenarios(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout *mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    this->setFixedSize(1000, 400);

    CTXmlDataParser *parser = new CTXmlDataParser();
    parser->initialize();
    CTTableData *table_data = parser->parseTable("test_scenario");
    xmlTable = new CTTableModel(table_data, this->parentWidget());
    table = new QTableView();
    table->setModel(xmlTable);

    /*disabling the editing through the table view*/
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();

    /*activates selection of whole row*/
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*columns which are not wished to be viewd */
    table->setColumnHidden(6, true); // xml_description hidden

    mainLayout->addWidget(table);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);

    QPushButton *edit = new QPushButton(tr("Edit"));
    QPushButton *add = new QPushButton(tr("Add"));
    QPushButton *copy = new QPushButton(tr("Copy"));
    QPushButton *remove = new QPushButton(tr("Remove"));
    buttonLayout->addWidget(edit);
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(copy);
    buttonLayout->addWidget(remove);
    buttonLayout->addStretch(1);

    connect(edit, SIGNAL(clicked()),this, SLOT(on_edit_clicked()));
    connect(add, SIGNAL(clicked()),this, SLOT(on_add_clicked()));
    connect(copy, SIGNAL(clicked()),this, SLOT(on_copy_clicked()));
    connect(remove, SIGNAL(clicked()),this, SLOT(on_remove_clicked()));

    /*Alternative way for editing a scenario*/
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_edit_clicked()));

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
    if(!selected.indexes().isEmpty()){tableSelected = true;}
    else{tableSelected = false;}
}

void CTViewOfScenarios::on_edit_clicked()
{
    if(tableSelected)
        editScenario(getSelected());
}

void CTViewOfScenarios::on_add_clicked()
{
    newScenario();
}

void CTViewOfScenarios::on_copy_clicked()
{
    if(tableSelected)
    {
        QModelIndex index = table->currentIndex();
        xmlTable->copyRecord(index);
    }
}

void CTViewOfScenarios::on_remove_clicked()
{
    if(tableSelected)
    {
        QModelIndex index = table->currentIndex();
        xmlTable->deleteRecord(index);
    }
}


QHash<QString,QString> CTViewOfScenarios::getSelected()
{
    QModelIndex index = table->currentIndex();
    return xmlTable->record(index);
}


void CTViewOfScenarios::save(QHash<QString, QString> scenario)
{
    xmlTable->save(scenario);
}
