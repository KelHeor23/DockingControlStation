#ifndef DRONEEXCHANGECLIENT_H
#define DRONEEXCHANGECLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDataStream>
#include <QByteArray>

class DroneExchangeClient : public QObject
{
    Q_OBJECT
public:

    enum Commands {
        None            = 0x0,
        StartMama       = 0xF1,
        StartPapa       = 0xF2,
        Docking         = 0xF3,
        Undocking       = 0xF4,
        Stop            = 0xF5,
        CargoMoveCV     = 0xF6,
        CargoMoveCCV    = 0xF7,
        cargoMoveHome   = 0xF8
    };

    DroneExchangeClient(QObject *parent = nullptr);

    void connectToServer(const QString& host, quint16 port);

    void sendMessage(Commands message);

signals:
    void messageReceived(const QString& message);

private slots:
    void handleRead();

    void handleError(QAbstractSocket::SocketError);

    void handleConnected();

    void handleDisconnected();

    void reconnect();

private:
    QTcpSocket* socket;
    QTimer* reconnectTimer;
    quint16 nextBlockSize = 0;

    QString m_address = "localhost";
    quint16 m_port = 8021;
};

#endif // DRONEEXCHANGECLIENT_H
