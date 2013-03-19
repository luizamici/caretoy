#ifndef CT_COMBOLOG_H
#define CT_COMBOLOG_H

#include <QObject>
#include <QComboBox>
#include "ct_worklog.h"

class CTComboLog : public QComboBox
{
    Q_OBJECT
public:
    explicit CTComboLog(QWidget *parent = 0);
    CTWorkLog *worklog;
//    void mousePressEvent (QMouseEvent *e );
//    void mouseReleaseEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);


signals:
    void comboSelected();
    void itemSelected(const int &itemIndex);

    
public slots:
    void updateData(const QStringList &itemsList);
    
};

#endif // CT_COMBOLOG_H
