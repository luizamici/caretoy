#ifndef CT_LOGINADMIN_H
#define CT_LOGINADMIN_H

#include <QObject>
#include "ct_logindialog.h"

class CTLoginAdmin : public QObject
{
    Q_OBJECT
public:
    explicit CTLoginAdmin(QObject *parent = 0);
    CTLoginDialog *loginDialog;

    void close();
    
signals:
    void requestForAuthentication(const QString &nickname,const QString &password);
public slots:
    void showWrongCredentialsMessage();
};

#endif // CT_LOGINADMIN_H
