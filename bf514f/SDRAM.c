#include "SDRAM.h"

int sdramTestBuff[TestSize];
 
 void sdramSetup(void)
 {
 	
 	
 	ADI_SYSCTRL_VALUES sysctrl ={	VRCTL_VALUE,
									PLLCTL_VALUE,		/* (25MHz CLKIN x (MSEL=16))::CCLK = 400MHz */
									PLLDIV_VALUE,		/* (400MHz/(SSEL=5))::SCLK = 80MHz */
									PLLLOCKCNT_VALUE,
									PLLSTAT_VALUE };
	bfrom_SysControl( SYSCTRL_WRITE | SYSCTRL_PLLCTL | SYSCTRL_PLLDIV, &sysctrl, NULL);
 	
 	
 	
	unsigned short SCLK_Freq, RDIV;
	unsigned short tempReg;
	
	tempReg = *pEBIU_SDSTAT;	// get status register
	tempReg = tempReg & 0x0008;	// check whether it has been reset
 
	// Determine core and system clock based on input clock
	SCLK_Freq = sdramCheckFreq((unsigned short)CLKIN);
	
	//Based on core + system clock, get refresh rate for SDRAM
	RDIV = sdramCalcParam(SCLK_Freq);
	
	// Set up SDRAM with given refresh rate 
	sdramInit(RDIV);
	
 }
 
 // From BF561 EZ-KIT Lite SDRAM Sample Code
 unsigned short sdramCheckFreq (unsigned short Clkin_Freq)
{
	unsigned short tempReg_PLLCTL, tempReg_PLLDIV;
	unsigned short MSEL_Mult, SCLK_Mult, Divide_Frequency, SCLK, VCO_Freq;
	
	tempReg_PLLCTL = *pPLL_CTL;
	tempReg_PLLDIV = *pPLL_DIV;
	
	Divide_Frequency = tempReg_PLLCTL & 0x0001;
	if(Divide_Frequency == 1)
	{
		Clkin_Freq = Clkin_Freq/2;
	}
	
	MSEL_Mult = ((tempReg_PLLCTL & 0x7E00) >> 9);
	VCO_Freq = MSEL_Mult * Clkin_Freq;
	
	SCLK_Mult = tempReg_PLLDIV & 0x000f;
	
	SCLK = VCO_Freq/SCLK_Mult;
	
	return SCLK;
	
}
 
 
unsigned short sdramCalcParam(unsigned short SCLK_Freq)
{
	
	float clk_timing, tempReg_one; 
	int full_SCLK;
	unsigned short tras, trp, tempReg_two, RDIV;
	 
	tempReg_one = (float)(SCLK_Freq) * (float)(TREF);
	tempReg_one = tempReg_one/(float)(NRA);
	tempReg_one = ceil(tempReg_one * (float)(EXP));
	
	
	full_SCLK = (int)(SCLK_Freq)*(int)(MHz_EXP);
	clk_timing = 1/((float)(full_SCLK));
	
	tras = (unsigned short)(ceil((float)(SPEC_TRAS)/clk_timing));
	trp = (unsigned short)(ceil((float)(SPEC_TRP)/clk_timing));
	
	tempReg_two = tras + trp;
	
	RDIV = (unsigned short)(tempReg_one) - tempReg_two;

	return RDIV;	
}


void sdramInit(unsigned short RDIV_Value)
{
	// No idea what this does, look at EE210 to figure out
	// ssync() synchronizes the system
	*pEBIU_SDBCTL = 0x00000015;
	ssync();
		
	// Set refresh rate control register to RDIV refresh rate	
	*pEBIU_SDRRC = RDIV_Value;
	ssync();
	
	// No idea what these are, look at EE210 to figure out
	*pEBIU_SDGCTL = 0x0091998B;
	ssync();
}



void sdramFillMem(void)
{
	
	int i = 0;
	for (i = 0; i < TestSize; i++)
	{
		sdramTestBuff[i] = i;
	}


}



void sdramTestMem(void)
{
	int i, TestVal;
	for (i = 0; i < TestSize; i++)
	{
		TestVal = sdramTestBuff[i];
		if (TestVal != i)
		{
			*pPORTGIO_SET = LED1;
		
		
		}
		
	}
	
	*pPORTGIO_SET = LED0;

	printf("DONE!");

}


