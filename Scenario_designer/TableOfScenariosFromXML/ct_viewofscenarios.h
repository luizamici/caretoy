#ifndef CT_VIEWOFSCENARIOS_H
#define CT_VIEWOFSCENARIOS_H

#include <QWidget>
#include <QTableView>
#include <QtGui>

#include "ct_xmlparser.h"
#include "ct_xmltableofscenarios.h"

class CTViewOfScenarios : public QWidget
{
    Q_OBJECT
public:
    explicit CTViewOfScenarios(QWidget *parent = 0);

    QTableView *table;
    CTXMLTableOfScenarios *xmlTable;
private:
     QHash<QString,QString> getSelected();
    
signals:
    void editScenario(QHash<QString,QString> scenario);
    void newScenario();
public slots:

private slots:
    void on_edit_clicked();
    void on_add_clicked();
    void on_copy_clicked();
    void on_remove_clicked();
};

#endif // CT_VIEWOFSCENARIOS_H
