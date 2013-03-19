#ifndef CT_QSQLTABLEOFRESULTS_H
#define CT_QSQLTABLEOFRESULTS_H

#include <QSqlTableModel>
#include <QStringList>

class CTQSqlTableOfResults : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CTQSqlTableOfResults(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    void initialize(QStringList id_scenarios_for_patient);
    
signals:
    
public slots:
    
};

#endif // CT_QSQLTABLEOFRESULTS_H
