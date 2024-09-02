#include "ServerManager.h"

ServerManager::ServerManager(ushort port, QObject *parent)
    : QObject{parent}
{
    setupServer(port);
}

void ServerManager::disconnectALlClients()
{
    // TODO: revise this maybe the issue
    for (QTcpSocket *client : std::as_const(_clients)){
        client->disconnectFromHost();
        if (client->state() != QTcpSocket::UnconnectedState) {
            client->waitForDisconnected();
        }
    }
}

void ServerManager::notifyOtherClients(QString prevName, QString name)
{
    auto message = _protocol.setClientNameMessage(prevName, name);
    foreach (auto cl, _clients) {
        auto clientName = cl->property("clientName").toString();
        if(clientName != name){
            cl->write(message);
        }
    }
}

void ServerManager::onTextForOtherClients(QString message, QString receiver, QString sender)
{
    auto msg = _protocol.textMessage(message, receiver);
    if(receiver == "All"){
        foreach (auto cl, _clients) {
            auto clientName = cl->property("clientName").toString();
            if(clientName != sender){
                cl->write(msg);
            }
        }
    } else {
        foreach (auto cl, _clients) {
            auto clientName = cl->property("clientName").toString();
            if(clientName == receiver){
                cl->write(msg);
                return;
            }
        }
    }
}

void ServerManager::newClientConnectionReceived()
{
    auto client = _server->nextPendingConnection();
    auto id = _clients.count() + 1;
    auto clientName = QString("Client (%1)").arg(id);
    client->setProperty("id", id);
    client->setProperty("clientName", clientName);
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onClientDisconnected);
    emit newClientConnected(client);
    if (id > 1) {
        auto message = _protocol.setConnectionACKMessage(clientName, _clients.keys());
        client->write(message);

        auto newClientMessage = _protocol.setNewClientMessage(clientName);
        foreach (auto cl, _clients){
            cl->write(newClientMessage);
        }
    }
    _clients[clientName] = client;
}

void ServerManager::onClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket *>(sender());
    auto clientName = client->property("clientName").toString();
    _clients.remove(clientName);
    auto message = _protocol.setClientDisconnectedMessage(clientName);
    foreach (auto cl, _clients) {
        cl->write(message);
    }
    emit clientDisconnected(client);
}
void ServerManager::setupServer(ushort port)
{
    _server = new QTcpServer(this);
    // Bind the server to all available network interafaces
    QHostAddress serverAddress = QHostAddress::Any;
    if(!_server->listen(serverAddress, port)){
        qDebug() << "Server could not start";
    }
    else {
        qDebug() << "Server started and listening on all interfaces, port:" << port;
        connect(_server, &QTcpServer::newConnection, this, &ServerManager::newClientConnectionReceived);
    }
}
