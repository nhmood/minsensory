#ifndef SDRAM
#define SDRAM
#include "base.h"


#define CLKIN (30)	// clock-in frequency in MHz
#define TREF 64
#define EXP 1000
#define NRA 8192
#define SPEC_TRAS (44e-9)
#define SPEC_TRP (20e-9)
#define MHz_EXP (1e6)


	
#define VRCTL_VALUE         0x0000
#define PLLCTL_VALUE        0x2000
#define PLLDIV_VALUE        0x0004
#define PLLLOCKCNT_VALUE    0x0000
#define PLLSTAT_VALUE       0x0000
#define RSICLK_DIV          0x0001


#define TestSize 1000



void sdramSetup(void);
unsigned short sdramCheckFreq(unsigned short);
unsigned short sdramCalcParam(unsigned short);
void sdramInit(unsigned short);
void initLED(void);
void sdramFillMem(void);
void sdramTestMem(void);
 
 
#endif
 


 

