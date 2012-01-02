#ifndef QUPSDOCK_H
#define QUPSDOCK_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class qUPSDock : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit qUPSDock(QWidget *parent = 0);

private:
    QAction *open;
    QAction *close;
    QMenu *trayIconMenu;

signals:

public slots:

};

#endif // QUPSDOCK_H
