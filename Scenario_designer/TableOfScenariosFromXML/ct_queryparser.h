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
                         InsertStatement, DeleteStatement };


    QString statement(StatementType type, const QString &tableName,
                                 const QHash<QString, QString> &rec) const;
};

#endif // CT_QUERYPARSER_H
