#include <cstdint>
#include <cstdio>
#include <unistd.h>

#include "event.h"
#include "testtask.h"

#ifdef IS_BARE_METAL
extern "C" int mcuBoot(void);
#endif

static EventScheduler s("main");

int main()
{
#ifdef IS_BARE_METAL
    mcuBoot();
#endif
    {
        TestTask t(&s,"test 1",10);
        t.schedule(s,1);
    }
    {
        TestTask t(&s,"test 2",20);
        t.schedule(s,1);
    }
    uint32_t cnt=100;
    while(cnt--){
        s.clock();
        sleep(1);
    }
}
