#include "ct_tablefield.h"

class CTTableFieldPrivate
{

public:
    CTTableFieldPrivate(const QString &name,
                        QVariant::Type type) :
                  ref(1), nm(name), ro(false), type(type), req(CTTableField::Unknown),
        autoval(false), len(-1), prec(-1)
    {
    }



    bool operator==(const CTTableFieldPrivate& other) const
    {
        return (nm == other.nm
                && ro == other.ro
                && type == other.type
                && req == other.req
                && def == other.def
                && autoval == other.autoval
                && len == other.len
                && prec == other.prec);
    }

    QAtomicInt ref;
    QString nm;
    uint ro: 1;
    QVariant::Type type;
    CTTableField::RequiredStatus req;
    QVariant def;
    uint autoval: 1;
    int len;
    int prec;

};


CTTableField::CTTableField(const QString &fieldName, QVariant::Type type)
{
    d = new CTTableFieldPrivate(fieldName, type);
    val = QVariant::typeToName(type);
}


CTTableField::CTTableField(const CTTableField& other)

{
    d = other.d;
    d->ref.ref();
    val = other.val;
}

CTTableField& CTTableField::operator=(const CTTableField& other)

{
    qAtomicAssign(d, other.d);
    val = other.val;
    return *this;
}

bool CTTableField::operator==(const CTTableField& other) const

{
    return ((d == other.d || *d == *other.d)
            && val == other.val);
}


CTTableField::~CTTableField()

{
    if (!d->ref.deref())
        delete d;
}

void CTTableField::setValue(const QVariant& value)
{
    if (isReadOnly())
        return;
    val = value;
}

void CTTableField::setName(const QString& name)
{
    detach();
    d->nm = name;
}

QString CTTableField::name() const
{
    return d->nm;
}

bool CTTableField::isNull() const
{ return val.isNull(); }

void CTTableField::setReadOnly(bool readOnly)
{
    detach();
    d->ro = readOnly;
}

bool CTTableField::isReadOnly() const
{ return d->ro; }


void CTTableField::clear()
{
    if (isReadOnly())
        return;
    val = QVariant(type());
}

QVariant::Type CTTableField::type() const
{
    return d->type;
}

void CTTableField::setType(QVariant::Type type)
{
    detach();
    d->type = type;
//    if (!val.isNull())
//        val = QVariant(type);
}

bool CTTableField::isAutoValue() const
{
    return d->autoval;
}

void CTTableField::setAutoValue(bool autoVal)
{
    detach();
    d->autoval = autoVal;
}

void CTTableField::setRequiredStatus(RequiredStatus required)
{
    detach();
    d->req = required;
}

CTTableField::RequiredStatus CTTableField::requiredStatus() const
{
    return d->req;
}

void CTTableField::setDefaultValue(const QVariant &value)
{
    detach();
    d->def = value;
}


QVariant CTTableField::defaultValue() const
{
    return d->def;
}

void CTTableField::setLength(int fieldLength)
{
    detach();
    d->len = fieldLength;
}

void CTTableField::setPrecision(int precision)
{
    detach();
    d->prec = precision;
}

int CTTableField::length() const
{
    return d->len;
}

int CTTableField::precision() const
{
    return d->prec;
}

/*Internal*/
void CTTableField::detach()
{
    qAtomicDetach(d);
}
