#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>

#include "queryparser.h"
#include "dbconn.h"
#include "ct_defs.h"

class SSLServerConnection: public QObject
{
    Q_OBJECT
public:
    SSLServerConnection(quint16 socket, QObject *parent = 0);
    ~SSLServerConnection();

private:
    QueryParser *parser;
    QSslSocket *socket;
    DBConn *dbConn;

    int _dataSize;
    bool _readHeader;
    quint32 _readType;


    void writeAll(const QString &parsedQuery, const quint32 &type);
    void processXML(QByteArray data);
    void processQuery(QByteArray data);

public slots:
    void ready();
    void acceptedClient();
    void readData();
    void connectionClosed();
    void error(QAbstractSocket::SocketError err);
};

class Server : public QTcpServer
{
    Q_OBJECT
    
public:
    Server(quint16 port, QObject *parent = 0);
    ~Server();

    void incomingConnection(int socketDescriptor);
};

#endif //SERVER_H
