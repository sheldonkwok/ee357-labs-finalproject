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
	
	asm{
		// Switch input
		move.l #0x0f,d0
		move.b d0,0x40100074 	// 00001111 into PDDPAR
		move.b d0,0x4010002C	// 00001111 into DDRDD
		move.b 0x40100044,d2	// move switch input to D2
		eori.l #0xf0,d2			// flip bits
		lsr.l #4,d2				// get rid of 4LSB bits 
		
		// LED output
		move.b d2,0x4010000F	// D2 into PORTTC
		move.l #0,d0
		move.b d0,0x4010006F 	// 00000000 into PTCPAR
		move.l #0x0f,d0			
		move.b d0,0x40100027	// 00001111 into DDRTC
		
		// Decrementing
	L2: subq.l #1, d2
		andi.l #0x0f, d2
		move.b d2,0x4010000F
		bra L2
		
	/*	Existing Incrementing
		clr.l d1
	L1:	subq.l #1, d1
		andi.l #0x0f, d1
		move.b d1,0x4010000F
		bra L1
	*/
		
		
	/*	ARI w/ Displacement
	 * 
		move.l #0x40100000,A1
		move.l #0x0f,d0
		move.b d0,(0x74,A1)
		move.b d0,(0x2c,A1)
		eori.l #0xf0,d2
		lsr.l #4,d2
		
		move.b d2,(0x0F,A1)
		move.l #0, d0
		move.b d0,(0x6F,A1)
		move.l #0x0f,d0
		move.b d0,(0x27,A1)
		
	L2: subq.l #1, d2
		andi.l #0x0f, d2
		move.b d2,(0x0F,A1)
		bra L2
	*/

	}
	
	for(;;) {	   
	   	counter++;
	}
}
