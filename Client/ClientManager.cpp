#include "ClientManager.h"
#include <QAudioFormat>
#include <QMediaDevices>

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
    _ip(ip),
    _port(port)
{
    setupClient();
    setupAudio();
}

void ClientManager::connectToServer()
{
    if(_socket->state() != QTcpSocket::ConnectedState){
        _socket->connectToHost(_ip, _port);
    }
}

void ClientManager::sendMessage(QString message, QString receiver)
{
    if(message.trimmed().length() > 0){
        _socket->write(_protocol.textMessage(message, receiver));
    }
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));
}

void ClientManager::sendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());
}

void ClientManager::sendInitSendingFile(QString fileName)
{
    _tmpFileName = fileName;
    _socket->write(_protocol.setInitSendingFileMessage(fileName));
}

void ClientManager::sendAcceptFile()
{
    _socket->write(_protocol.setAcceptFileMessage());
}

void ClientManager::sendRejectFile()
{
    _socket->write(_protocol.setRejectFileMessage());
}

void ClientManager::readyRead()
{
    auto data = _socket->readAll();
    _protocol.loadData(data);
    switch (_protocol.type()) {
    case ChatProtocol::Text:
        emit textMessageReceived(_protocol.message());
        break;
    case ChatProtocol::SetName:
        emit nameChanged(_protocol.name());
        break;
    case ChatProtocol::SetStatus:
        emit statusChanged(_protocol.status());
        break;
    case ChatProtocol::IsTyping:
        emit isTyping();
        break;
    case ChatProtocol::InitSendingFile:
        emit initReceivingFile(_protocol.name(), _protocol.fileName(), _protocol.fileSize());
        break;
    case ChatProtocol::AcceptSendingFile:
        sendFile();
        break;
    case ChatProtocol::RejectSendingFile:
        emit rejectReceivingFile();
        break;
    case ChatProtocol::ConnectionACK:
        emit connectionACK(_protocol.myName(), _protocol.clientsName());
        break;
    case ChatProtocol::NewClient:
        emit newClientConnectedToServer(_protocol.clientName());
        break;
    case ChatProtocol::ClientDisconnected:
        emit clientDisconnected(_protocol.clientName());
        break;
    case ChatProtocol::ClientName:
        emit clientNameChanged(_protocol.prevName(), _protocol.clientName());
        break;
    default:
        break;
    }
}

void ClientManager::onAudioDataAvailable()
{
    // QByteArray audioData = _audioDevice->readAll();
    // _udpSocket->writeDatagram(audioData, _recipientIP, _recipientPort); // Send to the recipient
}

void ClientManager::setupClient()
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
}

void ClientManager::sendFile()
{
    _socket->write(_protocol.setFileMessage(_tmpFileName));
}

void ClientManager::startVoiceCommunication()
{
    // Ensure audio input is properly started
    // if (_audioInput && !_audioInput->state() == QAudio::ActiveState) {
    //     _audioDevice = _audioInput->start();  // Start capturing audio data
    //     connect(_audioDevice, &QIODevice::readyRead, this, &ClientManager::onAudioDataAvailable);
    // }
}

void ClientManager::stopVoiceCommunication()
{
    // _audioInput->stop();
}

void ClientManager::setupAudio()
{
    // Set up the audio format
    QAudioFormat format;
    format.setSampleRate(8000);  // Sample rate suitable for voice
    format.setChannelCount(1);   // Mono
    format.setSampleFormat(QAudioFormat::Int16);  // 16-bit samples

    QAudioDevice info = QMediaDevices::defaultAudioInput();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.preferredFormat();  // Use the nearest supported format
    }

    // Create QAudioSource for capturing audio
    _audioSource = new QAudioSource(format, this);

    // Create UDP socket for transmitting audio data
    _udpSocket = new QUdpSocket(this);

    connect(_audioSource, &QAudioSource::stateChanged, this, &ClientManager::handleStateChanged);
}

void ClientManager::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
    case QAudio::IdleState:
        // Finished recording, so stop capturing audio
        stopVoiceCommunication();
        break;

    case QAudio::StoppedState:
        if (_audioSource->error() != QAudio::NoError) {
            // Handle error
            qWarning() << "Audio input error occurred";
        }
        break;

    default:
        break;
    }
}
