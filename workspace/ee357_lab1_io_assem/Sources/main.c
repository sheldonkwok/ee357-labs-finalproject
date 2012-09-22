/*
 * Johnny Jung - Lab 1
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

	asm{
		// Setup for Switch / LED
		move.l #0x0f,d0
		move.b d0,0x40100074 	// 00001111 into PDDPAR	(Switch)
		
		move.b d0,0x4010002C	// 00001111 into DDRDD	(Switch)
		
		move.b d0,0x40100027	// 00001111 into DDRTC	(LED)
		
		move.l #0,d0
		move.b d0,0x4010006F 	// 00000000 into PTCPAR	(LED)
		
		// Actual I/O
		move.b 0x40100044,d1	// move switch input to D1
		
		eori.l #0xf0,d1			// flip 4MSB bits -> can also do not.l d1
		lsr.l #4,d1				// get rid of 4LSB bits 
		move.b d1,0x4010000F	// D1 into PORTTC
		
		// Decrementing
	L1: subq.l #1, d1
		andi.l #0x0f, d1
		move.b d1,0x4010000F
		
		bra L1
		
		/*	ARI w/ Displacement 
		move.l #0x40100000,A1	// 0x40100000 into A1
		move.l #0x0f,d0			
		move.b d0,0x74(A1)		// 00001111 into PDDPAR
		move.b d0,0x2c(A1)		// 00001111 into DDRDD
		move.b d0,0x27(A1)		// 00001111 into DDRTC
		move.l #0,d0
		move.b d0,0x6F(A1)		// 00000000 into PTCPAR
		move.b 0x44(A1),d1		// move switch input to D2
		eori.l #0xf0,d1
		lsr.l #4,d1
		move.b d1,0x0F(A1)		// D2 into PORTTC
		
		// Decrementing
	L2: subq.l #1, d1
		andi.l #0x0f, d1
		move.b d1,0x0F(A1)
		bra L2
		*/
		
		
	}
	
	for(;;) { counter++; }
}
