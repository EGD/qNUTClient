#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new upsClient("localhost", 3493);
    QObject::connect(client, SIGNAL(connectedUPS()), this, SLOT(slotConnected()));
    QObject::connect(client, SIGNAL(disconnectedUPS()), this, SLOT(slotDisconnected()));
    QObject::connect(client, SIGNAL(readyReadUPS()), this, SLOT(slotReadyRead()));
    QObject::connect(client, SIGNAL(errorUPS(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)));

    client->connectToUPS(QString("upsmaster"), QString("fcd3e41583"), QString("myups"));

    getVars = new QTimer(this);
    QObject::connect(getVars, SIGNAL(timeout()), this, SLOT(slotGetVars()));

    getVars->start(1500);
}

MainWindow::~MainWindow()
{
    delete client;
    delete getVars;
    delete ui;
}

void MainWindow::slotConnected()
{
//    getVars->start(1500);
}

void MainWindow::slotDisconnected()
{
    getVars->stop();
}

void MainWindow::slotReadyRead()
{
    qDebug("READY_READ");
    ui->textLines->append(client->getValueAll());
}

void MainWindow::slotGetVars()
{
    ui->textLines->clear();
    client->sendCmd(QString("LIST VAR %1\n").arg("myups"));
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
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
        errStr = client->errorString();
        break;
    }

    QMessageBox::critical(0,"Error",errStr);
}
