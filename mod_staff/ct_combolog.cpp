#include "ct_combolog.h"
#include <QStyleOptionComboBox>

CTComboLog::CTComboLog(QWidget *parent) :
    QComboBox(parent)
{  
    this->insertItem(0,QIcon(":images/search_button.png"),"Search by date");
    this->setFixedWidth(180);
}

void CTComboLog::updateData(const QStringList &itemsList){

    this->clear();
    this->insertItem(0,QIcon(":images/search_button.png"),"Search by date");
    this->addItem("........................................... ");
    this->addItem(" ");
    this->addItems(itemsList);

}

void CTComboLog::focusInEvent(QFocusEvent *e){

    if(this->count() == 1){
        emit(comboSelected());
    }
}
