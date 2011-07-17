#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "upsclient.h"

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
    void slotError(QAbstractSocket::SocketError);
};

#endif // MAINWINDOW_H
