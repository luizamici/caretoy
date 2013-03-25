#ifndef CT_LOGINDIALOG_H
#define CT_LOGINDIALOG_H

#include <QDialog>
#include <QStatusBar>

namespace Ui {
class CTLoginDialog;
}

class CTLoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CTLoginDialog(QWidget *parent = 0);
    ~CTLoginDialog();
    QStatusBar *statusBar;
    
private:
    Ui::CTLoginDialog *ui;

public slots:
    void okPressed();

signals:
    void credentials(const QString &nickname,const QString &password);

};

#endif // CT_LOGINDIALOG_H
