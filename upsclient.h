#ifndef UPSCLIENT_H
#define UPSCLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QHash>

#define TMPL_SEND_USER "USERNAME %1\n"
#define TMPL_SEND_PASSWD "PASSWORD %1\n"
#define TMPL_SEND_LOGIN "LOGIN %1\n"
#define TMPL_GET_VARS "LIST VAR %1\n"

#define CMD_LOGOUT "LOGOUT"

namespace UPS {
    class upsClient;
    enum upsValues {UPS_LOAD};
}

class upsClient : public QObject
{
    Q_OBJECT

public:
    upsClient(const QString &host = "localhost", const quint16 &port = 3493);
    ~upsClient();

    void connectToUPS(const QString &userName, const QString &password, const QString &login);
    void disconnectFromUPS();
    void refreshValues();
    QString errorString() const;
    QString getValue(QString value) const;
    QString getValueAll() const;
    bool isConnected();

signals:
    void connectedUPS();
    void disconnectedUPS();
    void readyReadUPS();
    void errorUPS(QAbstractSocket::SocketError);

private:
    Q_DISABLE_COPY(upsClient)

    void sendCmd(const QString &command);
    QTcpSocket m_Socket;
    QString ups_host;
    quint16 ups_port;
    QString ups_login;
    QString ups_passwd;
    QString ups_user;
    bool upsConnectedOk;
    QHash<QString, QString> *data;
    QString AllValues;

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
};

#endif // UPSCLIENT_H
