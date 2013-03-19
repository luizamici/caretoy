#ifndef CT_QSQLTABLEOFSCENARIOS_H
#define CT_QSQLTABLEOFSCENARIOS_H

#include <QSqlTableModel>
#include <QStringList>

class CTQSqlTableOfScenarios : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CTQSqlTableOfScenarios(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    
    void initialize(QStringList id_scenarios_for_patient);
signals:
    
public slots:
    
};

#endif // CT_QSQLTABLEOFSCENARIOS_H
