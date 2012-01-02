#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "upsclient.h"
#include "qupsdock.h"

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
    qUPSDock *trayIcon;

private slots:
    void trayIconClicked(QSystemTrayIcon::ActivationReason);
};

#endif // MAINWINDOW_H
