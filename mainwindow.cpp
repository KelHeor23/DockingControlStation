#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , drone1_ConnectionStatus(new QLabel("drone 1"))
    , drone2_ConnectionStatus(new QLabel("drone 2"))
{
    ui->setupUi(this);

    drone1_ConnectionStatus->setStyleSheet("color: red;");
    drone2_ConnectionStatus->setStyleSheet("color: green;");

    ui->statusbar->addPermanentWidget(drone1_ConnectionStatus);
    ui->statusbar->addPermanentWidget(drone2_ConnectionStatus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

