#ifndef UPSCLIENT_H
#define UPSCLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>

class upsClient : public QObject
{
    Q_OBJECT

public:
    upsClient(const QString &host = "localhost", const quint16 &port = 3493);
    ~upsClient();

    void connectToUPS(const QString &userName, const QString &password, const QString &login);
    void disconnectFromUPS();
    void sendCmd(const QString &command);
    const QString getValueAll();
    QString errorString() const;
    QByteArray readAll() const;
    bool isConnected();

signals:
    void connectedUPS();
    void disconnectedUPS();
    void readyReadUPS();
    void errorUPS(QAbstractSocket::SocketError);

private:
    Q_DISABLE_COPY(upsClient)

    QTcpSocket m_Socket;
    QString ups_host;
    quint16 ups_port;
    bool upsConnectedOk;

private slots:
    void slotDisconnected();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
};

#endif // UPSCLIENT_H
