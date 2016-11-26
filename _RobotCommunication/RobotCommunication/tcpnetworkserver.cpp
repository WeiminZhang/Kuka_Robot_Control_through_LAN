#include "tcpnetworkserver.h"
#include <QDebug>
#include <QApplication>
#include <QMetaType>

Q_DECLARE_METATYPE(HostAddressConfig)
//Q_DECLARE_METATYPE(QAbstractSocket::SocketError)

TcpNetworkServer::TcpNetworkServer(QObject *parent)
    : QObject(parent),
      server_(NULL),
      socketServerConnection_(NULL)
{
    connect(this, SIGNAL(signalInitialize()),
            this, SLOT(initializeNetwork()));
    connect(this, SIGNAL(signalStartServer(HostAddressConfig)),
            this, SLOT(tryStartServer(HostAddressConfig)));
    connect(this, SIGNAL(signalStopServer()),
            this, SLOT(tryStopServer()));

    qRegisterMetaType<HostAddressConfig>();
    qRegisterMetaType<QAbstractSocket::SocketError>();
}

TcpNetworkServer::~TcpNetworkServer()
{
    qDebug() << "DESTRUCTOR TcpNetworkServer";
    stopServer();
//    releaseResources();
}

bool TcpNetworkServer::isStarted() const
{
    return server_->isListening();
}

QHostAddress TcpNetworkServer::hostInfoServer() const
{
    return server_->serverAddress();
}

quint16 TcpNetworkServer::portServer() const
{
    return server_->serverPort();
}

QHostAddress TcpNetworkServer::hostInfoClient() const
{
    if(socketServerConnection_)
    {
        return socketServerConnection_->peerAddress();
    }
    return QHostAddress();
}

quint16 TcpNetworkServer::portClient() const
{
    if(socketServerConnection_)
    {
        return socketServerConnection_->peerPort();
    }
    return 0;
}

void TcpNetworkServer::sendData(QByteArray data)
{
    if(socketServerConnection_)
    {
        socketServerConnection_->write(data);
        socketServerConnection_->flush();
    }
}

void TcpNetworkServer::startServer(HostAddressConfig address)
{
    emit signalStartServer(address);
}

void TcpNetworkServer::stopServer()
{
    emit signalStopServer();
}

void TcpNetworkServer::initilize()
{
    emit signalInitialize();
}

void TcpNetworkServer::newConnection()
{
    socketServerConnection_ = server_->nextPendingConnection();

    //create connection for properly working
    connect(socketServerConnection_, SIGNAL(readyRead()),
            this, SLOT(readIncommingData()));
    connect(socketServerConnection_, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(errorCommuication(QAbstractSocket::SocketError)));

    emit signalStateClient();

//    socket->write("Test\r\n");
//    socket->flush();
//    socket->waitForBytesWritten(3000);
    //    socket->close();
}

void TcpNetworkServer::errorCommuication(QAbstractSocket::SocketError socketError)
{
    qDebug() << "ERROR" << socketError;
    if (socketError == QTcpSocket::RemoteHostClosedError)
    {
//        socketServerConnection_ = NULL;
    }
    else
    {
        closeSocket();
        server_->close();
    }

    emit signalStateError(socketError, socketServerConnection_->errorString());
    closeSocket();
}

void TcpNetworkServer::readIncommingData()
{
    QByteArray byte = socketServerConnection_->readAll();
//    qDebug() << "READ INCOMING DATA" << byte;
    emit signalIncommingData(byte);

//    socketServerConnection_->write(QString("\r\nTEST %1 \r\n").arg(byte.data()).toLatin1().data());
//    socketServerConnection_->flush();
}

void TcpNetworkServer::config()
{
    server_ = new QTcpServer(this);
    //set maximum pending connection
    server_->setMaxPendingConnections(1);
}

void TcpNetworkServer::connection()
{
    connect(server_, SIGNAL(newConnection()),
            this, SLOT(newConnection()));
}

void TcpNetworkServer::initializeNetwork()
{
    qDebug() << "INITIALIZE NETWORK";
    config();
    connection();
}

bool TcpNetworkServer::tryStartServer(HostAddressConfig address)
{
    if(!server_->listen(address.address(),address.port()))
    {
        qDebug() << "Server could not start";        
        emit signalStateServer();
        return false;
    }
    else
    {
        qDebug() << "Server started";
        emit signalStateServer();
        return true;
    }

}

void TcpNetworkServer::tryStopServer()
{
    emit signalStateServer();
    closeSocket();
    releaseResources();
}

void TcpNetworkServer::releaseResources()
{
    server_->close();
}

void TcpNetworkServer::closeSocket()
{
    if(socketServerConnection_)
        socketServerConnection_->close();
    socketServerConnection_ = NULL;
}

