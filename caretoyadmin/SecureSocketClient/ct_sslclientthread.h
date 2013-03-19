#ifndef CT_SSLCLIENTTHREAD_H
#define CT_SSLCLIENTTHREAD_H

#include <QObject>
#include <QThread>
#include <QSslSocket>
#include <QHostAddress>
#include <QFile>
#include <QTimer>
#include "CareToy_Admin/ct_defs.h"

class CTSslClientThread : public QThread
{
    Q_OBJECT
public:
    explicit CTSslClientThread(QObject *parent = 0);

    void run();
    void initialize();

private:
    QSslSocket *socket;
    int _dataSize;
    bool _readHeader;
    quint32 _readType;

    bool encryptedConn;
    bool waitForTable;
    bool readingSocket;

    QString _tableRequest;

signals:
    void notConnected(QString warning);
    void connectionSuccessful(QString mss);
    void dataReceived(QString input);

    void processData(QByteArray input);
    void processXML(QByteArray input);
public slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void sslErrors(const QList<QSslError> &errors);
    void socketReadyRead();
    void connectedToHost();

    bool writeIntoSocket(const QString &parsedQuery, const quint32 &type);
    void requestTable(const QString &parsedQuery, const quint32 &type);

};

#endif // CT_SSLCLIENTTHREAD_H
