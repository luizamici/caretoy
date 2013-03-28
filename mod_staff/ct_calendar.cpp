#include "ct_calendar.h"

CTCalendar::CTCalendar(QWidget *parent) :
    QCalendarWidget(parent)
{
    setGridVisible(true);
    setFirstDayOfWeek(Qt::Monday);
    setMinimumDate(QDate::fromString("2013-01-01", "yyyy-MM-dd"));
    setDateEditEnabled(false);
}


void CTCalendar::init(QStringList *datesForLogs)
{
    qDebug() << "CTCalendar::init";
    QTextCharFormat format;
    format.setBackground(QColor(240, 240, 255));
    qDebug() << datesForLogs->size();
    for (int var = 0; var < datesForLogs->size(); ++var) {
        QDateTime _dateTime = QDateTime::fromString(datesForLogs->at(var),
                                                    "yyyy-MM-dd HH:mm:ss");
        QDate _date = _dateTime.date();
        this->setDateTextFormat(_date, format);
    }
    return;
}
