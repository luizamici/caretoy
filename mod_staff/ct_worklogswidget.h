#ifndef CT_WORKLOGSWIDGET_H
#define CT_WORKLOGSWIDGET_H

#include <QWidget>

namespace Ui {
class CTWorklogsWidget;
}

class CTWorklogsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTWorklogsWidget(QWidget *parent = 0);
    ~CTWorklogsWidget();

    void init(QByteArray table_data);
    
private slots:
    void on_qtb_saveButton_clicked();
    void on_qtb_editButton_clicked();
    void on_qtbNewButton_clicked();

private:
    Ui::CTWorklogsWidget *ui;

    void readMode();
    void editMode();
};

#endif // CT_WORKLOGSWIDGET_H
