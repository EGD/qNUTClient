#include "qupsdock.h"

qUPSDock::qUPSDock(QWidget *parent) : QSystemTrayIcon (parent)
{
    open = new QAction(tr("&Open"), this);
    connect(open, SIGNAL(triggered()), parent, SLOT(show()));

    close = new QAction(tr("&Quit"), this);
    connect(close, SIGNAL(triggered()), parent, SLOT(quit()));


    trayIconMenu = new QMenu(parent);

    trayIconMenu->addAction(open);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(close);

//    connect(
//                trayIcon,
//                SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//                this,
//                SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
//                );

   this->setContextMenu(trayIconMenu);
   this->setIcon(QIcon(":/icon/lo32-app-knutclient.png"));

   this->show();
}
