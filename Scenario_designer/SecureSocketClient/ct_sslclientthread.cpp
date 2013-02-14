#include "ct_sslclientthread.h"

#define CT_CHUNK 512

CTSslClientThread::CTSslClientThread(QObject *parent) :
    QThread(parent)
{
    _dataSize = strlen("CT_PKT") + sizeof(int);
    _readHeader = false;
}


void CTSslClientThread::run()
{
    initialize();
}

void CTSslClientThread::initialize()
{
    socket = new QSslSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connectedToHost()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(encrypted()),
            this, SLOT(socketEncrypted()));
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(sslErrors(QList<QSslError>)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));

    socket->connectToHostEncrypted(QHostAddress(QHostAddress::LocalHost).
                                   toString(),44444);
}


void CTSslClientThread::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState)
    {
        emit notConnected("Unable to connect to server!");
        socket->deleteLater();
        socket = 0;
    }
}


void CTSslClientThread::socketEncrypted()
{
    qDebug() << "Socket encrypted!";
    emit encryptionStarted();
}


void CTSslClientThread::sslErrors(const QList<QSslError> &errors)
{
    QList<QSslError> errs;
    foreach(QSslError err, errors)
    {
        if(err == QSslError::CertificateUntrusted ||
                QSslError::HostNameMismatch)
        {
            errs.append(err);
        }
    }
    socket->ignoreSslErrors(errs);
    qDebug() << "SSl Errors: " << errors;
    qDebug() << "Ssl Errors ignored! : " << errs ;
}

void CTSslClientThread::socketReadyRead()
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
    if (!payload.isEmpty()) { proccessData(payload); }
}

void CTSslClientThread::connectedToHost()
{
    qDebug() << "Connected to " << QHostAddress(QHostAddress::LocalHost).
                toString() << ": " << 55555;
}


bool CTSslClientThread::requestForWrite(const QString &parsedQuery)
{
    QByteArray out;
    out.append(parsedQuery);
    if(socket != 0)
    {
        qint64 sent = socket->write("CT_PKT");
        socket->flush();

        if (-1 == sent)
        {
                qDebug() << socket->errorString();
                return false;
        }

        int size = out.size();
        sent = socket->write((const char*) &size, sizeof(int));
        socket->flush();
        if (-1 == sent)
        {
              qDebug() << socket->errorString();
              return false;
        }

        int pos = 0;
        while(pos< out.size())
        {
            sent = socket->write(out.mid(pos, CT_CHUNK));
            socket->flush();
            if (-1 == sent)
                {
                    qDebug() << socket->errorString();
                    return false;
                }
                pos += sent;
        }
    }
}

bool CTSslClientThread::isConnected()
{
    return (socket != 0);
}
