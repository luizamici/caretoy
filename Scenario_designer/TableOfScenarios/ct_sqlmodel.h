#ifndef CT_SQLMODEL_H
#define CT_SQLMODEL_H

#include <QWidget>
#include <QSqlTableModel>

class CTSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CTSqlModel(QWidget *parent = 0,QSqlDatabase db = QSqlDatabase());

    void setQuery(const QSqlQuery &query);
    
signals:
    
public slots:
    
};

#endif // CT_SQLMODEL_H
