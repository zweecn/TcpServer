#include "dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    createHostInfoGroupBox();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(hostInfoGroupBox);
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
    resize(400, 200);
    setWindowTitle(tr("Server"));

    tcpSocket = NULL;
    connect(listenButton, SIGNAL(clicked()), this, SLOT(listen()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopListen()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
    connect(disConnectButton, SIGNAL(clicked()), this, SLOT(disConnect()));
}


Dialog::~Dialog()
{

}

void Dialog::listen()
{
    statusLabel->setText(tr("Listening..."));
    tcpServer = new QTcpServer(this);
    QHostAddress host = QHostAddress(hostEdit->text());
    quint16 port = portEdit->text().toInt();
    if (!tcpServer->listen(host, port))
    {
        QMessageBox::critical(this, "Error", tcpServer->errorString());
        close();
    }
    listenButton->setEnabled(false);
    stopButton->setEnabled(true);
    portEdit->setEnabled(false);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnect()));
}

void Dialog::stopListen()
{
    if (tcpSocket != NULL)
    {
        tcpSocket->disconnectFromHost();
    }
    statusLabel->setText(tr("Stoping listen..."));
    tcpServer->close();
    listenButton->setEnabled(true);
    stopButton->setEnabled(false);
    sendButton->setEnabled(false);
    portEdit->setEnabled(true);
    sendMessageEdit->setEnabled(false);
    statusLabel->setText("Stop succeed.");
}

void Dialog::acceptConnect()
{
    sendMessageEdit->setEnabled(false);
    disConnectButton->setEnabled(true);
    statusLabel->setText(tr("Accepting connection..."));
    tcpSocket = tcpServer->nextPendingConnection();
    sendButton->setEnabled(true);
    sendMessageEdit->setEnabled(true);
    qDebug() << "Accept connectiong.";
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(displayDisconnect()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displayError(QAbstractSocket::SocketError)));
    statusLabel->setText(tr("Connected."));
}

void Dialog::readData()
{
    statusLabel->setText(tr("Reading data..."));
    qDebug() << "In readData, IP=" << tcpSocket->peerAddress().toString()
             << " Port=" << tcpSocket->peerPort();
    QString recvStr;
    recvStr = tcpSocket->readAll();
    qDebug() << "Receive: " + recvStr;
    receiveMessageEdit->setText(recvStr);
    statusLabel->setText(tr("Read succeed. Client address %1:%2")
                         .arg(tcpSocket->peerAddress().toString())
                         .arg(tcpSocket->peerPort()));
}

void Dialog::sendData()
{
    sendMessageEdit->setEnabled(false);
    static int i = 0;
    QString sendStr = sendMessageEdit->text();
    statusLabel->setText(tr("Sending..."));
    tcpSocket->write(sendStr.toAscii());
    i++;
    sendMessageEdit->setEnabled(true);
    statusLabel->setText(tr("Send succeed."));
}

void Dialog::displayDisconnect()
{
    qDebug() << "Disconnected.";
    sendButton->setEnabled(false);
    statusLabel->setText(tr("Disconnected."));
    disConnectButton->setEnabled(false);
}
void Dialog::disConnect()
{
    statusLabel->setText(tr("Disconnecting..."));
    tcpSocket->disconnectFromHost();
}


void Dialog::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    statusLabel->setText(tcpSocket->errorString());
    sendButton->setEnabled(false);
    stopButton->setEnabled(true);
    sendMessageEdit->setEnabled(false);
    disConnectButton->setEnabled(false);
}

void Dialog::createHostInfoGroupBox()
{
    hostInfoGroupBox = new QGroupBox(tr("Host Info"));
    hostLabel = new QLabel(tr("Host"));
    portLabel = new QLabel(tr("Port"));
    hostEdit = new QLineEdit(tr("localhost"));
    hostEdit->setEnabled(false);
    portEdit = new QLineEdit(tr("6666"));
    sendMessageLabel = new QLabel(tr("Send Message"));
    receiveMessageLabel = new QLabel(tr("Receive Message"));
    sendMessageEdit = new QLineEdit();
    receiveMessageEdit = new QLineEdit();
    sendMessageEdit->setEnabled(false);
    receiveMessageEdit->setEnabled(false);

    statusLabel = new QLabel(tr("Wait for connect."));

    listenButton = new QPushButton(tr("Listen"));
    stopButton = new QPushButton(tr("Stop"));
    sendButton = new QPushButton(tr("Send"));
    disConnectButton = new QPushButton(tr("Disconnect"));
    sendButton->setEnabled(false);
    stopButton->setEnabled(false);
    disConnectButton->setEnabled(false);
    hostLabel->setAlignment(Qt::AlignCenter);
    portLabel->setAlignment(Qt::AlignCenter);
    sendMessageLabel->setAlignment(Qt::AlignCenter);
    receiveMessageLabel->setAlignment(Qt::AlignCenter);

    QGridLayout *hostInfoLayout = new QGridLayout;
    hostInfoLayout->addWidget(hostLabel, 0, 0, 1, 1);
    hostInfoLayout->addWidget(portLabel, 1, 0, 1, 1);
    hostInfoLayout->addWidget(hostEdit, 0, 1, 1, 2);
    hostInfoLayout->addWidget(portEdit, 1, 1, 1, 2);

    hostInfoLayout->addWidget(sendMessageLabel, 3, 0, 1, 1);
    hostInfoLayout->addWidget(receiveMessageLabel, 2, 0, 1, 1);
    hostInfoLayout->addWidget(sendMessageEdit, 3, 1, 1, 2);
    hostInfoLayout->addWidget(receiveMessageEdit, 2, 1, 1, 2);
    hostInfoLayout->addWidget(listenButton, 0, 3, 1, 1);
    hostInfoLayout->addWidget(stopButton, 1, 3, 1, 1);
    hostInfoLayout->addWidget(disConnectButton, 2, 3, 1, 1);
    hostInfoLayout->addWidget(sendButton, 3, 3, 1, 1);

    hostInfoGroupBox->setLayout(hostInfoLayout);
}
