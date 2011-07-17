#include "upsclient.h"

upsClient::upsClient(const QString &host, const quint16 &port) :
    ups_host(host),
    ups_port(port),
    upsConnectedOk(false)
{
    QObject::connect(&m_Socket, SIGNAL(disconnected), this, SLOT(slotDisconected()));
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
    m_Socket.connectToHost(ups_host, ups_port, QIODevice::ReadWrite);
    m_Socket.waitForConnected();

    if (m_Socket.isOpen()) {
        qDebug("CONNECTED");
        //Autohorize
        sendCmd(QString("USERNAME %1\n").arg(userName));
        sendCmd(QString("PASSWORD %1\n").arg(password));
        sendCmd(QString("LOGIN %1\n").arg(login));
        m_Socket.waitForBytesWritten();

        qDebug("AUTH SEND");
        upsConnectedOk = true;
        emit connectedUPS();
    }
}

void upsClient::disconnectFromUPS()
{
    m_Socket.write(QString("LOGOUT").toAscii());
    m_Socket.disconnectFromHost();
}

const QString upsClient::getValueAll()
{
    QTextStream in(m_Socket.readAll(), QIODevice::ReadOnly);

    return in.readAll();
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

void upsClient::slotDisconnected()
{
    upsConnectedOk = false;
    emit disconnectedUPS();
}

void upsClient::slotReadyRead()
{
    if (isConnected()) {
        emit readyReadUPS();
    }
}

void upsClient::slotError(QAbstractSocket::SocketError err)
{
    emit errorUPS(err);
}
