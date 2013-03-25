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
    connect(ui->treeofworklogs, SIGNAL(clicked(QModelIndex)), this,
            SLOT(showLog(QModelIndex)));
}


CTWorklogsWidget::~CTWorklogsWidget()
{
    delete ui;
}


void CTWorklogsWidget::init(QByteArray table_data)
{
    ui->treeofworklogs->init(table_data);
}


void CTWorklogsWidget::showLog(QModelIndex index)
{
    QStringList _log= ui->treeofworklogs->getWorklog(index);
    idLog = _log.at(1);
    currentLog = _log.at(0);
    ui->qte_worklogedit->setText(currentLog);
    readMode();
}


void CTWorklogsWidget::on_qtb_saveButton_clicked()
{
    if(currentLog != ui->qte_worklogedit->toPlainText() && !idLog.trimmed().isEmpty())
    {
        QStringList stmt = ui->treeofworklogs->save(ui->qte_worklogedit->toPlainText(), idLog);
        execParsedQuery(stmt.at(0), stmt.at(1));
    }
    else if(currentLog != ui->qte_worklogedit->toPlainText() && idLog.trimmed().isEmpty())
    {
        QStringList stmt = ui->treeofworklogs->saveNew(ui->qte_worklogedit->toPlainText());
        execParsedQuery(stmt.at(0), stmt.at(1));
    }
    else
        nothingToSave("No changes to save.");
}

void CTWorklogsWidget::on_qtb_editButton_clicked(){editMode();}

void CTWorklogsWidget::on_qtbNewButton_clicked(){ newLogMode();}

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
