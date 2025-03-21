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

void DroneExchangeClient::sendMessage(const QString &message) {
    /*QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << message;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    out << message;*/
    socket->write(message.toUtf8());
}

void DroneExchangeClient::handleRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);

    if (nextBlockSize == 0) {
        if (socket->bytesAvailable() < sizeof(quint16)) {
            return;
        }
        in >> nextBlockSize;
    }

    if (socket->bytesAvailable() < nextBlockSize) {
        return;
    }

    QString message;
    in >> message;
    emit messageReceived(message);
    nextBlockSize = 0;
}

void DroneExchangeClient::handleError(QAbstractSocket::SocketError) {
    reconnectTimer->start();
}

void DroneExchangeClient::handleConnected() {
    reconnectTimer->stop();
}

void DroneExchangeClient::handleDisconnected() {
    reconnectTimer->start();
}

void DroneExchangeClient::reconnect() {
    socket->connectToHost(m_address, m_port);
}
