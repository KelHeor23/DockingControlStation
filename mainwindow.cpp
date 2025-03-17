#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , conn_set(new class conn_settings)
    , drone1_ConnectionStatus(new QLabel("drone 1"))
    , drone2_ConnectionStatus(new QLabel("drone 2"))
{
    ui->setupUi(this);

    drone1_ConnectionStatus->setStyleSheet("color: red;");
    drone2_ConnectionStatus->setStyleSheet("color: green;");

    ui->statusbar->addPermanentWidget(drone1_ConnectionStatus);
    ui->statusbar->addPermanentWidget(drone2_ConnectionStatus);

    connect(ui->openConnectionSettings, SIGNAL(triggered(bool)), this, SLOT(openConnectionSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openConnectionSettings()
{
    conn_set->show();
}

