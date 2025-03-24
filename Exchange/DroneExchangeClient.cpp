#include "DroneExchangeClient.h"


DroneExchangeClient::DroneExchangeClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &DroneExchangeClient::handleRead);
    //connect(socket, &QTcpSocket::error, this, &DroneExchangeClient::handleError);
    connect(socket, &QTcpSocket::connected, this, &DroneExchangeClient::handleConnected);
    connect(socket, &QTcpSocket::disconnected, this, &DroneExchangeClient::handleDisconnected);

    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, &QTimer::timeout, this, &DroneExchangeClient::reconnect);
}

void DroneExchangeClient::connectToServer(const QString &host, quint16 port) {
    m_address = host;
    m_port = port;
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
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray buffer = socket->readAll();
    QString data = QString::fromUtf8(buffer);

    // Поиск маркера (например, "\n")
    int markerPos = data.indexOf('\n');

    if (markerPos != -1) {
        QString completeMessage = data.left(markerPos);
        // Обработка сообщения
        emit messageReceived(completeMessage);
        qDebug() << completeMessage;
    }
}

void DroneExchangeClient::handleError(QAbstractSocket::SocketError) {
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
    socket->connectToHost(m_address, m_port);
}
