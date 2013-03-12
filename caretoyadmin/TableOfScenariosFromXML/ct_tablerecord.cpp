#include "ct_tablerecord.h"

class CTTableRecordPrivate
{
public:
    CTTableRecordPrivate();
    CTTableRecordPrivate(const CTTableRecordPrivate &other);

    inline bool contains(int index) { return index >= 0 && index < fields.count(); }
    QString createField(int index, const QString &prefix) const;

    QVector<CTTableField> fields;
    QAtomicInt ref;
};

CTTableRecordPrivate::CTTableRecordPrivate()
{
    ref = 1;
}

CTTableRecordPrivate::CTTableRecordPrivate(const CTTableRecordPrivate &other)
    :fields(other.fields)
{
    ref = 1;
}


QString CTTableRecordPrivate::createField(int index, const QString &prefix) const
{
    QString f;
    if (!prefix.isEmpty())
    f = prefix + QLatin1Char('.');
    f += fields.at(index).name();
    return f;
}



CTTableRecord::CTTableRecord()
{
    d = new CTTableRecordPrivate();
}


CTTableRecord::CTTableRecord(const CTTableRecord& other)
{
    d = other.d;
    d->ref.ref();
}

CTTableRecord::~CTTableRecord()
{
    if (!d->ref.deref())
    delete d;
}

CTTableRecord& CTTableRecord::operator=(const CTTableRecord& other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool CTTableRecord::operator==(const CTTableRecord &other) const
{
    return d->fields == other.d->fields;
}


QString CTTableRecord::value(int index) const
{
    return d->fields.value(index).value();
}


QString CTTableRecord::value(const QString& name) const
{
    return value(indexOf(name));
}

void CTTableRecord::setValue(int index, const QString& val)
{
    if (!d->contains(index))
    return;
    detach();
    d->fields[index].setValue(val);
}

void CTTableRecord::setValue(const QString& name, const QString& val)
{
    setValue(indexOf(name), val);
}


int CTTableRecord::indexOf(const QString& name) const
{
    QString nm = name.toUpper();
    for (int i = 0; i < count(); ++i) {
        if (d->fields.at(i).name().toUpper() == nm) // TODO: case-insensitive comparison
        return i;
    }
    return -1;
}

QString CTTableRecord::fieldName(int index) const
{
    return d->fields.value(index).name();
}

CTTableField CTTableRecord::field(int index) const
{
    return d->fields.value(index);
}

CTTableField CTTableRecord::field(const QString &name) const
{
    return field(indexOf(name));
}


bool CTTableRecord::isNull(int index) const
{
    return d->fields.value(index).isNull();
}

bool CTTableRecord::isNull(const QString& name) const
{
    return isNull(indexOf(name));
}

void CTTableRecord::setNull(int index)
{
    if (!d->contains(index))
    return;
    detach();
    d->fields[index].clear();
}


void CTTableRecord::setNull(const QString& name)
{
    setNull(indexOf(name));
}

void CTTableRecord::append(const CTTableField& field)
{
    detach();
    d->fields.append(field);
}

void CTTableRecord::insert(int pos, const CTTableField& field)
{
    detach();
    d->fields.insert(pos, field);
}

void CTTableRecord::replace(int pos, const CTTableField& field)
{
    if (!d->contains(pos))
        return;

    detach();
    d->fields[pos] = field;
}

void CTTableRecord::remove(int pos)
{
    if (!d->contains(pos))
        return;

    detach();
    d->fields.remove(pos);
}

bool CTTableRecord::isEmpty() const
{
    return d->fields.isEmpty();
}

bool CTTableRecord::contains(const QString& name) const
{
    return indexOf(name) >= 0;
}

void CTTableRecord::clear()
{
    detach();
    d->fields.clear();
}

void CTTableRecord::clearValues()
{
    detach();
    int count = d->fields.count();
    for (int i = 0; i < count; ++i)
    d->fields[i].clear();
}

int CTTableRecord::count() const
{
    return d->fields.count();
}

void CTTableRecord::detach()
{
    qAtomicDetach(d);
}
