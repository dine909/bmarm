#include <stdint.h>
#include <stdio.h>


#ifdef IS_BARE_METAL
extern "C" int mcuBoot(void);
#else
//#include <chrono.h>
//#include <thread.h>
#endif


int main(int argc,char**argv)
{
#ifdef IS_BARE_METAL
//    mcuBoot();
#endif
    while(1){
    }
}
