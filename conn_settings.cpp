#include "conn_settings.h"
#include "ui_conn_settings.h"

#include <QRegularExpression>
#include <QTextDocument>

conn_settings::conn_settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::conn_settings)
{
    ui->setupUi(this);

    conSettings = new QSettings("drones", "conSettings", this);

    loadSettings();

    connect(ui->setConnValuesBtn, &QPushButton::clicked, this, &conn_settings::setConnValue);
    connect(ui->cancel, &QPushButton::clicked, this, &conn_settings::loadSettings);
}

conn_settings::~conn_settings()
{
    delete ui;
}

bool conn_settings::isValidIPAddress(const QString &ip)
{
    // Регулярное выражение для IPv4 или "localhost"
    QRegularExpression regex("^(localhost|((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))$");
    return regex.match(ip).hasMatch(); // Проверка соответствия
}

bool conn_settings::isValidIPPort(const QString &port)
{
    bool succes = true;
    int port_t = port.toInt(&succes);
    return succes && (port_t >= 0 && port_t <= 65535);
}

void conn_settings::setDefaultBorders()
{
    ui->Drone1IpAdressTxtEdt->setStyleSheet("QTextEdit { border: 0px solid black }");
    ui->Drone1PortTxtEdt->setStyleSheet("QTextEdit { border: 0px solid black }");
    ui->Drone2IpAdressTxtEdt->setStyleSheet("QTextEdit { border: 0px solid black }");
    ui->Drone2PortTxtEdt->setStyleSheet("QTextEdit { border: 0px solid black }");
}

void conn_settings::setConnValue()
{
    bool isFalse = false;

    if (!isValidIPAddress(ui->Drone1IpAdressTxtEdt->toPlainText())){
        ui->Drone1IpAdressTxtEdt->setStyleSheet("QTextEdit { border: 2px solid red; }");
        isFalse = true;
    }

    if (!isValidIPAddress(ui->Drone2IpAdressTxtEdt->toPlainText())){
        ui->Drone2IpAdressTxtEdt->setStyleSheet("QTextEdit { border: 2px solid red; }");
        isFalse = true;
    }

    if (!isValidIPPort(ui->Drone1PortTxtEdt->toPlainText())){
        ui->Drone1PortTxtEdt->setStyleSheet("QTextEdit { border: 2px solid red; }");
        isFalse = true;
    }

    if (!isValidIPPort(ui->Drone2PortTxtEdt->toPlainText())){
        ui->Drone2PortTxtEdt->setStyleSheet("QTextEdit { border: 2px solid red; }");
        isFalse = true;
    }

    if (!isFalse) {
        drone1IP    = ui->Drone1IpAdressTxtEdt->toPlainText();
        drone1Port  = ui->Drone1PortTxtEdt->toPlainText().toInt();
        drone2IP    = ui->Drone2IpAdressTxtEdt->toPlainText();
        drone2Port  = ui->Drone2PortTxtEdt->toPlainText().toInt();

        setDefaultBorders();
        saveSettings();

        emit newConnSettings();
    }
}

void conn_settings::loadSettings()
{
    drone1IP    = conSettings->value("drone1IP", "localhost").toString();
    drone1Port  = conSettings->value("drone1Port", 8021).toInt();
    drone2IP    = conSettings->value("drone2IP", "localhost").toString();;
    drone2Port  = conSettings->value("drone2Port", 8022).toInt();

    ui->Drone1IpAdressTxtEdt->setText(drone1IP);
    ui->Drone2IpAdressTxtEdt->setText(drone2IP);
    ui->Drone1PortTxtEdt->setText(QString::number(drone1Port));
    ui->Drone2PortTxtEdt->setText(QString::number(drone2Port));

    setDefaultBorders();
}

void conn_settings::saveSettings()
{
    conSettings->setValue("drone1IP", drone1IP);
    conSettings->setValue("drone1Port", drone1Port);
    conSettings->setValue("drone2IP", drone2IP);
    conSettings->setValue("drone2Port", drone2Port);
}

quint16 conn_settings::getDrone2Port() const
{
    return drone2Port;
}

QString conn_settings::getDrone2IP() const
{
    return drone2IP;
}

quint16 conn_settings::getDrone1Port() const
{
    return drone1Port;
}

QString conn_settings::getDrone1IP() const
{
    return drone1IP;
}
