#include "ct_viewofscenarios.h"

CTViewOfScenarios::CTViewOfScenarios(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout *mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    this->setFixedSize(1000, 400);

    CTXmlParser *parser = new CTXmlParser();
    parser->initialize();
    QList<QStringList> data = parser->parseTable();
    xmlTable = new CTXMLTableOfScenarios(parser->getNumRows(),
                                          parser->getNumColumns(),
                                          data);
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
}


void CTViewOfScenarios::on_edit_clicked()
{
     editScenario(getSelected());
}

void CTViewOfScenarios::on_add_clicked()
{
    newScenario();
}

void CTViewOfScenarios::on_copy_clicked()
{}

void CTViewOfScenarios::on_remove_clicked()
{}


QHash<QString,QString> CTViewOfScenarios::getSelected()
{
    QModelIndex index = table->currentIndex();
    QStringList record = xmlTable->record(index).toStringList();

    QHash<QString,QString> scenario;

    scenario["id"] = record.at(0);
    scenario["execution_day"] = record.at(1);
    scenario["execution_order"] = record.at(2);
    scenario["description"] = record.at(5);
    scenario["xml_description"] = record.at(6);
    return scenario;
}
