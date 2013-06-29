#ifndef CT_LOGINDIALOG_H
#define CT_LOGINDIALOG_H

#include <QDialog>
#include <QDebug>
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

signals:
    void credentials(const QString &nickname,const QString &password);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

public slots:
    void showMessage(QString mss);
};

#endif // CT_LOGINDIALOG_H
