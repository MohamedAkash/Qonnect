#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);

signals:

private:
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
};

#endif // CLIENTMANAGER_H
