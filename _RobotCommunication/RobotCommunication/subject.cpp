#include "subject.h"
#include "observer.h"

Subject::~Subject()
{

}

bool Subject::addObserver(Observer *observer)
{
    if(listObserver_.contains(observer))
        return false;

    listObserver_.push_back(observer);
    return true;
}

bool Subject::removeObserver(Observer *observer)
{
    return listObserver_.removeOne(observer);
}

void Subject::notify(Event *event)
{
    for(int i = 0; i < listObserver_.size(); i++)
        listObserver_[i]->onNotify(event);

    if(event)
        delete event;
}

