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


int main(void)
{
	int counter = 0;

#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
	printf("Hello World in C++ from MCF52259 derivative on MCF52259 board\n\r");
#endif
	

	
	asm {
		

		/* Question #3
		move.l #0xabcd0123,D0
		move.l #0x12345678,D1
		move.l D1,0x20003004
		move.w 0x20003004, D0
		move.w D0,0x20003006
		move.b D0,0x20003004
		move.w 0x20003004, D0
		move.b 0x20003007, D0
		move.w D0,0x20003004
		rts
		*/
		
		/* Question #7 
		MOVE.W #0x0000FFFF,D0 // I0
		MOVE.L 0x12345678,D2 // I1
		ADD.L -(A2),D2 // I2
		MOVE.W 7(A2),D1 // I3
		MOVE.W (A2)+, -4(A1,D2*2) // I4
		*/
		
	}
	
	
	for(;;) {	   
	   	counter++;
	}
}
