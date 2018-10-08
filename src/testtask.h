#ifndef TESTTASK_H
#define TESTTASK_H

#include <cstdio>

#include <cstdlib>
#include <iostream>
#include "event.h"

using namespace std;

class TestTask : public Event
{
    EventContext &eventContext;
    uint32_t cy;
public:
    TestTask(EventContext *context, const char* name,uint32_t cy) :
        Event (name),
        eventContext(*context),
        cy(cy)
    {

    }

    // Event interface
public:
    inline void event() override
    {
        cout << name() << ": " << eventContext.getTime() << endl;

        schedule(eventContext,cy);
    }
};

#endif // TESTTASK_H
