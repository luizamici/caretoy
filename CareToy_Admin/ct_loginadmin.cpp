#include "ct_loginadmin.h"

CTLoginAdmin::CTLoginAdmin(QObject *parent) :
    QObject(parent)
{
    loginDialog = new CTLoginDialog();
    loginDialog->show();

    connect(loginDialog,SIGNAL(credentials(QString,QString)),this, SIGNAL(requestForAuthentication(QString,QString)));
}

void CTLoginAdmin::showWrongCredentialsMessage(){
    QPalette palette;
    palette.setColor( QPalette::WindowText, "red" );
    loginDialog->statusBar->setPalette( palette );
    loginDialog->statusBar->showMessage("Username or password incorrect");
}

void CTLoginAdmin::close(){
    loginDialog->close();
}
