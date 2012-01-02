#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    trayIcon = new qUPSDock(this);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    upsClient *client = new upsClient("localhost", 3493, this);

    client->setPollTime(1500);
    client->setUserName("upsmaster");
    client->setPassword("20af73d00b");
    client->setLogin("myups");

    upsClientModel *model = new upsClientModel(client,this);
    ui->textLines->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (this->isVisible()) {
            this->hide();
        } else {
            this->show();
        }
    }
}
