#include "ct_sslclientthread.h"

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
    encryptedConn = false;

    QFile ctCaCert("./includ/caretoy_ca.crt");
    ctCaCert.open(QIODevice::ReadOnly);
    QSslCertificate caCert(ctCaCert.readAll());
    ctCaCert.close();

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

    socket->connectToHostEncrypted("fsm.caretoy.eu", 61499);
//    socket->connectToHostEncrypted(QHostAddress(QHostAddress::LocalHost).
//                                   toString(),44444);
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
    encryptedConn = true;
//    emit encryptionStarted();
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
            qint64 recv = socket->read((char*)&_readType, sizeof(quint32));
            if (-1 == recv)
            {
                qDebug() << socket->errorString();
                return;
            }

            qint64 size = 0;
            recv = socket->read((char *) &size, sizeof(qint64));
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
        if(CT_PKTDATA == _readType)
            processXML(payload);
        else if(CT_DBSDATA == _readType)
            processData(payload);
    }
}

void CTSslClientThread::connectedToHost()
{
    qDebug() << "Connected to " << QHostAddress(QHostAddress::LocalHost).
                toString() << ": " << 55555;
}


bool CTSslClientThread::writeIntoSocket(const QString &parsedQuery,
                                        const quint32 &type)
{
    QByteArray out;
    qDebug() << parsedQuery;
    out.append(parsedQuery);
    // Write data type
//    quint32 type = CT_PKTDATA;
    socket->write((const char *) &type, sizeof(quint32));
    socket->flush();
    socket->waitForBytesWritten();
    // Write data size
    qint64 size = out.size();
    socket->write((const char *) &size, sizeof(qint64));
    socket->flush();
    socket->waitForBytesWritten();
    // Write data
    quint32 pos = 0;
    while (pos < size)
    {
        qint64 written = socket->write(out.mid(pos, CT_CHUNKSIZE));
        socket->flush();
        socket->waitForBytesWritten();
        pos += written;
    }
    return true;
}

bool CTSslClientThread::isConnected()
{
    return (socket != 0);
}
