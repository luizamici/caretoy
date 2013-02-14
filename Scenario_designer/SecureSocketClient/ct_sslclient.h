#ifndef CT_SSLCLIENT_H
#define CT_SSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QtGui>

class CTSslClient : public QObject
{
    Q_OBJECT
public:
    explicit CTSslClient(QObject *parent = 0);


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
        void initialize();

};

#endif // CT_SSLCLIENT_H
