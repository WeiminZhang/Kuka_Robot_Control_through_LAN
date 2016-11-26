#ifndef TCPNETWORKSERVER_H
#define TCPNETWORKSERVER_H
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include "hostaddressconfig.h"

class TcpNetworkServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpNetworkServer(QObject *parent = 0);
    virtual ~TcpNetworkServer();
    bool isStarted() const;
    QHostAddress hostInfoServer()const;
    quint16 portServer()const;
    QHostAddress hostInfoClient()const;
    quint16 portClient()const;

signals:
    void signalInitialize();
    void signalStartServer(HostAddressConfig);
    void signalStopServer();
    void signalStateServer();
    void signalStateClient();
    void signalStateError(QAbstractSocket::SocketError, QString);
    void signalIncommingData(QByteArray);

public slots:
    void sendData(QByteArray data);
    void startServer(HostAddressConfig address);
    void stopServer();
    void initilize();

private slots:
    void newConnection();
    void errorCommuication(QAbstractSocket::SocketError socketError);
    void readIncommingData();
    void initializeNetwork();
    bool tryStartServer(HostAddressConfig address);
    void tryStopServer();

private:
    void config();
    void connection();
    void releaseResources();
    void closeSocket();

    //attributes
    QTcpServer *server_;
    QTcpSocket *socketServerConnection_;    
};

#endif // TCPNETWORKSERVER_H
