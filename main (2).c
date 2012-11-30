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

int main(void)
{	
	//http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
	//http://wiibrew.org/wiki/Wiimote/Extension_Controllers/
	//http://dangerousprototypes.com/2009/08/19/bus-pirate-wii-nunchuck-quick-guide/
	//http://www.nerdkits.com/forum/thread/972/
	
	char init1 = 0x00;
	int sx = 0; // 35 (left) to 228 (right) -> center = 128
	int sy = 0; // 27 (left) to 220 (right) -> center = 128
	/*int ax = 0; // 300 (fully tilted left) to 740 (fully tilted right) -> center = 512
	int ay = 0; // 280 (fully tilted backwards) to 720 (fully tilted forwards)
	int az = 0; // 320 (upside-down) to 760 (right-side up)
	int bc = 0; // 0 is pressed
	int bz = 0; // 0 is pressed
	
	
	
	
	
	
	int i = 0;*/
	//char mymsg1[] = "You pressed button 1";
	//char mymsg2[] = "You pressed button 2";
	unsigned char* init;
    unsigned char* tempData;
	//char mybuf[15];

    tempData = (unsigned char*) malloc(6);
	init = (unsigned char*) malloc(3);
	/*for(i=0;i<6;i++)
	{
		tempData[i]=0;
	}*/
	//p_lm75              = &lm75;

	i2c_init();
	
	// i2c initialization
	// writing 0x55 to 0x(4)A400F0, then writing 0x00 to 0x(4)A400FB
	init[0] = 0xA4;
	init[1] = 0xF0;
	init[2] = 0x55;
	i2c_write(0x52, (uint8 *)&(init), 0x03);
	
	// 

	init[0] = 0xA4;
	init[0] = 0xFB;
	init[1] = 0x00;
    i2c_write(0x52, (uint8 *)&(init), 0x03);
	
	/*init[0] = 0xA4;
	init[1] = 0xF0;
	init[1] = 0x55;
	i2c_write(0x52, (uint8 *)&(init), 0x02);
	init[0] = 0xFB;
	init[1] = 0x00;
    i2c_write(0x52, (uint8 *)&(init), 0x02);*/
	
	// maybe write 0x00 to 0x40?
	// maybe device address is 0x52
	
	//i2c_write(0xF0, (uint8 *)&(init1), 0x02);
    //i2c_write(0xFB, (uint8 *)&(init0), 0x02);
    //i2c_read(0xFC, (uint8 *)&(tempData), 0x01);

  	while (1){
  		
  		//init1 = 0x08;
	    //i2c_write(0xA4, (uint8 *)&(init1), 0x01);
        //i2c_read(0xA4, (uint8 *)&(tempData), 0x06);
  		
  		init[0] = 0xA4;
  		init[1] = 0x00;
  	    i2c_write(0x52, (uint8 *)&(init), 0x02);
        i2c_read_w_ptr(0x52, 0xA4,(uint8 *)&(tempData), 0x06);
  		// maybe read from 0x00?
 
  		sx = tempData[0];
  		sy = tempData[1];
  		printf(" SX: %u", sx);
  		printf(", SY: %u", sy);
  		/*ax = tempData[2];
  		ax = (ax << 2) | ((tempData[5] >> 2) & 3);
  		ay = tempData[3];
  		ay = (ay << 2) | ((tempData[5] >> 4) & 3);
  		az = tempData[4];
  		az = (az << 2) | ((tempData[5] >> 6) & 3);
		bc = (tempData[5] >> 1) & 1;
		bz = tempData[5] & 1;

  		//, AX: %u, AY: %u, AZ: %u, BC: %u, BZ: %u \n",sx, sy, ax, ay, az, bc, bz);
		*/
  		// not my stuff below

            //reg_addr = 0;
		    //i2c_write(0x92, (uint8 *)&(reg_addr), 0x01);
		    //i2c_read(0x92, (uint8 *)&(p_lm75->u16_temp), 0x02);// values are in struct
		    //i2c_read_w_ptr(0x92, reg_addr, (uint8 *)&(p_lm75->u16_temp), 0x02);	  
  	}
}
