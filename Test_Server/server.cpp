#include "server.h"
#include <qdir.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <QSslSocket>

#define CT_CHUNK 512

Server::Server(quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    if(listen(QHostAddress::Any, port))
        qDebug() << "Server started on port:  " << serverPort();

    dbConn = new DBConn();
}

void Server::incomingConnection(int socketDescriptor)
{
    SSLServerConnection *conn = new SSLServerConnection(socketDescriptor,this->parent());
    connect(conn, SIGNAL(exec(QHash<QString,QVariant>)), dbConn, SLOT(exec(QHash<QString,QVariant>)));
    connect(dbConn, SIGNAL(requestToWrite(QString)),conn, SLOT(requestToWrite(QString)));
}

Server::~Server()
{
}

SSLServerConnection::SSLServerConnection(quint16 socketDescriptor,
                                             QObject *parent)
        : QObject(parent)
{

    _dataSize = strlen("CT_PKT") + sizeof(int);
    _readHeader = false;

    // Create an SSL socket and make its QTcpSocket use our accepted
    // socket, then give it the path to our certificate & private key
    // file
    socket = new QSslSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    socket->setPrivateKey("server.key");
    socket->setLocalCertificate("server.crt");
    socket->startServerEncryption();

    parser = new QueryParser();
    connect(socket, SIGNAL(connected()),this,SLOT(acceptedClient()));


    connect(socket, SIGNAL(disconnected()), SLOT(connectionClosed()));
    connect(socket, SIGNAL(encrypted()),this, SLOT(ready()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readData()));
}

SSLServerConnection::~SSLServerConnection()
{
    // Report that the connection has closed.
    qDebug("Connection closed.");
}

void SSLServerConnection::test()
{
}

void SSLServerConnection::requestToWrite(QString outputData)
{
    QByteArray out;
    out.append(outputData);
    if(socket->isOpen())
    {
        qint64 sent = socket->write("CT_PKT");
        socket->flush();

        if (-1 == sent)
        {
            qDebug() << socket->errorString();
            return;
        }

        int size = out.size();
        sent = socket->write((const char*) &size, sizeof(int));
        socket->flush();

        if (-1 == sent)
        {
            qDebug() << socket->errorString();
            return;
        }

        int pos = 0;
        while(pos< out.size())
        {
            sent = socket->write(out.mid(pos, CT_CHUNK));
            socket->flush();
            if (-1 == sent)
            {
                qDebug() << socket->errorString();
                return;
            }
            pos += sent;
        }
    }
}

void SSLServerConnection::ready()
{
    qDebug() << "Encrypted mode started with : " << socket->peerAddress()
             << socket->peerPort();
}

void SSLServerConnection::acceptedClient()
{
    // Provide feedback to the user about incoming connections. This
    // slot is only called if the connection was established, so all
    // communication is now encrypted.
    qDebug("Accepted new client from %s:%d",
           qPrintable(socket->peerAddress().toString()),
              socket->peerPort());

    // Print a simple string. Write this to the SSL socket.
    // The SSL socket encrypts the data, and sends it to the client.
    QString s = "Welcome!";
    socket->write(s.toLatin1().constData(), s.length());
}

void SSLServerConnection::readData()
{   
    QByteArray payload;
    while (_dataSize <= socket->bytesAvailable())
    {
        // Read packet header
        if (!_readHeader)
        {
            char ctPkt[7];
            qint64 recv = socket->read(ctPkt, strlen("CT_PKT"));
            ctPkt[6] = '\0';
            if (-1 == recv)
            {
                qDebug() << socket->errorString();
                return;
            }

            int size = 0;
            recv = socket->read((char *) &size, sizeof(int));
            if (-1 == recv)
            {
                qDebug() << socket->errorString();
                return;
            }

            if ("CT_PKT" == QString(ctPkt))
            {
                _dataSize = size;
                _readHeader = true;
            }
        }
        // Read packet payload
        else
        {
            while (_dataSize <= socket->bytesAvailable() && _dataSize > 0)
            {
                char *chunk = new char[_dataSize];
                qint64 recv = socket->read(chunk, _dataSize);
                if (-1 == recv) { qDebug() << socket->errorString(); }
                else
                {
                    _dataSize -= recv;
                    payload.append(chunk, recv);
                }
                delete chunk;
            }
            _readHeader = false;
            _dataSize = strlen("CT_PKT") + sizeof(int);
        }
    }
    if (!payload.isEmpty()) { exec(parser->parse(payload)); }
}

void SSLServerConnection::connectionClosed()
{
    // Although the socket may be closing, we must not delete it until
    // the delayed close is done.
    if (socket->state() == QAbstractSocket::ClosingState) {
            connect(socket, SIGNAL(disconnected()), SLOT(deleteLater()));
    } else {
        deleteLater();
        return;
    }
    qDebug("SSL Connection closed.");
}

void SSLServerConnection::error(QAbstractSocket::SocketError)
{
    // The SSL socket conveniently provides human readable error
    // messages through the errorString() call. Note that sometimes
    // the errors come directly from the underlying SSL library, and
    // the quality of the text may vary.
    qDebug("Error: %s", qPrintable(socket->errorString()));
}
