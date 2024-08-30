#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClientChatWidget.h"
#include "ServerManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);
    void on_btnDisconnectAll_clicked();
    void setClientName(QString prevName, QString name);
    void setClientStatus(ChatProtocol::Status status);
    void on_tbChats_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    ServerManager *_server;

private: //methods
    void setupServer();
};
#endif // MAINWINDOW_H
