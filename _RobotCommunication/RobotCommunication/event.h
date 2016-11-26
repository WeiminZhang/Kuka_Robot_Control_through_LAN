#ifndef EVENT
#define EVENT
#include <QString>

class AbstractModule
{
public:
    virtual ~AbstractModule(){}
};

class Event
{
public:

    enum ID_EVENT
    {
        VOID,
        NETWORK,
        ROBOT_DATA,
        ALGORITHM_1
    };

    Event(ID_EVENT id = VOID,
          quint32 idReseiver = 0xFFFFFFFF,
          quint32 idSender = 0xFFFFFFFF)
        : id_(id),
          idReseiver_(idReseiver),
          idSender_(idSender)
    {}
    virtual void execute(AbstractModule *abstractModule) = 0;
    virtual int id() const {return static_cast<int>(id_);}
    quint32 idReseiver() const{return idReseiver_;}
    quint32 idSender() const{return idSender_;}

private:
    ID_EVENT id_;
    quint32 idReseiver_;
    quint32 idSender_;
};

#endif // EVENT

