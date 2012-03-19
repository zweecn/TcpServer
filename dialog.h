#ifndef DIALOG_H
#define DIALOG_H
#include <QtNetwork>
#include <QtGui>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    void createHostInfoGroupBox();
    QGroupBox* hostInfoGroupBox;
    QLabel* hostLabel;
    QLabel* portLabel;
    QLabel* sendMessageLabel;
    QLabel* receiveMessageLabel;
    QLineEdit* hostEdit;
    QLineEdit* portEdit;
    QLineEdit* sendMessageEdit;
    QLineEdit* receiveMessageEdit;
    QPushButton* listenButton;
    QPushButton* stopButton;
    QPushButton* sendButton;
    QPushButton* disConnectButton;
    QLabel* statusLabel;
private slots:
    void listen();
    void stopListen();
    void acceptConnect();
    void readData();
    void sendData();
    void disConnect();
    void displayDisconnect();
    void displayError(QAbstractSocket::SocketError);
};

#endif // DIALOG_H
