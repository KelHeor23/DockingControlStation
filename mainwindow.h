#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "conn_settings.h"

#include <QMainWindow>
#include <QLabel>

#include "Exchange/DroneExchangeClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openConnectionSettings();
    void sendStartMsg();

private:
    Ui::MainWindow *ui;
    conn_settings *conn_set;

    QLabel *drone1_ConnectionStatus;
    QLabel *drone2_ConnectionStatus;

    DroneExchangeClient *drone1Client;
    DroneExchangeClient *drone2Client;
};
#endif // MAINWINDOW_H
