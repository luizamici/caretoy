#include "ct_searchpatient.h"
#include "ui_ct_searchpatient.h"
#include <QDebug>

CTSearchPatient::CTSearchPatient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTSearchPatient)
{
    ui->setupUi(this);

    connect(ui->qcbSearchBy,SIGNAL(currentIndexChanged(int)),this, SLOT(search(int)));
    connect(ui->qleINputSearch,SIGNAL(textChanged(QString)),this,SIGNAL(searchFor(QString)));
}


void CTSearchPatient::search(int index){
    ui->qleINputSearch->setEnabled(true);
    emit searchBy(index);
}


CTSearchPatient::~CTSearchPatient()
{
    delete ui;
}
