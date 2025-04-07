#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "conn_settings.h"

#include <QMainWindow>
#include <QLabel>

#include "Exchange/DroneExchangeClient.h"
#include "qgroupbox.h"

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
    void recieveMsgDrone1(QString status);
    void recieveMsgDrone2(QString status);
    void recieveStatusPapa(QString status);
    void recieveStatusMama(QString status);
    void setDroneType(DroneExchangeClient*& droneTarget, QGroupBox* groupBox, const QString& droneName, bool isPapa);

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

    DroneExchangeClient *drone1Client = nullptr;
    DroneExchangeClient *drone2Client = nullptr;

    DroneExchangeClient *droneMama = nullptr;
    DroneExchangeClient *dronePapa = nullptr;

    QString red = "background-color: rgba(255, 0, 0, 75); font-size: 25px;";
    QString green = "background-color: rgba(0, 255, 0, 75); font-size: 25px;";

    bool drone1TypeSet = false;
    bool drone2TypeSet = false;
};
#endif // MAINWINDOW_H
