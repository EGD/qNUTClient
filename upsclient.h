#ifndef UPSCLIENT_H
#define UPSCLIENT_H

#include <QObject>
#include <QTimer>
#include <QAbstractListModel>
#include <QtNetwork/QTcpSocket>
#include <QHash>

#define TMPL_SEND_USER "USERNAME %1\n"
#define TMPL_SEND_PASSWD "PASSWORD %1\n"
#define TMPL_SEND_LOGIN "LOGIN %1\n"
#define TMPL_GET_VARS "LIST VAR %1\n"

#define DEFAULT_POLL_TIME 1500

#define CMD_LOGOUT "LOGOUT"

class upsClient;
class upsClientModel;

class upsClientModel : public QAbstractListModel
{
    Q_OBJECT

public:
    upsClientModel(upsClient *p_ups, QObject *pobj = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

public:
    enum upsValues {BATTERY_VOLTAGE_NOMINAL = 0,
                               BEEPER_STATUS,
                               DEVICE_TYPE,
                               DRIVER_NAME,
                               DRIVER_PARAMETER_POLLINTERVAL,
                               DRIVER_PARAMETER_PORT,
                               DRIVER_VERSION,
                               DRIVER_VERSION_INTERNAL,
                               INPUT_CURRENT_NOMINAL,
                               INPUT_FREQUENCY,
                               INPUT_FREQUENCY_NOMINAL,
                               INPUT_VOLTAGE,
                               INPUT_VOLTAGE_FAULT,
                               INPUT_VOLTAGE_NOMINAL,
                               OUTPUT_VOLTAGE,
                               UPS_DELAY_SHUTDOWN,
                               UPS_DELAY_START,
                               UPS_LOAD,
                               UPS_STATUS,
                               UPS_TEMPERATURE,
                               UPS_TYPE};

private:
    upsClient *m_ups;
    QList<QString> m_propertis;

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);
};

class upsClient : public QObject
{
    Q_OBJECT

public:
    upsClient(const QString &host = "localhost", const quint16 &port = 3493, QObject *pobj = 0);
    ~upsClient();

    void connectToUPS(const QString &userName, const QString &password, const QString &login);
    void connectToUPS();
    void disconnectFromUPS();
    QString errorString() const;
    QString getValue(QString value) const;
    QList<QString> getValueAll() const;
    bool isConnected();

public slots:
    void setPollTime(int poll_time);
    void setLogin(const QString &login);
    void setUserName(const QString &name);
    void setPassword(const QString &pass);
    void setHost(const QString &host);
    void setPort(const quint16 &port);

signals:
    void connectedUPS();
    void disconnectedUPS();
    void readyReadUPS();
    void errorUPS(QAbstractSocket::SocketError);

private:
    Q_DISABLE_COPY(upsClient)

    void sendCmd(const QString &command);
    QTcpSocket m_Socket;
    QTimer *m_pollTimer;
    QString ups_host;
    quint16 ups_port;
    QString ups_login;
    QString ups_passwd;
    QString ups_user;
    bool upsConnectedOk;
    QHash<QString, QString> *data;
    QList<QString> AllValues;

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotRefreshValues();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
};

#endif // UPSCLIENT_H
