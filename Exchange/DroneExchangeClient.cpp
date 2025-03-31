#include "DroneExchangeClient.h"


DroneExchangeClient::DroneExchangeClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &DroneExchangeClient::handleRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &DroneExchangeClient::handleError);
    connect(socket, &QTcpSocket::connected, this, &DroneExchangeClient::handleConnected);
    connect(socket, &QTcpSocket::disconnected, this, &DroneExchangeClient::handleDisconnected);

    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, &QTimer::timeout, this, &DroneExchangeClient::reconnect);
}

void DroneExchangeClient::connectToServer(const QString &host, quint16 port) {
    m_address = host;
    m_port = port;

    // Сбрасываем соединение перед новым подключением
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->abort();
    }

    socket->connectToHost(m_address, m_port);
    reconnectTimer->start(5000); // 5 секунд между попытками
}

void DroneExchangeClient::sendMessage(Commands message) {
    QByteArray block;
    block.append(static_cast<char>(message));
    block.append(static_cast<char>('\n'));
    socket->write(block);
}

void DroneExchangeClient::handleRead() {
    while (socket->canReadLine()) { // Читаем построчно
        QByteArray line = socket->readLine().trimmed();
        if (!line.isEmpty()) {
            emit messageReceived(QString::fromUtf8(line));
        }
    }
}

void DroneExchangeClient::handleError(QAbstractSocket::SocketError) {
    qWarning() << "Socket error:" << socket->errorString();
    reconnectTimer->start();
    emit connected(false);
}

void DroneExchangeClient::handleConnected() {
    reconnectTimer->stop();
    emit connected(true);
}

void DroneExchangeClient::handleDisconnected() {
    reconnectTimer->start();
    emit connected(false);
}

void DroneExchangeClient::reconnect() {
    qDebug() << "Attempting to reconnect...";
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost(m_address, m_port);
    }
}
