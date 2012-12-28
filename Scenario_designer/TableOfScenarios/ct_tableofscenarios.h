#ifndef CT_TABLEOFSCENARIOS_H
#define CT_TABLEOFSCENARIOS_H

#include <QWidget>
#include <QtGui>
#include <QSqlRecord>
#include "ct_sqltable.h"

namespace Ui {
class CTTableOfScenarios;
}

class CTTableOfScenarios : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTTableOfScenarios(QWidget *parent = 0);
    ~CTTableOfScenarios();

    CTSqlTable *tableOfScenarios;
    
private slots:
    void on_qbt_add_clicked();
    void on_qbt_edit_clicked();
    void on_qbt_copy_clicked();
    void on_qbt_delete_clicked();
    void on_qbt_submit_clicked();

private:
    Ui::CTTableOfScenarios *ui;
    QStatusBar *statusBar;



signals:
    void newScenario();
    void editScenario(QHash<QString,QString> scenario);
    void copyScenario(QHash<QString,QString> scenario);
};

#endif // CT_TABLEOFSCENARIOS_H
