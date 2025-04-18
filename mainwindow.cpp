#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conn_settings_m(new class conn_settings)
    , drone1_ConnectionStatus(new QLabel("Drone1 IP:"))
    , drone2_ConnectionStatus(new QLabel("Drone2 IP:"))
    , drone1Client(new DroneExchangeClient(this))
    , drone2Client(new DroneExchangeClient(this))
{
    ui->setupUi(this);

    ui->statusbar->addPermanentWidget(drone1_ConnectionStatus);
    ui->statusbar->addPermanentWidget(drone2_ConnectionStatus);

    connect(ui->openConnectionSettings, &QAction::triggered,                        this, &MainWindow::openConnectionSettings);
    connect(ui->startDockingBtn,        &QPushButton::clicked,                      this, &MainWindow::sendDockingMsg);
    connect(ui->undockingBtn,           &QPushButton::clicked,                      this, &MainWindow::sendUnDockingMsg);
    connect(ui->stopBtn,                &QPushButton::clicked,                      this, &MainWindow::sendStopMsg);
    connect(ui->drone1MoveCargoCV,      &QPushButton::clicked,                      this, &MainWindow::sendDrone1MoveCargoCV);
    connect(ui->drone2MoveCargoCV,      &QPushButton::clicked,                      this, &MainWindow::sendDrone2MoveCargoCV);
    connect(ui->drone1MoveCargoCCV,     &QPushButton::clicked,                      this, &MainWindow::sendDrone1MoveCargoCCV);
    connect(ui->drone2MoveCargoCCV,     &QPushButton::clicked,                      this, &MainWindow::sendDrone2MoveCargoCCV);
    connect(ui->drone1MoveCargoHome,    &QPushButton::clicked,                      this, &MainWindow::sendDrone1MoveCargoHome);
    connect(ui->drone2MoveCargoHome,    &QPushButton::clicked,                      this, &MainWindow::sendDrone2MoveCargoHome);
    connect(ui->papaIsD1,               &QAction::triggered,                        this, &MainWindow::handlePapaIsD1);
    connect(ui->papaIsD2,               &QAction::triggered,                        this, &MainWindow::handlePapaIsD2);
    connect(ui->reconnect,              &QAction::toggled,                          this, &MainWindow::reconnect);
    connect(conn_settings_m,            &conn_settings::newConnSettings,            this, &MainWindow::reconnect);
    connect(drone1Client,               &DroneExchangeClient::connected,            this, &MainWindow::drone1Connected);
    connect(drone2Client,               &DroneExchangeClient::connected,            this, &MainWindow::drone2Connected);

    reconnect();
    resetAllStatusLbl();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDroneMsgReciveConn()
{
    if (dronePapa)
        connect(dronePapa, &DroneExchangeClient::messageReceived,    this, &MainWindow::recieveStatusPapa);

    if (droneMama)
        connect(droneMama, &DroneExchangeClient::messageReceived,    this, &MainWindow::recieveStatusMama);
}

void MainWindow::resetAllStatusLbl()
{
    ui->dronPapaReadinessStatus->setStyleSheet(red);
    ui->dronMamaReadinessStatus->setStyleSheet(red);
    ui->rodExtention->setStyleSheet(red);
    ui->hooksLock->setStyleSheet(red);
    ui->pullingUp->setStyleSheet(red);
    ui->transferDone->setStyleSheet(red);
}

void MainWindow::openConnectionSettings(bool)
{
    conn_settings_m->show();
}


void MainWindow::switchDroneRoles(bool makeDrone1Papa)
{    
    // 1. Сначала сбрасываем текущие роли
    if (dronePapa) {
        dronePapa = nullptr;
    }
    if (droneMama) {
        droneMama = nullptr;
    }


    // 2. Устанавливаем новые роли
    if (makeDrone1Papa) {
        if (!drone1TypeSet){
            drone1Client->sendMessage(DroneExchangeClient::StartPapa);
            setDroneType(drone1Client, ui->groupBox, "Дрон 1", true);
            drone1TypeSet = true;
        }
        if (!drone2TypeSet){
            drone2Client->sendMessage(DroneExchangeClient::StartMama);
            setDroneType(drone2Client, ui->groupBox_2, "Дрон 2", false);
            drone2TypeSet = true;
        }
        ui->papaIsD1->setChecked(true);
        ui->papaIsD2->setChecked(false);
    } else {
        if (!drone1TypeSet){
            drone1Client->sendMessage(DroneExchangeClient::StartMama);
            setDroneType(drone1Client, ui->groupBox, "Дрон 1", false);
            drone1TypeSet = true;
        }
        if (!drone2TypeSet){
            drone2Client->sendMessage(DroneExchangeClient::StartPapa);
            setDroneType(drone2Client, ui->groupBox_2, "Дрон 2", true);
            drone2TypeSet = true;
        }
        ui->papaIsD1->setChecked(false);
        ui->papaIsD2->setChecked(true);
    }

    // 3. Обновляем соединения для обработки сообщений
    setupDroneMsgReciveConn();
    resetAllStatusLbl();
}

void MainWindow::handlePapaIsD1(bool checked)
{
    switchDroneRoles(true);
}

void MainWindow::handlePapaIsD2(bool checked)
{
    switchDroneRoles(false);
}

void MainWindow::reconnect()
{
    drone1Client->connectToServer(conn_settings_m->getDrone1IP(), conn_settings_m->getDrone1Port());
    drone2Client->connectToServer(conn_settings_m->getDrone2IP(), conn_settings_m->getDrone2Port());

    drone1_ConnectionStatus->setText("Drone1 IP:" + conn_settings_m->getDrone1IP() + ":" + QString::number(conn_settings_m->getDrone1Port()));
    drone2_ConnectionStatus->setText("Drone2 IP:" + conn_settings_m->getDrone2IP() + ":" + QString::number(conn_settings_m->getDrone2Port()));
}

void MainWindow::drone1Connected(bool succes)
{
    if (succes) {
        drone1_ConnectionStatus->setStyleSheet("color: orange;");
        drone1Client->sendMessage(DroneExchangeClient::GetCurDocking);
        connect(drone1Client,               &DroneExchangeClient::messageReceived,      this, &MainWindow::recieveMsgDrone1);
    } else {
        drone1_ConnectionStatus->setStyleSheet("color: red;");
        drone1TypeSet = false;
        if (drone1Client == dronePapa)
            dronePapa = nullptr;
        else if (drone1Client == droneMama)
            droneMama = nullptr;
    }
}

void MainWindow::drone2Connected(bool succes)
{
    if (succes) {
        drone2_ConnectionStatus->setStyleSheet("color: orange;");
        drone2Client->sendMessage(DroneExchangeClient::GetCurDocking);
        connect(drone2Client,               &DroneExchangeClient::messageReceived,      this, &MainWindow::recieveMsgDrone2);
    } else {
        drone2_ConnectionStatus->setStyleSheet("color: red;");
        drone2TypeSet = false;
        if (drone2Client == dronePapa)
            dronePapa = nullptr;
        else if (drone2Client == droneMama)
            droneMama = nullptr;
    }
}

void MainWindow::recieveMsgDrone1(QString msg)
{
    if (msg.size() == 2)
    {
        if (msg[0] == static_cast<char>(DroneExchangeClient::GetCurDocking)) {
            if (msg[1] == static_cast<char>(DroneExchangeClient::Mama)){
                setDroneType(drone1Client, ui->groupBox, "Дрон 1", false);
                drone1TypeSet = true;
            } else if (msg[1] == static_cast<char>(DroneExchangeClient::Papa)) {
                setDroneType(drone1Client, ui->groupBox, "Дрон 1", true);
                drone1TypeSet = true;
            }
        }
        disconnect(drone1Client,               &DroneExchangeClient::messageReceived,      this, &MainWindow::recieveMsgDrone1);

        setupDroneMsgReciveConn();
    } else {
        drone1Client->sendMessage(DroneExchangeClient::GetCurDocking);
    }
}

void MainWindow::recieveMsgDrone2(QString msg)
{
    if (msg.size() == 2)
    {
        if (msg[0] == static_cast<char>(DroneExchangeClient::GetCurDocking)) {
            if (msg[1] == static_cast<char>(DroneExchangeClient::Mama)){
                setDroneType(drone2Client, ui->groupBox_2, "Дрон 2", false);
                drone2TypeSet = true;
            } else if (msg[1] == static_cast<char>(DroneExchangeClient::Papa)) {
                setDroneType(drone2Client, ui->groupBox_2, "Дрон 2", true);
                drone2TypeSet = true;
            }
        }

        disconnect(drone2Client,               &DroneExchangeClient::messageReceived,      this, &MainWindow::recieveMsgDrone2);

        setupDroneMsgReciveConn();
    } else {
        drone2Client->sendMessage(DroneExchangeClient::GetCurDocking);
    }
}

void MainWindow::recieveStatusPapa(QString status)
{
    if (status.size() != 3)
        return;

    if (status[0] == '0')
        ui->dronPapaReadinessStatus->setStyleSheet(red);
    else if (status[0] == '1')
        ui->dronPapaReadinessStatus->setStyleSheet(green);

    if (status[1] == '0')
        ui->rodExtention->setStyleSheet(red);
    else if (status[0] == '1')
        ui->rodExtention->setStyleSheet(green);

    if (status[2] == '0')
        ui->pullingUp->setStyleSheet(red);
    else if (status[0] == '1')
        ui->pullingUp->setStyleSheet(green);

    if (drone2Client == dronePapa)
        drone2_ConnectionStatus->setStyleSheet("color: green;");
    else if (drone1Client == dronePapa)
        drone1_ConnectionStatus->setStyleSheet("color: green;");

    qDebug() << "Papa: " << status;
}

void MainWindow::recieveStatusMama(QString status)
{
    if (status.size() != 3)
        return;

    if (status[0] == '0')
        ui->dronMamaReadinessStatus->setStyleSheet(red);
    else if (status[0] == '1')
        ui->dronMamaReadinessStatus->setStyleSheet(green);

    if (status[1] == '0')
        ui->hooksLock->setStyleSheet(red);
    else if (status[0] == '1')
        ui->hooksLock->setStyleSheet(green);

    if (status[2] == '1')
        ui->transferDone->setStyleSheet(green);

    if (drone2Client == droneMama)
        drone2_ConnectionStatus->setStyleSheet("color: green;");
    else if (drone1Client == droneMama)
        drone1_ConnectionStatus->setStyleSheet("color: green;");

    qDebug() << "Mama: " << status;
}

void MainWindow::setDroneType(DroneExchangeClient *&droneTarget, QGroupBox *groupBox, const QString &droneName, bool isPapa)
{
    if (isPapa) {
        dronePapa = droneTarget;
        groupBox->setTitle(QString("%1. Папа").arg(droneName));
    } else {
        droneMama = droneTarget;
        groupBox->setTitle(QString("%1. Мама").arg(droneName));
    }
}

void MainWindow::sendDockingMsg()
{
    drone1Client->sendMessage(DroneExchangeClient::Docking);
    drone2Client->sendMessage(DroneExchangeClient::Docking);
}

void MainWindow::sendUnDockingMsg()
{
    drone1Client->sendMessage(DroneExchangeClient::Undocking);
    drone2Client->sendMessage(DroneExchangeClient::Undocking);

    ui->transferDone->setStyleSheet(red);
}

void MainWindow::sendStopMsg()
{
    drone1Client->sendMessage(DroneExchangeClient::Stop);
    drone2Client->sendMessage(DroneExchangeClient::Stop);
}

void MainWindow::sendDrone1MoveCargoCV()
{
    drone1Client->sendMessage(DroneExchangeClient::CargoMoveCV);
}

void MainWindow::sendDrone1MoveCargoCCV()
{
    drone1Client->sendMessage(DroneExchangeClient::CargoMoveCCV);
}

void MainWindow::sendDrone1MoveCargoHome()
{
    drone1Client->sendMessage(DroneExchangeClient::cargoMoveHome);
}

void MainWindow::sendDrone2MoveCargoCV()
{
    drone2Client->sendMessage(DroneExchangeClient::CargoMoveCV);
}

void MainWindow::sendDrone2MoveCargoCCV()
{
    drone2Client->sendMessage(DroneExchangeClient::CargoMoveCCV);
}

void MainWindow::sendDrone2MoveCargoHome()
{
    drone2Client->sendMessage(DroneExchangeClient::cargoMoveHome);
}
