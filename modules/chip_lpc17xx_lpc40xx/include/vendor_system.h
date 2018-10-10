//#include "sys_config.h"
#include "chip.h"
#ifdef CORE_M4
#include "core_cm4.h"
#include "system_ARMCM4.h"
#elif CORE_M3
#include "core_cm3.h"
#include "system_ARMCM3.h"
#endif
