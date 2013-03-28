#include "ct_worklogswidget.h"
#include "ui_ct_worklogswidget.h"

CTWorklogsWidget::CTWorklogsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTWorklogsWidget)
{
    ui->setupUi(this);

    QFont serifFont("Serif", 12, QFont::Normal);
    ui->cle_worklogs->setFont(serifFont);

    currentLog = "";
    idLog = "";
    connect(ui->qtb_treeview, SIGNAL(clicked(QModelIndex)), this,
            SLOT(showLog(QModelIndex)));
    connect(ui->qtb_contract_all, SIGNAL(clicked(bool)),this, SLOT(
                contractButtonToggled(bool)));

    connect(ui->qtb_calendarview, SIGNAL(clicked(QDate)), this, SLOT(
                on_qtb_calendarview_clicked(QDate)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(
                tabChanged(int)));
}


CTWorklogsWidget::~CTWorklogsWidget()
{
    delete ui;
}


void CTWorklogsWidget::init(QByteArray table_data)
{
    ui->qtb_treeview->init(table_data);
}

void CTWorklogsWidget::tabChanged(int id)
{
    if(id = 1)
        ui->qtb_calendarview->init(ui->qtb_treeview->getParentNodes());
}

void CTWorklogsWidget::showLog(QModelIndex index)
{
    QStringList _log= ui->qtb_treeview->getWorklog(index);
    idLog = _log.at(1);
    currentLog = _log.at(0);
    ui->qte_worklogedit->setText(currentLog);
    readMode();
}

void CTWorklogsWidget::showLogByDate(QDate date_selected)
{

}

void CTWorklogsWidget::contractButtonToggled(bool toggled)
{
    if(true == toggled)
    {
        ui->qtb_contract_all->setText("Expand all");
        ui->qtb_treeview->collapseAll();
    }
    else
    {
        ui->qtb_contract_all->setText("Collapse all");
        ui->qtb_treeview->expandAll();
    }
}

void CTWorklogsWidget::on_qtb_saveButton_clicked()
{
    if(currentLog != ui->qte_worklogedit->toPlainText() &&
            !idLog.trimmed().isEmpty())
    {
        QStringList stmt = ui->qtb_treeview->save(
                    ui->qte_worklogedit->toPlainText(), idLog);
        execParsedQuery(stmt.at(0), stmt.at(1));
    }
    else if(currentLog != ui->qte_worklogedit->toPlainText() &&
            idLog.trimmed().isEmpty())
    {
        QStringList stmt = ui->qtb_treeview->saveNew(
                    ui->qte_worklogedit->toPlainText());
        execParsedQuery(stmt.at(0), stmt.at(1));
    }
    else
        nothingToSave("No changes to save.");
}

void CTWorklogsWidget::on_qtb_editButton_clicked(){editMode();}

void CTWorklogsWidget::on_qtbNewButton_clicked(){ newLogMode();}


void CTWorklogsWidget::on_qtbDeleteButton_clicked()
{
    QStringList stmt = ui->qtb_treeview->deleteLog(idLog);
    execParsedQuery(stmt.at(0), stmt.at(1));
}

void CTWorklogsWidget::on_qtb_calendarview_clicked(const QDate &date)
{
        QStringList _log= ui->qtb_treeview->getWorklogByDate(date);
        if(!_log.isEmpty())
        {
            idLog = _log.at(1);
            currentLog = _log.at(0);
            ui->qte_worklogedit->setText(currentLog);
            readMode();
        }
}

void CTWorklogsWidget::readMode()
{
    /*
     *setting the editor read-only
     */
    ui->qte_worklogedit->setReadOnly(true);
    /*
     *changing the background color
     */
    QPalette p = ui->qte_worklogedit->palette();
    p.setColor(QPalette::Base, QColor(240, 240, 255,30));
    ui->qte_worklogedit->setPalette(p);
}

void CTWorklogsWidget::editMode()
{
    /*
     *setting the editor writable
     */
    ui->qte_worklogedit->setReadOnly(false);
    /*
     *putting focus on the editor
     */
    ui->qte_worklogedit->setFocus();
    /*
     *changing the background color
     */
    QPalette p = ui->qte_worklogedit->palette();
    p.setColor(QPalette::Base, QColor(255, 255, 255));
    ui->qte_worklogedit->setPalette(p);
}

void CTWorklogsWidget::newLogMode()
{
    ui->qte_worklogedit->clear();
    idLog.clear();
    editMode();
}
