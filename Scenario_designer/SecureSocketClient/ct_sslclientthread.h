#ifndef CT_SSLCLIENTTHREAD_H
#define CT_SSLCLIENTTHREAD_H

#include <QObject>
#include <QThread>
#include <QSslSocket>
#include <QHostAddress>

class CTSslClientThread : public QThread
{
    Q_OBJECT
public:
    explicit CTSslClientThread(QObject *parent = 0);

    void run();
    void initialize();
    bool isConnected();
private:
    QSslSocket *socket;
    int _dataSize;
    bool _readHeader;

signals:
    void notConnected(QString warning);
    void encryptionStarted();
    void dataReceived(QString input);
    void proccessData(QByteArray input);
public slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void sslErrors(const QList<QSslError> &errors);
    void socketReadyRead();
    void connectedToHost();

    bool requestForWrite(const QString &parsedQuery);

};

#endif // CT_SSLCLIENTTHREAD_H
