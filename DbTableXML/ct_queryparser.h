#ifndef CT_QUERYPARSER_H
#define CT_QUERYPARSER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QStringList>
#include <QVariant>
#include <QtDebug>

#include "ct_tabledata.h"
//class CTTableData;
#include "ct_tablerecord.h"
//class CTTableRecord;

class CTQueryParser
{
public:
    CTQueryParser();

    enum StatementType { WhereStatement, SelectStatement, UpdateStatement,
                         InsertStatement, DeleteStatement };

    static QString xmlStatement(StatementType type, const QString &tableName,
                                const CTTableRecord &row);
    static QString prepareQuery(QString initStmt, QString whereStmt);
};

#endif // CT_QUERYPARSER_H
