#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QDesktopServices>
#include <QProcess>
#include <QUrl>
#include <QFileInfo>
#include <QOperatingSystemVersion>

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::textMessageReceived, this, &ClientChatWidget::textMessageReceived);
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::onClientNameChanged);
    connect(_client, &ClientManager::statusChanged, this, &ClientChatWidget::statusChanged);
    connect(_client, &ClientManager::initReceivingFile, this, &ClientChatWidget::onInitReceivingFile);
    connect(_client, &ClientManager::fileSaved, this, &ClientChatWidget::onFileSaved);
    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);

    dir.mkdir(_client->name());
    dir.setPath("./" + _client->name());
}

void ClientChatWidget::disconnect()
{
    _client->disconnectFromHost();
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _client->sendMessage(message);
    ui->lnMessage->setText("");
    ui->lstMessages->addItem(message);
}

void ClientChatWidget::clientDisconnected()
{
    ui->wdgSend->setEnabled(false);
}

void ClientChatWidget::textMessageReceived(QString message, QString receiver)
{
    if(receiver == "Server" || receiver == "All"){
        ui->lstMessages->addItem(message);
    }
    if(receiver != "Server"){
        emit textForOtherClients(message, receiver, _client->name());
    }
}

void ClientChatWidget::onTyping()
{
    emit isTyping(QString("%1 is typing...").arg(_client->name()));
}

void ClientChatWidget::onInitReceivingFile(QString clientName, QString fileName, qint64 fileSize)
{
    auto message = QString("Client (%1) wants to send a file. Do you want to accept it?\nFile Name:%2\nFile Size: %3 bytes")
    .arg(clientName, fileName)
        .arg(fileSize);

    QMessageBox msgBox(QMessageBox::Question, "Receiving File", message, QMessageBox::Yes | QMessageBox::No, this);

    // Calculate the position to center the message box relative to the main window
    QRect geom = this->geometry();
    int x = geom.x() + (geom.width() - msgBox.width()) / 2;
    int y = geom.y() + (geom.height() - msgBox.height()) / 2;
    msgBox.move(x, y);

    int result = msgBox.exec();

    if (result == QMessageBox::Yes) {
        _client->sendAcceptFile();
    } else {
        _client->sendRejectFile();
    }
}

void ClientChatWidget::onFileSaved(QString path)
{
    auto message = QString("File saved here:\n%1").arg(path);
    QMessageBox::information(this, "File saved", message);
}

void ClientChatWidget::on_lblOpenFolder_linkActivated(const QString &link)
{
    // Assuming _client->name() returns just the directory name, not the full path
    QString directoryName = _client->name();

    // Construct the full path by combining with a base directory
    QString basePath = QDir::currentPath(); // or use another appropriate base path
    QString fullPath = QDir::cleanPath(basePath + QDir::separator() + directoryName);

    // Check if the directory exists and is accessible
    if (QFileInfo::exists(fullPath) && QFileInfo(fullPath).isDir()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    } else {
        qDebug() << "Directory does not exist or is not accessible:" << fullPath;
    }
}

void ClientChatWidget::onClientNameChanged(QString prevName, QString name)
{
    QFile::rename(dir.canonicalPath(), name);
    emit clientNameChanged(prevName, name);
}

