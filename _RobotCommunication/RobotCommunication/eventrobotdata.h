#ifndef EVENTROBOTDATA_H
#define EVENTROBOTDATA_H
#include "event.h"
#include "robotdata.h"

class EventRobotData :
        public Event
{
public:
    enum ID_EVENT
    {
        VOID,
        EXTRACT_DATA,
        EMBED_DATA
    };

    EventRobotData(EventRobotData::ID_EVENT id = EventRobotData::VOID)
        : Event(Event::ROBOT_DATA),
          id_(id)
    {}

    virtual void execute(AbstractModule *abstractModule) = 0;
    virtual int id() const {return static_cast<int>(id_);}
private:
    ID_EVENT id_;
};

class EventRobotDataExtractData :
        public EventRobotData
{
public:
     EventRobotDataExtractData(RobotData robotData)
         : EventRobotData(EventRobotData::EXTRACT_DATA),
           robotData_(robotData)
     {}

     void execute(AbstractModule *abstractModule)
     {
         Q_UNUSED(abstractModule)
     }

     RobotData robotData()const
     {
         return robotData_;
     }

private:
     RobotData robotData_;
};

class EventRobotDataEmbedData :
        public EventRobotData
{
public:
     EventRobotDataEmbedData(RobotData robotData)
         : EventRobotData(EventRobotData::EMBED_DATA),
           robotData_(robotData)
     {}

     void execute(AbstractModule *abstractModule)
     {
         Q_UNUSED(abstractModule)
     }

     RobotData robotData()const
     {
         return robotData_;
     }

private:
     RobotData robotData_;
};

#endif // EVENTROBOTDATA_H
