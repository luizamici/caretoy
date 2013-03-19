#ifndef CT_QSQLTABLEOFPATIENTS_H
#define CT_QSQLTABLEOFPATIENTS_H

#include "qsqltablemodel.h"


class CTQSqlTableOfPatients : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CTQSqlTableOfPatients(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

signals:

};

#endif // CT_QSQLTABLEOFPATIENTS_H
