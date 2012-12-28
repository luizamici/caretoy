#include "ct_dialog.h"
#include "ui_ct_dialog.h"

CTDialog::CTDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Save scenario");

    /*Center window in desktop*/
    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2,
               desktop.height()/2 - this->height()/2);
}

CTDialog::~CTDialog()
{
    delete ui;
}

/*If save button was clicked*/
void CTDialog::on_pushButton_clicked()
{
    emit accepted(ui->qle_description->text(),ui->qde_execution->text(),
                  ui->qsb_execution->text());
    this->close();
}

/*If cancel button was clicked*/
void CTDialog::on_pushButton_2_clicked()
{
    this->close();
}

/*Public function for setting data into fields*/
void CTDialog::setData(QString description, QString execution_day,
                       QString execution_order)
{
    ui->qle_description->setText(description);
    QDate date(QDate::fromString(execution_day,"yyyy-MM-dd"));
    ui->qde_execution->setDate(date);
    ui->qsb_execution->setValue(execution_order.toInt());
}
