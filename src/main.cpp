#include <stdint.h>
#include <stdio.h>

#include "event.h"
#include "testtask.h"

#ifdef IS_BARE_METAL
extern "C" int mcuBoot(void);
#else
//#include <chrono.h>
//#include <thread.h>
#endif

static EventScheduler s("main");
volatile uint32_t cnt=20000;
volatile unsigned long tn;

static short sam[2];
static float ti=0;


int main(int argc,char**argv)
{
#ifdef IS_BARE_METAL
    mcuBoot();
#endif
    {
        TestTask t(&s,"test 1",2000);
        t.schedule(s,1);
    }
    {
        TestTask t(&s,"test 2",2000);
        t.schedule(s,1);
    }
    {
        TestTask t(&s,"test 3",2000);
        t.schedule(s,1);
    }
    while(cnt--){
//        tn=clock();
        s.clock();
#ifdef IS_BARE_METAL
#else
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif
    }
}
