#ifndef CT_TABLEFIELD_H
#define CT_TABLEFIELD_H

#include <QtCore>

class CTTableFieldPrivate;

class CTTableField
{
public:
    CTTableField(const QString& fieldName = QString(),
                 QVariant::Type type = QVariant::Invalid);

    CTTableField(const CTTableField& other);
    CTTableField& operator=(const CTTableField& other);
    bool operator==(const CTTableField& other) const;
    inline bool operator!=(const CTTableField &other) const { return !operator==(other); }

    ~CTTableField();

    enum RequiredStatus { Unknown = -1, Optional = 0, Required = 1 }; //required for NOT NULL

    void setValue(const QVariant &value);
    inline QVariant value() const  { return val; }
    void setName(const QString& name);
    QString name() const;
    bool isNull() const;
    void setReadOnly(bool readOnly);
    bool isReadOnly() const;
    void clear();
    QVariant::Type type() const;
    void setType(QVariant::Type type);
    bool isAutoValue() const;
    void setAutoValue(bool autoVal);

    void setRequiredStatus(RequiredStatus status);
    RequiredStatus requiredStatus() const;

    inline void setRequired(bool required)  { setRequiredStatus(required ? Required : Optional); }
    void setDefaultValue(const QVariant &value);
    QVariant defaultValue() const;

    void setLength(int fieldLength);
    void setPrecision(int precision);

    int length() const;
    int precision() const;


private:
    CTTableFieldPrivate* d;
    QVariant val;
    void detach();
};

#endif // CT_TABLEFIELD_H
