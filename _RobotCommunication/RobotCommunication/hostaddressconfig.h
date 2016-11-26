#ifndef HOSTADDRESSCONFIG
#define HOSTADDRESSCONFIG
#include <QHostAddress>

class HostAddressConfig
{
public:
    HostAddressConfig()
        : address_(QHostAddress()),
          port_(0)
    {}

    HostAddressConfig(const QHostAddress &address,
                      quint16 port)
        : address_(address),
          port_(port)
    {
        if(address_.isNull())
            throw QString("Invalid IP address");
    }

    QHostAddress &address() { return address_;}
    quint16 port() { return port_;}

private:
   QHostAddress address_;
   quint16 port_;
};


#endif // HOSTADDRESSCONFIG

