#ifndef STATENETWORK
#define STATENETWORK
#include <QHostAddress>
#include <QAbstractSocket>
/////
/// \brief The StateNetwork class
/// Class store network state
class StateNetwork
{
public:
    StateNetwork()
        : isStarted_(false){}

    bool isStarted() const
    {
        return isStarted_;
    }

    void setIsStarted(bool isStarted)
    {
        isStarted_ = isStarted;
    }

    QHostAddress hostaddressServer() const
    {
        return hostaddressServer_;
    }
    void setHostaddressServer(const QHostAddress &hostaddressServer)
    {
        hostaddressServer_ = hostaddressServer;
    }

    QHostAddress hostAddressClient() const
    {
        return hostAddressClient_;
    }
    void setHostAddressClient(const QHostAddress &hostAddressClient)
    {
        hostAddressClient_ = hostAddressClient;
    }

    quint16 portServer() const
    {
        return portServer_;
    }
    void setPortServer(const quint16 &portServer)
    {
        portServer_ = portServer;
    }

    quint16 portClient() const
    {
        return portClient_;
    }

    void setPortClient(const quint16 &portClient)
    {
        portClient_ = portClient;
    }

    QAbstractSocket::SocketError statusError() const
    {
        return statusError_;
    }

    void setStatusError(const QAbstractSocket::SocketError &statusError)
    {
        statusError_ = statusError;
    }

    QString errorString() const
    {
        return errorString_;
    }

    void setErrorString(const QString &errorString)
    {
        errorString_ = errorString;
    }

private:
    bool isStarted_;
    QHostAddress hostaddressServer_;
    quint16 portServer_;
    QHostAddress hostAddressClient_;
    quint16 portClient_;
    QAbstractSocket::SocketError statusError_;
    QString errorString_;
};

#endif // STATENETWORK
