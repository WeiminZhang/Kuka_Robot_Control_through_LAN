#include "modulenetwork.h"
#include <QApplication>
#include <QDebug>

ModuleNetwork::ModuleNetwork(QObject *parent)
    : QObject(parent),
      threadTcpNetwork_(NULL),
      tcpNetwork_(NULL)
{
    config();
    connection();
}

ModuleNetwork::~ModuleNetwork()
{
    qDebug() << "DESTRUCTOR ModuleNetwork";
    releaseResource();
}

void ModuleNetwork::onNotify(Event *event)
{
    if(event)
        event->execute(this);
}

void ModuleNetwork::startServer(EventNetworkStartServer *event)
{
    qDebug() << "EVENT NETWORK START";
    tcpNetwork_->startServer(event->hostAddress());


}

void ModuleNetwork::stopServer(EventNetworkStopServer *event)
{
    Q_UNUSED(event)
    qDebug() << "EVENT NETWORK STOP";
    tcpNetwork_->stopServer();
}

void ModuleNetwork::sendData(EventNetworkSendData *event)
{
    qDebug() << "SEND data" << event->byte();
    emit signalSendData(event->byte());
}

void ModuleNetwork::stateServer()
{
    qDebug() << "SERVER" << tcpNetwork_->isStarted();
    stateNetworkServer_.setIsStarted(tcpNetwork_->isStarted());
    stateNetworkServer_.setHostaddressServer(tcpNetwork_->hostInfoServer());
    stateNetworkServer_.setPortServer(tcpNetwork_->portServer());
    stateNetworkServer_.setHostAddressClient(tcpNetwork_->hostInfoClient());
    stateNetworkServer_.setPortClient(tcpNetwork_->portClient());
    stateNetworkServer_.setStatusError(QAbstractSocket::SocketError());
    stateNetworkServer_.setErrorString("");
    notify(new EventNetworkStateServer(&this->stateNetworkServer_));
}

void ModuleNetwork::stateClient()
{
    qDebug() << "NEW CONNECTION" << tcpNetwork_->hostInfoClient();
    stateNetworkServer_.setIsStarted(tcpNetwork_->isStarted());
    stateNetworkServer_.setHostaddressServer(tcpNetwork_->hostInfoServer());
    stateNetworkServer_.setPortServer(tcpNetwork_->portServer());
    stateNetworkServer_.setHostAddressClient(tcpNetwork_->hostInfoClient());
    stateNetworkServer_.setPortClient(tcpNetwork_->portClient());
    stateNetworkServer_.setStatusError(QAbstractSocket::SocketError());
    stateNetworkServer_.setErrorString("");
    notify(new EventNetworkStateClient(&this->stateNetworkServer_));
}

void ModuleNetwork::stateError(QAbstractSocket::SocketError socketError, QString errorString)
{
    qDebug() << "ERROR 1" << tcpNetwork_->hostInfoClient();
    stateNetworkServer_.setIsStarted(tcpNetwork_->isStarted());
    stateNetworkServer_.setHostaddressServer(tcpNetwork_->hostInfoServer());
    stateNetworkServer_.setPortServer(tcpNetwork_->portServer());
    stateNetworkServer_.setHostAddressClient(tcpNetwork_->hostInfoClient());
    stateNetworkServer_.setPortClient(tcpNetwork_->portClient());
    stateNetworkServer_.setStatusError(socketError);
    stateNetworkServer_.setErrorString(errorString);
    notify(new EventNetworkStateError(&this->stateNetworkServer_));
}

void ModuleNetwork::incommingData(QByteArray byte)
{
    qDebug() << "INCOMMING " << byte;
    notify(new EventNetworkReceivedData(byte));
}

void ModuleNetwork::config()
{
    //instance Network and separate thread
    tcpNetwork_ = new TcpNetworkServer;
    threadTcpNetwork_ = new QThread;

    //move to parallel thread
    tcpNetwork_->moveToThread(threadTcpNetwork_);
    //start parallel thread
    threadTcpNetwork_->start();
    //initialize
    tcpNetwork_->initilize();
}

void ModuleNetwork::connection()
{
    connect(tcpNetwork_, SIGNAL(signalStateServer()),
            this, SLOT(stateServer()));
    connect(tcpNetwork_, SIGNAL(signalStateClient()),
            this, SLOT(stateClient()));
    connect(tcpNetwork_, SIGNAL(signalStateError(QAbstractSocket::SocketError,QString)),
            this, SLOT(stateError(QAbstractSocket::SocketError,QString)));
    connect(tcpNetwork_, SIGNAL(signalIncommingData(QByteArray)),
            this, SLOT(incommingData(QByteArray)));
    connect(this, SIGNAL(signalSendData(QByteArray)),
            tcpNetwork_, SLOT(sendData(QByteArray)));
}

void ModuleNetwork::releaseResource()
{    
    if(tcpNetwork_)
    {
        tcpNetwork_->stopServer();
        tcpNetwork_->deleteLater();
    }
    tcpNetwork_ = NULL;
    if(threadTcpNetwork_)
        threadTcpNetwork_->deleteLater();
    threadTcpNetwork_ = NULL;
}

