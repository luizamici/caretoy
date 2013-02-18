#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>

#include "queryparser.h"
#include "dbconn.h"

class SSLServerConnection: public QObject
{
    Q_OBJECT
public:
    SSLServerConnection(quint16 socket, QObject *parent = 0);
    ~SSLServerConnection();

private:

    QueryParser *parser;
signals:
    void exec(QHash<QString,QVariant> query);

public slots:
    void ready();
    void acceptedClient();
    void readData();
    void connectionClosed();
    void error(QAbstractSocket::SocketError err);

    void test();
    void requestToWrite(QString outputData);

private:
    QSslSocket *socket;
    int _dataSize;
    bool _readHeader;
};

class Server : public QTcpServer
{
    Q_OBJECT
    
public:
    Server(quint16 port, QObject *parent = 0);
    ~Server();

    void incomingConnection(int socketDescriptor);
private:
    DBConn *dbConn;

};

#endif //SERVER_H
