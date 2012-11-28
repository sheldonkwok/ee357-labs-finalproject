/*
 * main implementation: use this sample to create your own application
 *
 */


#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#include "usc_support.h"
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

void leds_display(unsigned char cnt)
{
	MCF_GPIO_PORTTC = cnt;

}

/******************************************/
/* Global variable declaration
/******************************************/
int sample = 0;


int main(void)
{
	int counter = 0;
	int x=2000,y=2000,z=2000;

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif

	init_gpio();
	
    /****************************************/
	/* Setup ADC0 on M52259 Tower Board        */
    /****************************************/
    /* Setting PortAN0 as ADC functionality */
    MCF_GPIO_PANPAR |= MCF_GPIO_PANPAR_PANPAR3 
					  | MCF_GPIO_PANPAR_PANPAR2 
					  | MCF_GPIO_PANPAR_PANPAR1
					  | MCF_GPIO_PANPAR_PANPAR0;


    /****************************************/
    /* Setup ADC Module					    */
	/****************************************/

	/* Initialize the CRTL1 register to 0's with SMODE = Once Sequential = 000 */
    MCF_ADC_CTRL1 = 0 | MCF_ADC_CTRL1_SMODE(0);

    /* Setting divisor in CTRL2 register */
    MCF_ADC_CTRL2 = MCF_ADC_CTRL2_DIV(3);

    /* Setting Power Register appropriately - PUDELAY & clear PD0*/
    MCF_ADC_POWER = MCF_ADC_POWER_PUDELAY(4);

    /* Set AN0 to sample channel 3 once.  Note we will only look 	*/
    /* at the result of sample 0 even though the ADC will take  	*/
    /* an 8 sample scan 											*/
    MCF_ADC_ADLST1 = 0 | MCF_ADC_ADLST1_SAMPLE0(0)
						| MCF_ADC_ADLST1_SAMPLE1(1)
						| MCF_ADC_ADLST1_SAMPLE2(2)
						| MCF_ADC_ADLST1_SAMPLE3(3);

	/* Clear stop bit */
	MCF_ADC_CTRL1 &= ~MCF_ADC_CTRL1_STOP0;

	while(1){
		/* Toggle the LED's */
		MCF_GPIO_PORTTC = ~MCF_GPIO_PORTTC;

		/* Clear stop bit */
		MCF_ADC_CTRL1 &= ~MCF_ADC_CTRL1_STOP0;
	
		/* Set start bit */
		MCF_ADC_CTRL1 |= MCF_ADC_CTRL1_START0;
	
		/* Waiting for the last "Ready" bit to be set so we don't start the next scan to early*/
		while (!(MCF_ADC_ADSTAT & MCF_ADC_ADSTAT_RDY7));
	
		/* Toggle the LED's */
		MCF_GPIO_PORTTC = ~MCF_GPIO_PORTTC;

		x = MCF_ADC_ADRSLT(0) >> 3;
		y = MCF_ADC_ADRSLT(1) >> 3;
		z = MCF_ADC_ADRSLT(2) >> 3;
		sample = MCF_ADC_ADRSLT(3) >> 3;
	

		myprintf("Debug:  X=%d, Y=%d, Z=%d, POT=%d\r", x,y,z,sample);
//		if(x < 1600){
//			myprintf("X neg accel threshold exceeded\n");
//		}
//		else if(x > 2200){
//			myprintf("X pos accel threshold exceeded\n");
//		}
//		if(y < 1600){
//			myprintf("Y neg accel threshold exceeded\n");
//		}
//		else if(y > 2200){
//			myprintf("Y pos accel threshold exceeded\n");
//		}
//		if(z < 2000){
//			myprintf("Z neg accel threshold exceeded\n");
//		}
//		else if(z > 2700){
//			myprintf("Z pos accel threshold exceeded\n");
//		}

	}
	for(;;) {	   
	   	counter++;
	}
}
