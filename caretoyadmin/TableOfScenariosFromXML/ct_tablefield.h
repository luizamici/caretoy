#ifndef CT_TABLEFIELD_H
#define CT_TABLEFIELD_H

#include <QtCore>

class CTTableFieldPrivate;

class CTTableField
{
public:
    CTTableField(const QString& fieldName = QString(),
                 QString type = QString());

    CTTableField(const CTTableField& other);
    CTTableField& operator=(const CTTableField& other);
    bool operator==(const CTTableField& other) const;
    inline bool operator!=(const CTTableField &other) const { return !operator==(other); }

    ~CTTableField();

    enum RequiredStatus { Unknown = -1, Optional = 0, Required = 1 }; //required for NOT NULL

    void setValue(const QString &value);
    inline QString value() const  { return val; }
    void setName(const QString& name);
    QString name() const;
    bool isNull() const;
    void setReadOnly(bool readOnly);
    bool isReadOnly() const;
    void clear();
    QString type() const;
    void setType(QString type);
    bool isAutoValue() const;
    void setAutoValue(bool autoVal);

    void setRequiredStatus(RequiredStatus status);
    RequiredStatus requiredStatus() const;

    inline void setRequired(bool required)  { setRequiredStatus(required ? Required : Optional); }
    void setDefaultValue(const QString &value);
    QString defaultValue() const;

    void setLength(int fieldLength);
    void setPrecision(int precision);

    int length() const;
    int precision() const;


private:
    CTTableFieldPrivate* d;
    QString val;
    void detach();
};

#endif // CT_TABLEFIELD_H
