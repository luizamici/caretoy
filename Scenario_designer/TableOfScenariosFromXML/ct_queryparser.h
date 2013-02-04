#ifndef CT_QUERYPARSER_H
#define CT_QUERYPARSER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QVariant>


class CTQueryParser
{
public:
    CTQueryParser();

    enum StatementType { WhereStatement, SelectStatement, UpdateStatement,
                         InsertStatement, DeleteStatement , InsertOrUpdateStatement};

    static QString xmlStatement(StatementType type, const QString &tableName,
                      const QHash<QString, QString> &rec);
};

#endif // CT_QUERYPARSER_H
