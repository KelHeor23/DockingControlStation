#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conn_set(new class conn_settings)
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

    connect(ui->openConnectionSettings, SIGNAL(triggered(bool)), this, SLOT(openConnectionSettings()));
    connect(ui->startDocking, &QPushButton::clicked, this, &MainWindow::sendStartMsg);

    drone1Client->connectToServer("localhost", 8021);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openConnectionSettings()
{
    conn_set->show();
}

void MainWindow::sendStartMsg()
{
    drone1Client->sendMessage("1\n");
    drone2Client->sendMessage("1\n");
}

