#include "ct_worklogswidget.h"
#include "ui_ct_worklogswidget.h"

CTWorklogsWidget::CTWorklogsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTWorklogsWidget)
{
    ui->setupUi(this);
}

CTWorklogsWidget::~CTWorklogsWidget()
{
    delete ui;
}

void CTWorklogsWidget::init(QByteArray table_data)
{
    ui->treeofworklogs->init(table_data);
}



void CTWorklogsWidget::on_qtb_saveButton_clicked()
{

}

void CTWorklogsWidget::on_qtb_editButton_clicked(){editMode();}

void CTWorklogsWidget::on_qtbNewButton_clicked(){ editMode();}

void CTWorklogsWidget::readMode()
{
    /*
     *setting the editor read-only
     */
    ui->qte_worklogedit->setReadOnly(true);
    /*
     *changing the background color
     */
    QPalette p2 = ui->qte_worklogedit->palette();
    p2.setColor(QPalette::Base, QColor(240, 240, 255,30));
    ui->qte_worklogedit->setPalette(p2);

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

