#ifndef CT_DIALOG_H
#define CT_DIALOG_H

#include <QWidget>

namespace Ui {
class CTDialog;
}

class CTDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTDialog(QWidget *parent = 0);
    ~CTDialog();

    void clear();
    void setData(QString description, QString execution_day, QString execution_order);
    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::CTDialog *ui;

signals:
    void accepted(QString description,QString execution_day, QString execution_order);

};

#endif // CT_DIALOG_H
