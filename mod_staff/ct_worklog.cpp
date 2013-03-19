#include "ct_worklog.h"

CTWorkLog::CTWorkLog(QStringList sessionData, QWidget *parent) :
    QTextEdit(parent)
{
    openSessionData = sessionData;
    stringWorklog = conn->getWorkLog(openSessionData);
    this->setText(stringWorklog);
    this->setReadOnly(true);
    this->setTextInteractionFlags(Qt::NoTextInteraction);
    QString text =this->toPlainText();
    stringlistLog = text.split("*", QString::SkipEmptyParts);
    refreshLists();
}

/*Method called for the dates substring extraction, during the initalization
of the class end during the refresh.*/

void CTWorkLog::refreshLists(){

    dates.clear();
    logs.clear();
    for(int i=1; i < stringlistLog.size(); i = i + 2 ){
        dates.append(stringlistLog.at(i));
    }
    for(int i= 2; i <stringlistLog.size(); i=i+2){
       logs.append(stringlistLog.at(i));
   }
}

void CTWorkLog::refresh(){

    stringWorklog = conn->getWorkLog(openSessionData);
    this->setText(stringWorklog);
    QString text =this->toPlainText();
    stringlistLog.clear();
    stringlistLog = text.split("*", QString::SkipEmptyParts);
    refreshLists();
    sendDates();
}

void CTWorkLog::sendDates(){
    emitDates(dates);
}


void CTWorkLog::sendLog(const int &itemIndex){

    int index ;
    if(itemIndex > 2){
        index = itemIndex - 3;
        emitLog(logs.at(index), index);
    }else{
        emitLog(" ", index);
    }
}

void CTWorkLog::saveChanges(const QString &changedText, int index){

    QString oldLog = logs.at(index).trimmed();
    QString newLog = changedText.trimmed();
    qDebug() << stringWorklog.count(oldLog);
    if(stringWorklog.count(oldLog) == 1 ){
         stringWorklog = stringWorklog.replace(oldLog,newLog);
         if(conn->updateWorklog(stringWorklog,openSessionData)){
             emit worklogChanged();}
         else{emit error();}
    }else{
        emit doNothing();
        //TODO
    }
}




