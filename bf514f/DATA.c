#include "DATA.h"
#include "USB.h"

volatile unsigned short int DData1[NODES*BLOCK];
volatile unsigned short int DData2[NODES*BLOCK];
volatile unsigned short int TempBuff[NODES][BLOCK];
volatile int ping = 0, i = 0, n = 0, index = 0, j=0, flag = 0x8, cc = 0;
unsigned long int count = 0;
unsigned int firstnode = 0;
unsigned int offset = 0;
unsigned int average = 0;

char out;


EX_INTERRUPT_HANDLER(PPI_RX_Isr);

struct Descriptor
{
	struct Descriptor *Next;
	volatile unsigned short int *Address;
	unsigned short Config;
	unsigned short X_count;
	unsigned short X_modify;

}D1,D2;



void dataInit()
{
	
	
	
	// Port F all peripheral functions
	// Port G all but PG14 peripheral functions
	*pPORTF_FER = 0xFFFF;
	*pPORTG_FER = ~PG14;
	*pPORTGIO_DIR = PG14;
	// prope mux values for PPI
	*pPORTF_MUX = 0x0555;
	*pPORTG_MUX = 0x4080;
	
	
	D1.Next = &D2;
	D1.Address = DData1;
	D1.Config = 0x7487;
	D1.X_count = sizeof(DData1);
	D1.X_modify = sizeof(DData1[0]);

	D2.Next = &D1;
	D2.Address =  DData2;
	D2.Config = 0x7487;
	D2.X_count = sizeof(DData2);
	D2.X_modify = sizeof(DData2[0]);

	*pDMA0_X_MODIFY = 2;
	*pDMA0_X_COUNT = SAMPLE;
	*pDMA0_NEXT_DESC_PTR = &D1;
	*pDMA0_START_ADDR = &DData1;

	*pDMA0_CONFIG = 0x7487;
	ssync();
	
	
	
	*pPPI_CONTROL = 0x383C;
	*pPPI_DELAY   = 0x0000;
	*pPPI_COUNT = SAMPLE; 	// value - 1
	*pPPI_FRAME = 1;
	*pPPI_CONTROL |= 0x0001;




}




EX_INTERRUPT_HANDLER(PPI_RX_Isr)
{		
	//register unsigned long int count = 0;
	*pPPI_CONTROL &= 0xFFFE;
	
	ping = (ping+1)%2;
	
	
	//TempBuff[0][0] = TempBuff[0][9];

	if (ping)
	{
		while((DData1[firstnode] & 0xF000) != 0x1000 )
		{
			firstnode++;
		}
		
		
		for (n = 1; n < 10; n++)
		{
			offset = firstnode;
			while ((DData1[(n-1) * 256 + offset] & 0xF000) != 0x1000)
			{
				offset++;
			}
			
			for (i = 0; i < 256; i++)
			{
				TempBuff[i][n] = DData1[(n-1)*256 + offset+i] & 0x0FFF;
			}
		}
	}
	else
	{
		while((DData2[firstnode] & 0xF000) != 0x1000 )
		{
			firstnode++;
		}
		
		
		for (n = 1; n < 10; n++)
		{
			offset = firstnode;
			while ((DData2[(n-1) * 256 + offset] & 0xF000) != 0x1000)
			{
				offset++;
			}
			
			for (i = 0; i < 256; i++)
			{
				TempBuff[i][n] = DData2[(n-1)*256 + offset+i] & 0x0FFF;
			}
		}
		
	}

		
		
	
	usbWrite(0x10);
	usbWrite(0x08);
	usbWrite(0xFF);
	
	for (i = 1; i < 256; i++)
	{
		
		count = 0;
		for (j = 1; j < BLOCK; j++)
		{
			count += TempBuff[i][j];
			
		}
		
		average = count / (BLOCK-1);
		
		usbWrite(0x10 | ((average & 0x0C00) >> 10));
	
		usbWrite((average & 0x03FC) >> 2);
			
	}
		delayMS(1100000);
			
	/*	
		
			
		unsigned int average = 0;
		count = 0;
		for (i = 0; i < SAMPLE*BLOCK; i++)
		{
			
			count = count + (TempBuff[i] & 0x0FFF);
			
			
		}
			
		average = count/(SAMPLE*BLOCK);
		out = (char)(((average >> 10) & 0x0003) | flag); 
		//out = (char)((TempBuff[i] >> 10) & 0x0003);
		usbWrite(out);
		//	parallelOut(0x04);
		out = (char)((average>>2) & 0x00FF);
		usbWrite(out);
		if (cc++ == 128)
		{
			
			usbWrite(0x05);
			usbWrite(0xFF);
		}
		
		if (cc >= 255)
		{
			cc = 0;
		}
		delayMS(3600);
		
	n = 0;
	//	count = 0;
		
		
			
	}
	*/
	
	
	*pDMA0_IRQ_STATUS |= 0x0001;	// Write 1 to clear
	*pPPI_CONTROL |= 0x0001;
	ssync();
}



void dataInterrupts(void)
{
	// configure interrupts
	register_handler(ik_ivg8, PPI_RX_Isr);		// assign ISR to interrupt vector
		
	*pSIC_IAR1 =  0x10000000;   //bits 31:28 map to PPI DMA0 interrupt
	*pSIC_IMASK = 0x00008000;    //bit 15 corresponds to PPI				// enable PPI (DMA0) interrupt

}

void dataCPLD(void)
{
	
	*pPORTGIO_CLEAR = PG14;
	*pPORTGIO_SET = PG14;
}

