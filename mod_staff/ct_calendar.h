#ifndef CT_CALENDAR_H
#define CT_CALENDAR_H

#include <QWidget>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QDebug>
#include <QDate>

class CTCalendar : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit CTCalendar(QWidget *parent = 0);


signals:
    
public slots:
    void init(QStringList *datesForLogs);
    
};

#endif // CT_CALENDAR_H
