/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */
#include "usc_support.h"
#include "lcd128x64.h"
#include "fonts.h"
#include "i2c.h"
#include <string.h>
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

void init_gpio()
{
	
	// Init. input buttons that are connected to PortTA
	MCF_GPIO_PTAPAR = 0 
	    | MCF_GPIO_PTAPAR_ICOC0_GPIO
	    | MCF_GPIO_PTAPAR_ICOC1_GPIO;
	/* Set Data Direction to all input */
	MCF_GPIO_DDRTA = 0;
	
    /* Enable 4 LED signals as GPIO */
    MCF_GPIO_PTCPAR = 0
        | MCF_GPIO_PTCPAR_DTIN3_GPIO
        | MCF_GPIO_PTCPAR_DTIN2_GPIO
        | MCF_GPIO_PTCPAR_DTIN1_GPIO
        | MCF_GPIO_PTCPAR_DTIN0_GPIO;
    
    /* Enable signals as digital outputs */
    MCF_GPIO_DDRTC = 0
        | MCF_GPIO_DDRTC_DDRTC3
        | MCF_GPIO_DDRTC_DDRTC2
        | MCF_GPIO_DDRTC_DDRTC1
        | MCF_GPIO_DDRTC_DDRTC0;

	/* Enable 2 LED's on iMn FM board */
	MCF_GPIO_PTHPAR = 0 
		| MCF_GPIO_PTHPAR_MB_D0_GPIO
		| MCF_GPIO_PTHPAR_MB_D2_GPIO;
	
	MCF_GPIO_DDRTH = MCF_GPIO_DDRTH_DDRTH0 | MCF_GPIO_DDRTH_DDRTH2;

}


int get_SW1_v2()
{
	int i;
	// Read the current state of the switch 
	if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0))
	{
		cpu_pause(5000);
		if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0))
		{
			// Wait until button is released
			while(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA0));
			return 1;	
		}
	}
	return 0;

}

int get_SW3_v2()
{
	int i;
	/* Read the current state of the switch */
	if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1))
	{
		cpu_pause(5000);
		if(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1))
		{
			// Wait until button is released
			while(!(MCF_GPIO_SETTA & MCF_GPIO_SETTA_SETTA1));
			return 1;			
		}
	}
	return 0;
}


int main(void)
{
	//Initiate variables
	int counter = 0;
	int second = 0;
	int decisecond = 0;
	char mybuf[15];
	char output[4];
	int started = 0; // started running or not
	int lap = 0; // in the lap state or not
	
	uint8 reg_addr 		= ge_TS_LM75_TEMP_ADDR;           

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif

	init_gpio();
	i2c_init();


    //Loop program
  	while (1){

  	}
}
