//#include <stdlib.h>

#ifdef IS_BARE_METAL
extern "C" int mcuBoot(void);
#endif

int main(void)
{
#ifdef IS_BARE_METAL
    mcuBoot();
#endif


}
