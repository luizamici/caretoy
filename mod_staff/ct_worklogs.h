#ifndef CT_WORKLOGS_H
#define CT_WORKLOGS_H

#include <QWidget>
#include <QtGui>
#include "ct_treeofworklogs.h"

class CTWorklogs : public QWidget
{
    Q_OBJECT
public:
    explicit CTWorklogs(QWidget *parent = 0);
    
    QToolBar *toolBar;
    QComboBox *comboBox;
    QTextEdit *worklogList;
    QTextEdit *workLogEdit;

    CTTreeOfWorkLogs *treeOfLogs;

    QString getLog();
    void initializeComboBox(QStringList timestampsList);
    void initializeWorklogList(QStringList timestamps, QStringList worklogsList);

private:
    void resetComboBox();


signals:
    void saveLog();
    
public slots:
    void enableWorklogEditor();
    void disableWorkLogEditor(QTextEdit *editor);
    void newLog();


private slots:
    void showLog(QModelIndex index);
};

#endif // CT_WORKLOGS_H
