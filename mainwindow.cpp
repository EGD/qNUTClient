#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new upsClient("localhost", 3493);
    QObject::connect(client->ASocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    QObject::connect(client->ASocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

    client->connect();
    QObject::connect(client->ASocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

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
    qDebug() << "CONNECTED";
//Autohorize
    client->sendCmd(QString("USERNAME %1\n").arg("upsmaster"));
    client->sendCmd(QString("PASSWORD %1\n").arg("fcd3e41583"));
    client->sendCmd(QString("LOGIN %1\n").arg("myups"));
    client->ASocket->waitForBytesWritten();
    qDebug("AUTH SEND");
}

void MainWindow::slotDisconnected()
{
    getVars->stop();
}

void MainWindow::slotReadyRead()
{
    qDebug() << "READY_READ";
    QTextStream in(client->ASocket->readAll());
    ui->textLines->append(in.readAll());
}

void MainWindow::slotGetVars()
{
    ui->textLines->clear();
    client->sendCmd(QString("LIST VAR %1\n").arg("myups"));
}

upsClient::upsClient(const QString &host, const qint16 &port) : ups_host(host), ups_port(port)
{
    ASocket = new QTcpSocket(this);

    QObject::connect(ASocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)));
}

upsClient::~upsClient()
{
    ASocket->write(QString("LOGOUT").toAscii());
    ASocket->disconnectFromHost();
    qDebug() << "DISCONNECTED";
    delete ASocket;
}

void upsClient::connect()
{
    ASocket->connectToHost(ups_host, ups_port, QIODevice::ReadWrite);
}

upsClient const *upsClient::sendCmd(const QString &command)
{
    ASocket->write(command.toAscii());
    return this;
}

void upsClient::slotError(QAbstractSocket::SocketError err)
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
        errStr = ASocket->errorString();
        break;
    }

    QMessageBox::critical(0,"Error",errStr);
}
