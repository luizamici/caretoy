#ifndef CT_DIALOGS_H
#define CT_DIALOGS_H

#include <QDialog>
#include "CareToy_Admin/ct_dbconn.h"

namespace Ui {
class CTDialogs;
}

class CTDialogs : public QDialog
{
    Q_OBJECT
    
public:
    explicit CTDialogs(QStringList stringList, QWidget *parent);
    ~CTDialogs();
    
private:
    Ui::CTDialogs *ui;
    CTDBConn *conn;
    QStringList privateStringList;

public slots:
    void addLog();

signals:
    void logChanged();
};

#endif // CT_DIALOGS_H
