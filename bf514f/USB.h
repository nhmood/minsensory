#ifndef USB
#define USB
#include "base.h"

// Data Lines
// All on PORTH, can set L/B Endian on FTDI chip
#define AD0 PH0
#define AD1 PH1
#define AD2 PH2
#define AD3 PH3
#define AD4 PH4
#define AD5 PH5
#define AD6 PH6
#define AD7 PH7

// TX/RX Configuration Lines
// All on PORTG, check notes above for functionality
#define AC0 PG15
#define AC1 PG11
#define AC2 PG9
#define AC3 PG10 



#define PB0 PG3
#define PB1 PG4

/* Function Prototypes */

void usbInit(void);	// dir = 0 -> READ, dir = 1 -> WRITE
void usbWrite( char count );
void usbToggle(void);
void usbTest(void);

#endif

