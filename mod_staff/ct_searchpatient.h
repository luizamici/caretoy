#ifndef CT_SEARCHPATIENT_H
#define CT_SEARCHPATIENT_H

#include <QWidget>

namespace Ui {
class CTSearchPatient;
}

class CTSearchPatient : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTSearchPatient(QWidget *parent = 0);
    ~CTSearchPatient();
    
private:
    Ui::CTSearchPatient *ui;

signals:
    void searchBy(int index);
    void searchFor(QString input);

public slots:
    void search(int index);
};

#endif // CT_SEARCHPATIENT_H
