/*
 * EE357 - Lab 2
 * Johnny Jung / Sheldon Kwok
 *
 */
 
   		.data
NSAMP	.equ 3
arr1:	.space NSAMP
		.text 
		.global _main
		.global main
		.include "../Project_Headers/ee357_asm_lib_hdr.s"
_main:
main:			
		// Setting Up arr1 pointer
		movea.l #arr1,a1
		
		// Initialization of LED / Switch
		move.l #0x0f,d0
		move.b d0,0x40100074	// 00001111 into PDDPAR (Switch)
		move.b d0,0x4010002C 	// 00001111 into DDRDD (Switch)
		move.b d0,0x40100027	// 00001111 into DDRTC (LED)
		move.l #0,d0
		move.b d0,0x4010006F	// 00000000 into PTCPAR (LED)
		
		// Read, Store, Display 1st Number
		move.b 0x40100044,d1	// Move 1st # to D1
		not.l d1				// Invert bits of 1st #
		lsr.l #4,d1				// Get rid of 4LSB bits of 1st #
		andi.l #0x0f, d1		// Clear 1st # except for 4LSB bits
		move.b d1,(a1)			// Move 1st # to arr1[0]
		move.b (a1),0x4010000F 	// Display 1st # - D1 into PORTTC (LED)
		clr.l d1
		
		// Read and Loop Until Switch Input Changes
LOOP1:	move.b 0x40100044,d2	
		not.l d2
		lsr.l #4, d2
		andi.l #0x0f, d2
		
		move.b (a1),d1 			// move 1st # to d1 - doing this as CMP is a long-word operation
		CMP.l d2,d1
		BEQ LOOP1				// loop back if switch input is the same
		move.b (a1)+,d1 		// incrementing a1
		clr.l d1
		// arr[1] now
		
		// Store, Display 2nd Number
		move.b d2,(a1)			// Move 2nd # to arr1[1]
		move.b (a1),0x4010000F	// Display 2nd # - D1 into PORTTC (LED)
		clr.l d2
		
		// Calculate Average Number
		move.b -(a1),d1			// arr[0] now
		move.b (a1)+,d1			// redundant... but post-increment kinda sucks
		move.b (a1)+,d2			// arr[2] now
		add.l d2,d1
		lsr.l #1,d1
		move.b d1,(a1)			// Move Average # to arr1[2]
		
	
		// Loop To Wait (Total) 10 Seconds
		move.l #44444442, d3
LOOP2: 	SUBI.l #1,D3
		BNE LOOP2
		
AGAIN:	move.b (a1),0x4010000F // BLAH
		move.l #0,d0
		
		move.l #4444444,d3
LOOP3:  SUBI.l #1, d3
		BNE LOOP3
		
		move.b d0,0x4010000F
		move.l #4444444,d3
LOOP4: 	SUBI.l #1, d3
		BNE LOOP4
		JSR AGAIN
		
		/*
		move.b d1,0x4010000F
		move.b d0,0x4010000F
		move.b d1,0x4010000F */
				
		
/*
		// Flash Average in 1 Second Time Interval
LOOP3:			 */
		
		
		
/*	
L1:		subq.1 #1, d1
		andi.l #0x0f, d1
		move.b d1,0x4010000F
		bra L1 */
		
		rts // last line of code
