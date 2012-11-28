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
	int counter = 0;
	char mymsg1[] = "You pressed button 1";
	char mymsg2[] = "You pressed button 2";
	char mybuf[15];
	uint8 reg_addr 		= ge_TS_LM75_TEMP_ADDR;    
	uint8 init0 = 0x00;
	uint8 init1 = 0x55;
    char *tempData = malloc(6);

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif
	p_lm75              = &lm75;

	init_gpio();
	i2c_init();
	// i2c initialization
	// writing 0x55 to 0x(4)A400F0, then writing 0x00 to 0x(4)A400FB
	i2c_write(0xF0, (uint8 *)&(init1), 0x01);
    i2c_write(0xFB, (uint8 *)&(init0), 0x01);


	init_lcd();                                         // initialize LCD display
    grphErase();                                        // clear
    grphText (2,2,FONT_SEVEN_DOT,(unsigned char *)"Hi World!");
    grphUpdate(SCRN_TOP,SCRN_BOTTOM);

  	while (1){
  		if(get_SW1_v2()){
			MCF_GPIO_PORTTH ^= MCF_GPIO_PORTTH_PORTTH0;
		    //grphText (2,2,FONT_SEVEN_DOT,(unsigned char *)"Hello World!");
		    //grphInvertArea(2,2,127,10);
		    grphUpdate(0,127);

		    i2c_write(0x00, init0, 0x01)
            i2c_read(0x08, tempData, 0x06);

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
	
	for(;;) {	   
	   	counter++;
	}
}