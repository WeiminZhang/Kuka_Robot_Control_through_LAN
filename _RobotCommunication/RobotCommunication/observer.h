#ifndef OBSERVER
#define OBSERVER
#include "eventcollection.h"

class Observer
{
public:
    virtual ~Observer(){}
    virtual void onNotify(Event *event) = 0;
};

#endif // OBSERVER

