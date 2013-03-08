#include "server.h"
#include <qdir.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <QSslSocket>

Server::Server(quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    if(listen(QHostAddress::Any, port))
        qDebug() << "Server started on port:  " << serverPort();

//    dbConn = new DBConn();
}

void Server::incomingConnection(int socketDescriptor)
{
    SSLServerConnection *conn = new SSLServerConnection(socketDescriptor,this->parent());
//    connect(conn, SIGNAL(exec(QHash<QString,QVariant>)), dbConn, SLOT(exec(QHash<QString,QVariant>)));
//    connect(dbConn, SIGNAL(requestToWrite(QString)),conn, SLOT(requestToWrite(QString)));
}

Server::~Server()
{
}

SSLServerConnection::SSLServerConnection(quint16 socketDescriptor,
                                             QObject *parent)
        : QObject(parent)
{
    dbConn = new DBConn();
    dbConn->initialize();
    dbConn->openConnection();

    _dataSize = sizeof(quint32) + sizeof(int);
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
   dbConn->closeConnection();
   dbConn->deleteLater();
    qDebug("Connection closed.");
}


void SSLServerConnection::writeAll(const QString &parsedQuery,
                                   const quint32 &type)
{
    QByteArray out;
    out.append(parsedQuery);
    if(socket->isOpen())
    {
        qint64 sent = socket->write((const char*) &type, sizeof(quint32));
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
            sent = socket->write(out.mid(pos, CT_CHUNKSIZE));
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
            qint64 recv = socket->read((char*)&_readType, sizeof(quint32));
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
            if(CT_PKTDATA == _readType || CT_DBSDATA == _readType)
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
            _dataSize = sizeof(quint32) + sizeof(int);
        }
    }
    if (!payload.isEmpty())
    {
        qDebug() << CT_PKTDATA;
        qDebug() << CT_DBSDATA;
        qDebug() << _readType;
        if(CT_PKTDATA == _readType)
            processXML(payload);
        else if(CT_DBSDATA == _readType)
            processQuery(payload);
    }
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

void SSLServerConnection::processXML(QByteArray data)
{
    QXmlStreamReader reader(data);
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString tagName = reader.name().toString();
            if ("login_request" == tagName)
            {
                QXmlStreamAttributes attr = reader.attributes();
                QString username = attr.value("username").toString();
                QString password = attr.value("password").toString();
                // TO DO: Call database query function, send reply
                QString id = dbConn->authenticate(parser->parseForAuthentication(username,password));
                if("No valid ID found!" == id)
                {
                    qDebug() << "Authentication failure";
                    QString login_reply;
                    QXmlStreamWriter stream(&login_reply);
                    stream.writeStartElement("login_reply");
                    stream.writeAttribute("type", "failure");
                    stream.writeAttribute("user_id", id);
                    stream.writeEndElement();//end login_reply

                    writeAll(login_reply, CT_PKTDATA);
                }
                else if(id.contains("Error",Qt::CaseInsensitive))
                {
                    qDebug() << "DB Error" << id;
                    //TODO write db error to log
                }
                else
                {
                    //authentication successful
                    //TODO send client id
                    qDebug() << "Authentication successful : "<< id;
                    QString login_reply;
                    QXmlStreamWriter stream(&login_reply);
                    stream.writeStartElement("login_reply");
                    stream.writeAttribute("type", "success");
                    stream.writeAttribute("user_id", id);
                    stream.writeEndElement();//end login_reply

                    writeAll(login_reply, CT_PKTDATA);
                }
            }
            else if ("scenario_request" == tagName)
            {
                QString userId =
                        reader.attributes().value("user_id").toString();
                // TO DO: Call database query function, send reply
            }
        }
    }
}

// Process database query received from caretoyadmin client.
void SSLServerConnection::processQuery(QByteArray data)
{
    qDebug() << "processQuery";
    QString output;
    output = dbConn->exec(parser->parse(data));
    if("No valid data found!" == output)
    {
        //Send client no data found
    }
    else if(output.contains("Error", Qt::CaseInsensitive))
    {
        //write db error to log
    }
    else
        writeAll(output,CT_DBSDATA);
    return;
}
