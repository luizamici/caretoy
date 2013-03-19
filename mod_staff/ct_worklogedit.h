#ifndef CT_WORKLOGEDIT_H
#define CT_WORKLOGEDIT_H

#include <QWidget>
#include <QtGui>
#include "ct_worklog.h"

class CTWorkLogEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CTWorkLogEdit(QWidget *parent = 0);
    int indexofLog;
    QString initialLog;
    CTWorkLog *worklog;
    
signals:
    void newLog(const QString changedText, int index);
    void doNothing();
    
public slots:
    void update(const QString &log, int index);
    void checkForChanges();
};

#endif // CT_WORKLOGEDIT_H
