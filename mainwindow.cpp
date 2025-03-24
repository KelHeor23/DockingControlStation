#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conn_settings_m(new class conn_settings)
    , drone1_ConnectionStatus(new QLabel("drone 1"))
    , drone2_ConnectionStatus(new QLabel("drone 2"))
    , drone1Client(new DroneExchangeClient(this))
    , drone2Client(new DroneExchangeClient(this))
{
    ui->setupUi(this);

    drone1_ConnectionStatus->setStyleSheet("color: red;");
    drone2_ConnectionStatus->setStyleSheet("color: green;");

    ui->statusbar->addPermanentWidget(drone1_ConnectionStatus);
    ui->statusbar->addPermanentWidget(drone2_ConnectionStatus);

    connect(ui->openConnectionSettings, &QAction::triggered,                this, &MainWindow::openConnectionSettings);
    connect(ui->startDockingBtn,        &QPushButton::clicked,              this, &MainWindow::sendDockingMsg);
    connect(ui->undockingBtn,           &QPushButton::clicked,              this, &MainWindow::sendUnDockingMsg);
    connect(ui->stopBtn,                &QPushButton::clicked,              this, &MainWindow::sendStopMsg);
    connect(ui->drone1MoveCargoCV,      &QPushButton::clicked,              this, &MainWindow::sendDrone1MoveCargoCV);
    connect(ui->drone2MoveCargoCV,      &QPushButton::clicked,              this, &MainWindow::sendDrone2MoveCargoCV);
    connect(ui->drone1MoveCargoCCV,     &QPushButton::clicked,              this, &MainWindow::sendDrone1MoveCargoCCV);
    connect(ui->drone2MoveCargoCCV,     &QPushButton::clicked,              this, &MainWindow::sendDrone2MoveCargoCCV);
    connect(ui->drone1MoveCargoHome,    &QPushButton::clicked,              this, &MainWindow::sendDrone1MoveCargoHome);
    connect(ui->drone2MoveCargoHome,    &QPushButton::clicked,              this, &MainWindow::sendDrone2MoveCargoHome);
    connect(ui->papaIsD1,               &QAction::toggled,                  this, &MainWindow::handlePapaIsD1);
    connect(ui->papaIsD2,               &QAction::toggled,                  this, &MainWindow::handlePapaIsD2);
    connect(conn_settings_m,            &conn_settings::newConnSettings,    this, &MainWindow::reconnect);

    reconnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openConnectionSettings(bool)
{
    conn_settings_m->show();
}

void MainWindow::handlePapaIsD1(bool checked)
{
    if (checked) {
        ui->papaIsD2->setChecked(false);
        drone1Client->sendMessage(DroneExchangeClient::StartPapa);
        drone2Client->sendMessage(DroneExchangeClient::StartMama);
    }
}

void MainWindow::handlePapaIsD2(bool checked)
{
    if (checked) {
        ui->papaIsD1->setChecked(false);
        drone1Client->sendMessage(DroneExchangeClient::StartMama);
        drone2Client->sendMessage(DroneExchangeClient::StartPapa);
    }
}

void MainWindow::reconnect()
{
    drone1Client->connectToServer(conn_settings_m->getDrone1IP(), conn_settings_m->getDrone1Port());
    drone2Client->connectToServer(conn_settings_m->getDrone2IP(), conn_settings_m->getDrone2Port());
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
