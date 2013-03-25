#ifndef CT_WORKLOGSWIDGET_H
#define CT_WORKLOGSWIDGET_H

#include <QWidget>
#include <QtGui>

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
    void showLog(QModelIndex index);

private:
    Ui::CTWorklogsWidget *ui;

    QString currentLog;
    QString idLog;
    void readMode();
    void editMode();
    void newLogMode();

signals:
    void nothingToSave(QString message);
    void execParsedQuery(QString init_stmt,QString where_stmt);
};

#endif // CT_WORKLOGSWIDGET_H
