#ifndef CT_TABLERECORD_H
#define CT_TABLERECORD_H

#include "QtCore"
#include "ct_tablefield.h"

class CTTableRecordPrivate;

class CTTableRecord
{
public:
    CTTableRecord();

    CTTableRecord(const CTTableRecord& other);
    CTTableRecord& operator=(const CTTableRecord& other);
    ~CTTableRecord();

    bool operator==(const CTTableRecord &other) const;
    inline bool operator!=(const CTTableRecord &other) const { return !operator==(other); }

    QVariant value(int i) const;
    QVariant value(const QString& name) const;
    void setValue(int i, const QVariant &val);
    void setValue(const QString& name, const QVariant &val);

    void setNull(int i);
    void setNull(const QString& name);
    bool isNull(int i) const;
    bool isNull(const QString& name) const;

    int indexOf(const QString &name) const;
    QString fieldName(int i) const;

    CTTableField field(int i) const;
    CTTableField field(const QString &name) const;

    void append(const CTTableField& field);
    void replace(int pos, const CTTableField& field);
    void insert(int pos, const CTTableField& field);
    void remove(int pos);

    bool isEmpty() const;
    bool contains(const QString& name) const;
    void clear();
    void clearValues();
    int count() const;

private:
    void detach();
    CTTableRecordPrivate *d;

};

#endif // CT_TABLERECORD_H
