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
    bool tableSelected;

signals:
    void editScenario(QHash<QString,QString> scenario);
    void newScenario();

public slots:
    void save(QHash<QString, QString> scenario);

private slots:
    void on_edit_clicked();
    void on_add_clicked();
    void on_copy_clicked();
    void on_remove_clicked();

    void tableSelectionChanged(const QItemSelection &selected,
                               const QItemSelection &deselected);

};

#endif // CT_VIEWOFSCENARIOS_H
