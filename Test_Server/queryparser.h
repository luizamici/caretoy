#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QXmlStreamReader>


class QueryParser : public QObject
{
    Q_OBJECT
public:
    explicit QueryParser(QObject *parent = 0);
    QHash<QString, QVariant> parse(const QByteArray &stmt);
    QHash<QString, QVariant> parseForAuthentication(QString username,
                                                        QString psswd);

private:
    QString statement;
    QHash<QString, QVariant> selectStatement(const QByteArray &stmt);
    QHash<QString, QVariant> insertStatement(const QByteArray &stmt);
    QHash<QString,QVariant> updateStatement(const QByteArray &stmt);
    QHash<QString,QVariant> deleteStatement(const QByteArray &stmt);
    
};

#endif // QUERYPARSER_H
