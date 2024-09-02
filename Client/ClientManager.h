#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ChatProtocol.h"

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAudioSource>
#include <QUdpSocket>
class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);

    void connectToServer();

    void sendMessage(QString message, QString receiver);
    void sendName(QString name);
    void sendStatus(ChatProtocol::Status status);
    void sendIsTyping();

    void sendInitSendingFile(QString fileName);
    void sendAcceptFile();
    void sendRejectFile();

    void startVoiceCommunication();
    void stopVoiceCommunication();

signals:
    void connected();
    void disconnected();
    void textMessageReceived(QString message);
    void isTyping();
    void nameChanged(QString name);
    void statusChanged(ChatProtocol::Status status);
    void initReceivingFile(QString clientName, QString fileName, qint64 fileSize);
    void rejectReceivingFile();

    void connectionACK(QString myName, QStringList clientsName);
    void newClientConnectedToServer(QString clientName);
    void clientNameChanged(QString prevName, QString clientName);
    void clientDisconnected(QString clientName);

private slots:
    void readyRead();
    void onAudioDataAvailable();
    void handleStateChanged(QAudio::State newState);

private: //fields
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
    ChatProtocol _protocol;
    QString _tmpFileName;

    // Voice communication fields
    QAudioSource *_audioSource;
    QUdpSocket *_udpSocket;

    QHostAddress _recipientIP;  // IP address of the recipient
    quint16 _recipientPort;     // Port for vocie communication

private: //methods
    void setupClient();
    void sendFile();
    void setupAudio();
};

#endif // CLIENTMANAGER_H
