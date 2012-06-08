
#include "base.h"
static volatile unsigned long g_ulTickCount;	

void baseLEDInit(void)
{
	*pPORTG_FER &= ~(LED1 | LED0);
	*pPORTGIO_DIR |= (LED1 | LED0);
	*pPORTGIO_CLEAR = (LED1 | LED0);

}


void delayMS(const unsigned long ulMs)
{
	
	g_ulTickCount = 0;
	unsigned long ulEnd = (g_ulTickCount + ulMs);

	//sti(uiTIMASK);

    while( g_ulTickCount < ulEnd )
	{
		g_ulTickCount++;
	}
}

