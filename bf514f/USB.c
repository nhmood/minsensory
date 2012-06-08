#include "USB.h"




void usbInit(void)
{
	// Enable ALL pins used on PortF
	*pPORTH_FER &= ~(AD0 | AD1 | AD2 | AD3 | AD4 | AD5 | AD6 | AD7);
	*pPORTG_FER &= ~(AC0 | AC1 | AC2 | AC3);
	// Set output direction to high on all OUTPUT pins on PortF
	*pPORTHIO_DIR |= (AD0 | AD1 | AD2 | AD3 | AD4 | AD5 | AD6 | AD7); 
	*pPORTGIO_DIR |= ~PB0 + ~PB1 + (AC0 | AC1 | AC2 | AC3);
	
	
	
	
	
	// Pushbutton Interrupt enable
	*pPORTGIO_INEN  |= (PB0 | PB1);				// enable interrupt
	*pPORTGIO_POLAR &= ~(PB0 | PB1);			// polarity
	*pPORTGIO_EDGE |= (PB0 | PB1);				// rising edge
	*pPORTGIO_MASKA |= (PB0 | PB1);				// mask
	*pPORTGIO_MASKA_SET |= (PB0 | PB1);			// and again	
	
	// button INTERRUPT** doesn't work yet
	int iarMAP;
	iarMAP = *pSIC_IAR3;
	iarMAP |= 0x20000000;
	iarMAP &= 0x2fffffff;
	
	// AC0 -> PG15
	// AC0 is RXF# - When high, do not read data from the FIFO
	// If RXF# is high, we don't care about RD# (AC2 -> PF2)
	*pPORTGIO_SET = AC0;
	
	
	// AC1 -> PG11
	// AC1 is TXE# - When low, data can be written into the FIFO by strobing WR# high, then low.
	// #WR (AC3 -> PF0) must go high -> low -> high have data written, start it in LOW
	//*pPORTGIO_CLEAR = PG2;
	*pPORTGIO_CLEAR = AC1;
	
	*pPORTGIO_SET = AC3;
	
}



void usbWrite( char count )
{

	if (PG0 & count){ *pPORTHIO_SET = AD0; } else { *pPORTHIO_CLEAR = AD0; }
	if (PG1 & count){ *pPORTHIO_SET = AD1; } else { *pPORTHIO_CLEAR = AD1; }
	if (PG2 & count){ *pPORTHIO_SET = AD2; } else { *pPORTHIO_CLEAR = AD2; }
	if (PG3 & count){ *pPORTHIO_SET = AD3; } else { *pPORTHIO_CLEAR = AD3; }
	if (PG4 & count){ *pPORTHIO_SET = AD4; } else { *pPORTHIO_CLEAR = AD4; }
	if (PG5 & count){ *pPORTHIO_SET = AD5; } else { *pPORTHIO_CLEAR = AD5; }
	if (PG6 & count){ *pPORTHIO_SET = AD6; } else { *pPORTHIO_CLEAR = AD6; }
	if (PG7 & count){ *pPORTHIO_SET = AD7; } else { *pPORTHIO_CLEAR = AD7; }


	usbToggle();
	// Set each bit corresponding to the value being sent out
/*	int i, b;
	for (i = 0; i < 8; i++)
	{
		// Calculate whether bit should be high or low
		b = AD(i) & count;
		// If high, SET that pin, if low, CLEAR that pin
		(b == 1 ) ? (*pPORTGIO_SET = AD(i)) : (*pPORTGIO_CLEAR = AD(i));
	}
*/		
	
}



void usbToggle(void)
{
	// To write from FTDI chip we need a high -> low -> high transition
	// Corresponding pin is set to high during initialization
	// We therefore toggle the pin to low (CLEAR) then back to high (SET)
	// Delays of 20 cycles are placed in between to assure the edge gets
	// recognized

	delayMS(20);
	*pPORTGIO_CLEAR = AC3;
	delayMS(20);
	*pPORTGIO_SET = AC3;
	delayMS(20);

}


void usbTest(void)
{
	char TestString[] = "This is a functionality test of the USB driver\0";
	int i = 0;
	for (i = 0; i < sizeof(TestString); i++)
	{
		usbWrite(TestString[i]);
	
	}
	
	printf("USB Done\0");




}

