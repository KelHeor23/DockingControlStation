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

private:
    void setupDroneMsgReciveConn();
    void resetAllStatusLbl();
    void switchDroneRoles(bool makeDrone1Papa);

public slots:
    void openConnectionSettings(bool);
    void handlePapaIsD1(bool checked);
    void handlePapaIsD2(bool checked);
    void reconnect();
    void drone1Connected(bool succes);
    void drone2Connected(bool succes);
    void recieveStatusPapa(QString status);
    void recieveStatusMama(QString status);

    /// Слоты отправки сообщений
    void sendDockingMsg();
    void sendUnDockingMsg();
    void sendStopMsg();
    void sendDrone1MoveCargoCV();
    void sendDrone1MoveCargoCCV();
    void sendDrone1MoveCargoHome();
    void sendDrone2MoveCargoCV();
    void sendDrone2MoveCargoCCV();
    void sendDrone2MoveCargoHome();


private:
    Ui::MainWindow *ui;
    conn_settings *conn_settings_m;

    QLabel *drone1_ConnectionStatus;
    QLabel *drone2_ConnectionStatus;

    DroneExchangeClient *drone1Client;
    DroneExchangeClient *drone2Client;

    DroneExchangeClient *droneMama;
    DroneExchangeClient *dronePapa;

    QString red = "background-color: rgba(255, 0, 0, 75);";
    QString green = "background-color: rgba(0, 255, 0, 75);";
};
#endif // MAINWINDOW_H
