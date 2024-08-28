#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(ushort port = 4500, QObject *parent = nullptr);
    void disconnectALlClients();

private slots:
    void newClientConnectionReceived();
    void onClientDisconnected();
signals:
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);
private: //fields
    QTcpServer *_server;
    QList<QTcpSocket *> _clients;
private: //methods
    void setupServer(ushort port);
};

#endif // SERVERMANAGER_H
