#include "base.h"
#include "SDRAM.h"
#include "USB.h"
#include "DATA.h"




//hellos



void main(void)
{

	// Set up SDRAM and test to make sure we are properly writing/reading (basic test)
	baseLEDInit();
	sdramSetup();
	sdramFillMem();
	sdramTestMem();


//	usbTest();
	
	dataInit();
	dataInterrupts();
	dataCPLD();
	usbInit();
	
	while(1)
	{
		asm("nop;\n");
	}

}

