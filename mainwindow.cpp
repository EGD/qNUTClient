#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new upsClient("localhost", 3493);
    QObject::connect(client->ASocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    QObject::connect(client->ASocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::slotConnected()
{
    qDebug() << "CONNECTED";
//Autohorize
    client->ASocket->write(QString("USERNAME %1\n").arg("upsmaster").toAscii());
    client->ASocket->write(QString("PASSWORD %1\n").arg("fcd3e41583").toAscii());
    client->ASocket->write(QString("LOGIN %1\n").arg("myups").toAscii());
    client->ASocket->waitForBytesWritten();
    qDebug() << "AUTH SEND";
}

void MainWindow::slotReadyRead()
{
    qDebug() << "READY_READ";
    QTextStream in(client->ASocket);
    ui->textLines->append(in.readAll());
}

upsClient::upsClient(const QString &host, const int port)
{
    ASocket = new QTcpSocket(this);

    QObject::connect(ASocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)));

    ASocket->connectToHost(host, port, QIODevice::ReadWrite);
}

upsClient::~upsClient()
{
    ASocket->disconnectFromHost();
    qDebug() << "DISCONNECTED";
    delete ASocket;
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
