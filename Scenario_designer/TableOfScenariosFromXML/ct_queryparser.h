#ifndef CT_QUERYPARSER_H
#define CT_QUERYPARSER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QVariant>
#include <QtDebug>

class CTTableData;
class CTTableRecord;

class CTQueryParser
{
public:
    CTQueryParser();

    enum StatementType { WhereStatement, SelectStatement, UpdateStatement,
                         InsertStatement, DeleteStatement , InsertOrUpdateStatement};

    static QString xmlStatement(StatementType type, const QString &tableName,
                                const CTTableRecord &row);
    static QString prepareQuery(QString initStmt, QString whereStmt);
};

#endif // CT_QUERYPARSER_H
