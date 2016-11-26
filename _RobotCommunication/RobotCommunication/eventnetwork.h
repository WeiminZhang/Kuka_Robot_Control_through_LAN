#ifndef EVENTNETWORK
#define EVENTNETWORK
#include "event.h"
#include "hostaddressconfig.h"
#include "statenetwork.h"
#include <QByteArray>

class EventNetworkStartServer;
class EventNetworkStopServer;
class EventNetworkSendData;

////
/// \brief The AbstractNetwork class
/// Class for handle the events
class AbstractNetwork : public AbstractModule
{
public:
    virtual void startServer(EventNetworkStartServer *event) = 0;
    virtual void stopServer(EventNetworkStopServer *event) = 0;
    virtual void sendData(EventNetworkSendData *evewnt) = 0;
};

///////////
/// \brief The EventNetwork class
/// Network event
class EventNetwork : public Event
{
public:
    enum ID_EVENT
    {
        VOID,
        START_SERVER,
        STOP_SERVER,
        STATE_NETWORK_SERVER,
        STATE_NETWORK_CLIENT,
        STATE_NETWORK_ERROR,
        RECEIVED_DATA,
        SEND_DATA
    };

    EventNetwork(EventNetwork::ID_EVENT id = EventNetwork::VOID)
        : Event(Event::NETWORK),
          id_(id)
    {}
    virtual void execute(AbstractModule *abstractModule) = 0;
    virtual int id() const {return static_cast<int>(id_);}
private:
    ID_EVENT id_;
};


/////
/// \brief The EventNetworkStartServer class
/// event start server
class EventNetworkStartServer : public EventNetwork
{
public:
    EventNetworkStartServer(HostAddressConfig hostAddressConfig)
        : EventNetwork(EventNetwork::START_SERVER),
          hostAddressConfig_(hostAddressConfig)
    {}

    HostAddressConfig &hostAddress(){return hostAddressConfig_;}
    void execute(AbstractModule *abstractModule)
    {
        AbstractNetwork *abstractNetwork
                = dynamic_cast<AbstractNetwork*>(abstractModule);
        if(abstractNetwork)
        {
            abstractNetwork->startServer(this);
        }
    }

private:
    HostAddressConfig hostAddressConfig_;
};

/////////
/// \brief The EventNetworkStopServer class
/// event stop server
class EventNetworkStopServer : public EventNetwork
{
public:
    EventNetworkStopServer()
        : EventNetwork(EventNetwork::STOP_SERVER)
    {}
    void execute(AbstractModule *abstractModule)
    {
        AbstractNetwork *abstractNetwork
                = dynamic_cast<AbstractNetwork*>(abstractModule);
        if(abstractNetwork)
        {
            abstractNetwork->stopServer(this);
        }
    }
};

class EventNetworkStateServer : public EventNetwork
{
public:
    EventNetworkStateServer(StateNetwork *stateNetwork)
        : EventNetwork(EventNetwork::STATE_NETWORK_SERVER),
          stateNetwork_(stateNetwork)
    {}

    StateNetwork *stateNetwork(){return stateNetwork_;}
    void execute(AbstractModule *abstractModule)
    {
        Q_UNUSED(abstractModule)
    }

private:
    StateNetwork *stateNetwork_;
};

class EventNetworkStateClient : public EventNetwork
{
public:
    EventNetworkStateClient(StateNetwork *stateNetwork)
        : EventNetwork(EventNetwork::STATE_NETWORK_CLIENT),
          stateNetwork_(stateNetwork)
    {}

    StateNetwork *stateNetwork(){return stateNetwork_;}
    void execute(AbstractModule *abstractModule)
    {
        Q_UNUSED(abstractModule)
    }

private:
    StateNetwork *stateNetwork_;
};

class EventNetworkStateError : public EventNetwork
{
public:
    EventNetworkStateError(StateNetwork *stateNetwork)
        : EventNetwork(EventNetwork::STATE_NETWORK_ERROR),
          stateNetwork_(stateNetwork)
    {}

    StateNetwork *stateNetwork(){return stateNetwork_;}
    void execute(AbstractModule *abstractModule)
    {
        Q_UNUSED(abstractModule)
    }

private:
    StateNetwork *stateNetwork_;
};

class EventNetworkReceivedData : public EventNetwork
{
public:
    EventNetworkReceivedData(const QByteArray &byte)
        : EventNetwork(EventNetwork::RECEIVED_DATA),
          byte_(byte)
    {}

    QByteArray byte() const{return byte_;}
    void execute(AbstractModule *abstractModule)
    {
        Q_UNUSED(abstractModule)
    }

private:
    QByteArray byte_;
};

class EventNetworkSendData : public EventNetwork
{
public:
    EventNetworkSendData(const QByteArray &byte)
        : EventNetwork(EventNetwork::SEND_DATA),
          byte_(byte)
    {}

    QByteArray byte() const{return byte_;}
    void execute(AbstractModule *abstractModule)
    {
        AbstractNetwork *abstractNetwork
                = dynamic_cast<AbstractNetwork*>(abstractModule);
        if(abstractNetwork)
        {
            abstractNetwork->sendData(this);
        }
    }

private:
    QByteArray byte_;
};
#endif // EVENTNETWORK

