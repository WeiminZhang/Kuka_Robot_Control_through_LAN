#ifndef MODULENETWORK_H
#define MODULENETWORK_H
#include <QObject>
#include <QThread>
#include "observer.h"
#include "subject.h"
#include "tcpnetworkserver.h"
#include "statenetwork.h"

class ModuleNetwork :
        public QObject,
        public AbstractNetwork,
        public Observer,
        public Subject
{
    Q_OBJECT
public:
    ModuleNetwork(QObject *parent = 0);
    ~ModuleNetwork();

    void onNotify(Event *event);

signals:
    void signalSendData(QByteArray);

private slots:
    void startServer(EventNetworkStartServer *event);
    void stopServer(EventNetworkStopServer *event);
    void sendData(EventNetworkSendData *event);
    void stateServer();
    void stateClient();
    void stateError(QAbstractSocket::SocketError socketError, QString errorString);
    void incommingData(QByteArray byte);
    //incomming

private:
    void config();
    void connection();
    void releaseResource();

    //attributs
    QThread *threadTcpNetwork_;
    TcpNetworkServer *tcpNetwork_;
    StateNetwork stateNetworkServer_;
};

#endif // MODULENETWORK_H
