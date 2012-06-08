#ifndef BASEH
#define BASEH

#include <ccblkfn.h>
#include <cdefBF514.h>
#include <sys/exception.h>
#include <signal.h>
#include <sysreg.h>
#include <math.h>
#include <bfrom.h>


#define LED0 PG5
#define LED1 PG6
 
void baseLEDInit(void);
void delayMS(const unsigned long ulMs);

#endif

