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
    DroneExchangeClient(QObject *parent = nullptr);

    void connectToServer(const QString& host, quint16 port);

    void sendMessage(const QString& message);

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
