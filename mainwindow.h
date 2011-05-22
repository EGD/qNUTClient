#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtNetwork/QTcpSocket>

class upsClient : public QObject
{
    Q_OBJECT

public:
    upsClient(const QString &host, const qint16 &port);
    ~upsClient();

    upsClient const *sendCmd(const QString &command);
    void connect();
    QTcpSocket *ASocket;

private:
    //QTcpSocket *ASocket;
    QString ups_host;
    qint16 ups_port;
    Q_DISABLE_COPY(upsClient)

private slots:
    void slotError(QAbstractSocket::SocketError);
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    upsClient *client;
    QTimer *getVars;

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();
    void slotGetVars();
};

#endif // MAINWINDOW_H
