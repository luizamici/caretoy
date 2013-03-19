#include "ct_worklogedit.h"

CTWorkLogEdit::CTWorkLogEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

void CTWorkLogEdit::update(const QString &log, int index){

    indexofLog = index;
    initialLog = log;
    this->clear();
    this->setText(log);
}

void CTWorkLogEdit::checkForChanges(){

    if (this->toPlainText().compare(initialLog) == 0 ){ qDebug() << "text not changed!"; emit doNothing();}
    else{ emit newLog(this->toPlainText(),indexofLog);}
}
