#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new upsClient("localhost",3493);
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::slotConnected()
{
    textLines->append("CONNECTED");
}

void MainWindow::slotReadyRead()
{
    QTextStream in(client->ASocket);
    QString str;
    in >> str;
    textLines->append(str);
}

upsClient::upsClient(const QString &host, const int port)
{
    ASocket = new QTcpSocket(this);

    QObject::connect(ASocket, SIGNAL(connected()), this, SLOT(slotDoConnected()));
    QObject::connect(ASocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    QObject::connect(ASocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(slotError(QAbstractSocket::SocketError)));

    ASocket->connectToHost(host, port, QIODevice::ReadOnly/*Write*/);
}

upsClient::~upsClient()
{
    delete ASocket;
}

void upsClient::slotError(QAbstractSocket::SocketError err)
{

}
