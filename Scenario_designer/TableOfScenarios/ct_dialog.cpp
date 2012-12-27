#include "ct_dialog.h"
#include "ui_ct_dialog.h"

CTDialog::CTDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Save scenario");
}

CTDialog::~CTDialog()
{
    delete ui;
}

void CTDialog::on_pushButton_clicked()
{
    emit accepted(ui->qle_description->text(),ui->qde_execution->text(),ui->qsb_execution->text());
    this->close();
}

void CTDialog::on_pushButton_2_clicked()
{
    this->close();
}
void CTDialog::clear()
{

}

void CTDialog::setData(QString description, QString execution_day, QString execution_order)
{

}
