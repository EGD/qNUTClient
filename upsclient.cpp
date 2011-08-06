#include "upsclient.h"
#include <QMessageBox>

upsClient::upsClient(const QString &host, const quint16 &port) :
    ups_host(host),
    ups_port(port),
    upsConnectedOk(false)
{
    data = new QHash<QString, QString>();

    QObject::connect(&m_Socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    QObject::connect(&m_Socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    QObject::connect(&m_Socket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)));
    QObject::connect(&m_Socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

upsClient::~upsClient()
{
    if (isConnected()) {
        disconnectFromUPS();
    }
}

void upsClient::connectToUPS(const QString &userName, const QString &password, const QString &login)
{    
    ups_user = userName;
    ups_passwd = password;
    ups_login = login;

    m_Socket.connectToHost(ups_host, ups_port, QIODevice::ReadWrite);
}

void upsClient::disconnectFromUPS()
{
    m_Socket.write(QString(CMD_LOGOUT).toAscii());
    m_Socket.disconnectFromHost();
}

void upsClient::refreshValues()
{
    sendCmd(QString(TMPL_GET_VARS).arg(ups_login));
}

QString upsClient::errorString() const
{
    return m_Socket.errorString();
}

bool upsClient::isConnected()
{
    return upsConnectedOk;
}

void upsClient::sendCmd(const QString &command)
{
    m_Socket.write(command.toAscii());
}

QString upsClient::getValue(QString key) const
{
    return data->contains(key) ? data->value(key) : QString("-1");
}

QString upsClient::getValueAll() const
{
    return AllValues;
}

void upsClient::slotConnected()
{
#ifdef DEBUG
    qDebug("CONNECTED");
#endif
    //Autohorize
    sendCmd(QString(TMPL_SEND_USER).arg(ups_user));
    sendCmd(QString(TMPL_SEND_PASSWD).arg(ups_passwd));
    sendCmd(QString(TMPL_SEND_LOGIN).arg(ups_login));
    m_Socket.waitForBytesWritten();
#ifdef DEBUG
    qDebug("AUTH SEND");
#endif
    refreshValues();

    upsConnectedOk = true;
    emit connectedUPS();
}

void upsClient::slotDisconnected()
{
    upsConnectedOk = false;
    emit disconnectedUPS();
}

void upsClient::slotReadyRead()
{
    QTextStream in(m_Socket.readAll(), QIODevice::ReadOnly);

    QRegExp templ("VAR \\w+ ((?:\\w+\\.?)+) \"(.*)\"");
    AllValues.clear();
//    AllValues = in.readAll();
//    in.seek(0);

    while (!in.atEnd()) {
        QString line(in.readLine());

        if (line.indexOf(QString("BEGIN LIST VAR")) != -1 ||
                line.indexOf(QString("END LIST")) != -1) {
            continue;
        }

        templ.indexIn(line);

        if (templ.captureCount()) {
            (*data)[templ.cap(1)] = templ.cap(2);
            AllValues.append(templ.cap(1)).append(": ").append(templ.cap(2)).append("\n");
        }
    }

    emit readyReadUPS();
}

void upsClient::slotError(QAbstractSocket::SocketError err)
{
    emit errorUPS(err);
}
