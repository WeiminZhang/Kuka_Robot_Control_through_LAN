#ifndef SUBJECT_H
#define SUBJECT_H
#include <QVector>
#include "eventcollection.h"

class Observer;

class Subject
{
public:
    virtual ~Subject();
    bool addObserver(Observer *observer);
    bool removeObserver(Observer *observer);
    void notify(Event *event);

private:
    //attributes
    QVector<Observer *> listObserver_;
};

#endif // SUBJECT_H
