#ifndef CT_AUX_H
#define CT_AUX_H

#include <QString>

//Removes the 'T' character from the DB returned timestamp
inline QString formatDate(QString date)
{
    return date.replace("T", " ");
}

#endif // CT_AUX_H
