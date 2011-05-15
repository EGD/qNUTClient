#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtNetwork/QTcpSocket>

class upsClient : public QObject
{
    Q_OBJECT

public:
    upsClient(const QString &host, const int port);
    ~upsClient();

//private:
    QTcpSocket *ASocket;

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

private slots:
    void slotConnected();
    void slotReadyRead();
};

#endif // MAINWINDOW_H
