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

ts_lm75     lm75;              // create struct for Temp Sensor
ts_lm75     *p_lm75;

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
	
	//http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
	//http://wiibrew.org/wiki/Wiimote/Extension_Controllers/
	
	uint8 reg_addr 		= ge_TS_LM75_TEMP_ADDR;    
	uint8 init0 = 0x00;
	uint8 init1 = 0x55;
	int counter = 0;
	int sx = 0; // 35 (left) to 228 (right) -> center = 128
	int sy = 0; // 27 (left) to 220 (right) -> center = 128
	int ax = 0; // 300 (fully tilted left) to 740 (fully tilted right) -> center = 512
	int ay = 0; // 280 (fully tilted backwards) to 720 (fully tilted forwards)
	int az = 0; // 320 (upside-down) to 760 (right-side up)
	int bc = 0; // 0 is pressed
	int bz = 0; // 0 is pressed
		
	char mymsg1[] = "You pressed button 1";
	char mymsg2[] = "You pressed button 2";
	char mybuf[15];
    unsigned char* tempData;
    tempData = (unsigned char*) malloc(6);

	p_lm75              = &lm75;

	init_gpio();
	i2c_init();
	// i2c initialization
	// writing 0x55 to 0x(4)A400F0, then writing 0x00 to 0x(4)A400FB
	//i2c_write(0xF0, (uint8 *)&(init1), 0x01);
    //i2c_write(0xFB, (uint8 *)&(init0), 0x01);

	//init_lcd();                                         // initialize LCD display
    //grphErase();                                        // clear
    //grphText (2,2,FONT_SEVEN_DOT,(unsigned char *)"Hi World!");
    //grphUpdate(SCRN_TOP,SCRN_BOTTOM);

  	while (1){
  		
	    //i2c_write(0x00, (uint8 *)&(init0), 0x01);
        //i2c_read(0x08, (uint8 *)&(tempData), 0x06);
  		
  		tempData[0] = 144;
  		tempData[1] = 202;
  		tempData[2] = 46;
  		tempData[3] = 245;
  		tempData[4] = 107;
  		tempData[5] = 178;

  		sx = tempData[0];
  		printf(" SX: %u",sx);
  		sy = tempData[1];
  		printf(" SY: %u",sy);
  		ax = tempData[2];
  		ax = (ax << 2) | ((tempData[5] >> 2) & 3);
  		printf(" AX: %u",ax);
  		ay = tempData[3];
  		ay = (ay << 2) | ((tempData[5] >> 4) & 3);
  		printf(" AY: %u",ay);
  		az = tempData[4];
  		az = (az << 2) | ((tempData[5] >> 6) & 3);
  		printf(" AZ: %u",az);
		bc = (tempData[5] >> 1) & 1;
 		printf(" BC: %u",bc);
		bz = tempData[5] & 1;
 		printf(" BZ: %u",bz);
 		printf("\n");
  		
  		if(get_SW1_v2()){
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTH_PORTTH0;
		    //grphText (2,2,FONT_SEVEN_DOT,(unsigned char *)"Hello World!");
		    //grphInvertArea(2,2,127,10);
		    grphUpdate(0,127);
            //reg_addr = 0;
		    //i2c_write(0x92, (uint8 *)&(reg_addr), 0x01);
		    //i2c_read(0x92, (uint8 *)&(p_lm75->u16_temp), 0x02);// values are in struct
		    //i2c_read_w_ptr(0x92, reg_addr, (uint8 *)&(p_lm75->u16_temp), 0x02);
		    grphHorizLine(2,126,31);
		    grphVertLine(1,62,63);
		    //p_lm75->u16_temp = (p_lm75->u16_temp) >> 8;      // shift rt 5
            //snprintf(mybuf,15,"Temp. is %d C",p_lm75->u16_temp);
		    grphText (2,2,FONT_SEVEN_DOT,(unsigned char *)mybuf);
		    grphInvertArea(2,2,127,10);
		    grphUpdate(0,127);
            printf("%s\n",mybuf);
  		}
  		if(get_SW3_v2()){		
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTJ_PORTTJ2;
		    grphErase();
		    grphUpdate(0,127);
  		} 
  	}
}
