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
        GetCurDocking   = 0x32, // Запрос текущего типа
        StartMama       = 0xF1, // Задать тип мама
        StartPapa       = 0xF2, // Задать тип папа
        Docking         = 0xF3, // Начать стыковку
        Undocking       = 0xF4, // Начать расстыковку
        Stop            = 0xF5, // Остановить все сервы
        CargoMoveCV     = 0xF6, // Сдвинуть телегу вперед
        CargoMoveCCV    = 0xF7, // Сдвинуть телегу назад
        cargoMoveHome   = 0xF8  // Сдвинуть телегу к домашней точке
    };

    enum DockerType{
        NoCreated       = 0x30,
        Mama            = 0x31,
        Papa            = 0x32
    };

    DroneExchangeClient(QObject *parent = nullptr);

    void connectToServer(const QString& host, quint16 port);

    void sendMessage(Commands message);

signals:
    void messageReceived(const QString& message);
    void connected(bool);

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
