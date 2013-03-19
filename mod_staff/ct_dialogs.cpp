#include "ct_dialogs.h"
#include "ui_ct_dialogs.h"


CTDialogs::CTDialogs(QStringList stringList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTDialogs)
{

    privateStringList = stringList;
    ui->setupUi(this);
    ui->dateTimeEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setTime(QTime::currentTime());

    connect(ui->cancelButton, SIGNAL(clicked()), this , SLOT(close()));
    connect(ui->okButton, SIGNAL(clicked()), this , SLOT(addLog()));
}

void CTDialogs::addLog(){

    conn= new CTDBConn();
    QString date;
    QString worklog;
    date = ui->dateTimeEdit->text();
    worklog = "<br> * " +date +" * </br><br></br><br> "+ ui->textEdit->toPlainText() + "</br><br></br>";
    conn->insertQ(privateStringList, worklog);
    emit logChanged();
    this->close();

}

CTDialogs::~CTDialogs()
{
    delete ui;
}
