#include "upsclient.h"
#include <QMessageBox>

upsClientModel::upsClientModel(upsClient *p_ups, QObject *pobj)
    : QAbstractListModel(pobj),
       m_ups(p_ups)
{
    QObject::connect(m_ups, SIGNAL(readyReadUPS()), this, SLOT(slotReadyRead()));
    m_ups->connectToUPS();
}

QVariant upsClientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    return (role == Qt::DisplayRole ? QVariant(m_propertis.value(index.row())) : QVariant());
}

int upsClientModel::rowCount(const QModelIndex &parent) const
{
    return m_propertis.size();
}

void upsClientModel::slotReadyRead()
{
#ifndef QT_NO_DEBUG
    qDebug("READY_READ");
#endif
    m_propertis.clear();
    m_propertis << m_ups->getValueAll();

    emit dataChanged(QModelIndex(),QModelIndex());
}

void upsClientModel::slotError(QAbstractSocket::SocketError err)
{
    QString errStr;
    switch (err) {
    case QAbstractSocket::HostNotFoundError:
        errStr = "The host was not found.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errStr = "The remote host is closed";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errStr = "The connection was refused.";
        break;
    default:
        errStr = m_ups->errorString();
        break;
    }

    QMessageBox::critical(0,"Error",errStr);
}


upsClient::upsClient(const QString &host, const quint16 &port, QObject *pobj) :
    QObject(pobj),
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

    m_pollTimer = new QTimer(this);
    m_pollTimer->setInterval(DEFAULT_POLL_TIME);
    QObject::connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(slotRefreshValues()));
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

    connectToUPS();
}

void upsClient::connectToUPS()
{
    m_Socket.connectToHost(ups_host, ups_port, QIODevice::ReadWrite);
}

void upsClient::disconnectFromUPS()
{
    m_Socket.write(QString(CMD_LOGOUT).toAscii());
    m_Socket.disconnectFromHost();
}

void upsClient::setPollTime(int poll_time)
{
    m_pollTimer->setInterval(poll_time);
}

void upsClient::slotRefreshValues()
{
    sendCmd(QString(TMPL_GET_VARS).arg(ups_login));
}

void upsClient::setLogin(const QString &login) {
    ups_login = login;
}

void upsClient::setUserName(const QString &name) {
    ups_user = name;
}

void upsClient::setPassword(const QString &pass) {
    ups_passwd = pass;
}

void upsClient::setHost(const QString &host) {
    ups_host = host;
}

void upsClient::setPort(const quint16 &port) {
    ups_port = port;
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
#ifndef QT_NO_DEBUG
    qDebug("CONNECTED");
#endif
    //Autohorize
    sendCmd(QString(TMPL_SEND_USER).arg(ups_user));
    sendCmd(QString(TMPL_SEND_PASSWD).arg(ups_passwd));
    sendCmd(QString(TMPL_SEND_LOGIN).arg(ups_login));
    m_Socket.waitForBytesWritten();
#ifndef QT_NO_DEBUG
    qDebug("AUTH SEND");
#endif

    upsConnectedOk = true;
    m_pollTimer->start();

    emit connectedUPS();
}

void upsClient::slotDisconnected()
{
    upsConnectedOk = false;
    m_pollTimer->stop();

    emit disconnectedUPS();
}

void upsClient::slotReadyRead()
{
    QTextStream in(m_Socket.readAll(), QIODevice::ReadOnly);

    QRegExp templ("VAR \\w+ ((?:\\w+\\.?)+) \"(.*)\"");
    AllValues.clear();

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
