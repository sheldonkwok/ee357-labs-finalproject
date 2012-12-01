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
#include <stdlib.h>
#endif

//function that initializes joy stick input 
void init_joy()
{  
   /* Enable signals as digital outputs */
    MCF_GPIO_DDRTJ = 0
        | MCF_GPIO_DDRTJ_DDRTJ0
        | MCF_GPIO_DDRTJ_DDRTJ1
        | MCF_GPIO_DDRTJ_DDRTJ6
        | MCF_GPIO_DDRTJ_DDRTJ7 | MCF_GPIO_DDRTJ_DDRTJ2;
    MCF_GPIO_PORTTJ = 0xff; 
}
//function that outputs a 1 if joy stick switch is left
int get_lt_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ0));
        return 1;            
        }
    }
    return 0;
}

//function that outputs a 1 if joy stick switch is right
int get_rt_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ1));
        return 1;            
        }
    }
    return 0;
}

//function that outputs a 1 if joy stick switch is down
int get_down_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ6));
        return 1;            
        }
    }
    return 0;
}

//added a new function that outputs a 1 if switch is up
int get_up_sw_v2()
{
    int i;
    /* Read the current state of the switch */
    if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7))
    {
        //cpu_pause(5000);
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7))
        {
        // Wait until button is released
        if(!(MCF_GPIO_SETTJ & MCF_GPIO_SETTJ_SETTJ7));
        return 1;            
        }
    }
    return 0;
} 

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

int sample = 0;

int main(void)
{
	// A character on screen.... can't move beyond edge
	// move it around using the joystick.
	// detect sudden change in accelerometer....  output text on bottom (show for a couple frames)
	// button press triggers boolean or something... output text on bottom (show for a couple frames)
	
	// 0 to 121
	// 0 to 54
	//Initiate variables
	int dx = 121; // x location
	int dy = 54; // y location
	int second = 0;
	char player;
	int left;
	int right;
	int up;
	int down;
	int decisecond = 0;
	int x=2000,y=2000,z=2000;
	
	char xbuf[1];
	char ybuf[15];
	char output[4];

	int started = 0; // started running or not
	int lap = 0; // in the lap state or not
	
	uint8 reg_addr 		= ge_TS_LM75_TEMP_ADDR;           

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
	
	init_joy();
//	i2c_init();
	
	init_lcd();                                         // initialize LCD display


    //Loop program
  	while (1){

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



// change after here.
		myprintf("Debug:  X=%d, Y=%d, Z=%d, POT=%d\r", x,y,z,sample);
  		
  		left = get_lt_sw_v2();
  		right = get_rt_sw_v2();
  		down = get_down_sw_v2();
  		up = get_up_sw_v2();
  		
  		if(left)
  		{  			
  	  		grphErase();
			if(dx>0)dx--;		
		}
  		
  		if(right)
  		{
  	  		grphErase();
			if(dx<121)dx++;	
  		}
  		
  		if(down)
  		{
  	  		grphErase();
			if(dy<54)dy++;	
  		}
  		
  		if(up)
  		{
  	  		grphErase();
			if(dy>0)dy--;
  		}
  		
  		sprintf(xbuf,"A");  		
  		output[0] = xbuf[0];
  		output[1] = xbuf[1];
  		output[2] = xbuf[2];
  		output[3] = xbuf[3];
		grphText(dx,dy,FONT_NINE_DOT,(unsigned char *)output);
		grphUpdate(SCRN_TOP,SCRN_BOTTOM);


  		// below is from lab 4

		/*if(second < 10) {
			output[0] = '0';
			output[1] = mybuf[0];
		} else {
			output[0] = mybuf[0];
			output[1] = mybuf[1];
		}
		
		
		//
		
  		
  		//Keep internal timer
  		if(started) {
  			//cpu_pause(10000); //pause for a tenth of a second. Not exactly accurage b/c rest of code takes time to run. In theory it's 100000
  			decisecond++;
  			if(decisecond == 10) {
  				decisecond = 0;
  				second++;
  			}
  			
  			//Reset timer count
  			if(second == 60 && decisecond == 0) {
  				second = 0;
  				decisecond = 0;
  			}

  			//Generate output
  			sprintf(mybuf,"%d",second);
			if(second < 10) {
				output[0] = '0';
				output[1] = mybuf[0];
			} else {
				output[0] = mybuf[0];
				output[1] = mybuf[1];
			}
			
			output[2] = ':';
			
			sprintf(mybuf,"%d",decisecond);
			output[3] = mybuf[0];
			
			//printf(output);
			//printf("\n");
			
			//Don't graph output if in lap state
  			if(!lap) {		
				//grphErase();
				grphText(20,10,FONT_NINE_DOT,(unsigned char *)output);
				grphUpdate(SCRN_TOP,SCRN_BOTTOM);
  			} 
  		}
  		
  		//Button 1 pressed
  		if(get_SW1_v2()){
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTH_PORTTH0;
			//printf("button1");
			//printf("\n");
			if(lap){
				lap = 0;
			} else {
				started = started ? 0 : 1;
			}
			
  		}
  		
  		//Button 3 pressed
  		if(get_SW3_v2()){		
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTJ_PORTTJ2;
		    //printf("button2");
		    //printf("\n");
		    
		    //Determine whether to reset or change lap state
		    if(lap || !started) {
				second = 0;
				decisecond = 0;
				//printf("Resetting to 00:0");
				//printf("\n");
				started = 0;
				lap = 0;
				//grphErase();                                       
				grphText (20,10,FONT_NINE_DOT,(unsigned char *)"00:0");
				grphUpdate(SCRN_TOP,SCRN_BOTTOM);
			} else {
				lap = lap ? 0 : 1;
			}
  		}*/
  		
		
  	}
}
