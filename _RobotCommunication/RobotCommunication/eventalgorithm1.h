#ifndef EVENTALGORITHM1_H
#define EVENTALGORITHM1_H
#include "event.h"

class EventAlgorithm1Start;
class EventAlgorithm1Stop;

class AbstractAlgorithm1 : public AbstractModule
{
public:
    virtual void startAlgorithm(EventAlgorithm1Start *event) = 0;
    virtual void stopAlgorithm(EventAlgorithm1Stop *event) = 0;
};

class EventAlgorithm1 :
        public Event
{
public:
    enum ID_EVENT
    {
        VOID,
        START,
        STOP,
        STATUS
    };

    EventAlgorithm1(EventAlgorithm1::ID_EVENT id = EventAlgorithm1::VOID)
        : Event(Event::ALGORITHM_1),
          id_(id)
    {}
    virtual void execute(AbstractModule *abstractModule) = 0;
    virtual int id() const {return static_cast<int>(id_);}
private:
    ID_EVENT id_;
};

class EventAlgorithm1Start :
        public EventAlgorithm1
{
public:
    EventAlgorithm1Start(double x, double y, double z, double step)
        : EventAlgorithm1(EventAlgorithm1::START),
          x_(x),
          y_(y),
          z_(z),
          step_(step)
    {}
    void execute(AbstractModule *abstractModule)
    {
        AbstractAlgorithm1 *abstractAlg
                = dynamic_cast<AbstractAlgorithm1*>(abstractModule);

        if(abstractAlg)
        {
            abstractAlg->startAlgorithm(this);
        }
    }

    double x() const {return x_;}
    double y() const {return y_;}
    double z() const {return z_;}
    double step() const {return step_;}

private:
    double x_;
    double y_;
    double z_;
    double step_;
};

class EventAlgorithm1Stop :
        public EventAlgorithm1
{
public:
    EventAlgorithm1Stop()
        : EventAlgorithm1(EventAlgorithm1::STOP)
    {}
    void execute(AbstractModule *abstractModule)
    {
        AbstractAlgorithm1 *abstractAlg
                = dynamic_cast<AbstractAlgorithm1*>(abstractModule);

        if(abstractAlg)
        {
            abstractAlg->stopAlgorithm(this);
        }
    }

private:
};

class EventAlgorithm1Status :
        public EventAlgorithm1
{
public:
    EventAlgorithm1Status(double x,
                          double y,
                          double z,
                          double executed,
                          bool isRun)
        : EventAlgorithm1(EventAlgorithm1::STATUS),
          x_(x),
          y_(y),
          z_(z),
          executed_(executed),
          isRun_(isRun)
    {}
    void execute(AbstractModule *abstractModule)
    {
        Q_UNUSED(abstractModule)
    }

    double x() const {return x_;}
    double y() const {return y_;}
    double z() const {return z_;}
    double executed() const {return executed_;}
    bool isRun() const {return isRun_;}

private:
    double x_;
    double y_;
    double z_;
    double executed_;
    bool isRun_;
};

#endif // EVENTALGORITHM1_H
