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
		// a1 - pointer for arr1
		// d0 - use for LED/Switch registers
		// d1 - temporarily hold 1st number and later average
		// d2 - temporarily hold 2nd number
		// d3 - hold loop count for waiting
		
		// Setting Up arr1 Pointer
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
		andi.l #0x0f,d1			// Clear 1st # except for 4 LSB
		
		move.b d1,(a1)			// Move 1st # to arr1[0]
		move.b (a1),0x4010000F 	// Display 1st # - arr1[0] into PORTTC (LED)
		clr.l d1				// Clear D1
		
		// Read and Loop Until Switch Input Changes
LOOP1:	move.b 0x40100044,d2	// Read 2nd # into D2
		not.l d2				// Invert bits of 2nd #
		lsr.l #4,d2				// Get rid of 4LSB bits of 2nd #
		andi.l #0x0f,d2			// Clear 2nd # except for 4 LSB
		
		move.b (a1),d1 			// Move 1st # to D1 - doing this as CMP is a longword operation
		CMP.l d2,d1				// Compare 1st # to 2nd # -> check if equal
		BEQ LOOP1				// Loop back if switch input is the same
		
		move.b (a1)+,d1 		// Increment to arr1[1] -> can't do this in the loop
		clr.l d1				// Clear D1
				
		// Store, Display 2nd Number
		move.b d2,(a1)			// Move 2nd # to arr1[1]
		move.b (a1),0x4010000F	// Display 2nd # - arr[1] into PORTTC (LED)
		clr.l d2				// Clear D2
		
		// Calculate Average Number
		move.b -(a1),d1			// Decrement to arr[0] + Move arr[0] to D1
		move.b (a1)+,d1			// Move arr[0] to D1 (again) + Increment to arr[1]
		move.b (a1)+,d2			// Move arr[1] to D2 + Increment to arr[2]
		add.l d2,d1				// Add two numbers together - doing this as ADD is a longword operation
		lsr.l #1,d1				// "Divide" by 2 to get Average # in D1
		move.b d1,(a1)			// Move Average # in D1 to arr1[2]
	
		// Loop To Wait (Total) 10 Seconds
		move.l #33333333,d3
LOOP2: 	SUBI.l #1,D3
		BNE LOOP2
		
		move.l #0,d0			// Initialize d0 for displaying nothing
		
		/* Blinking Average Loop */ 
		// Displaying Average #
AGAIN:	move.b (a1),0x4010000F 	// Display Average # - arr[2] into PORTTC (LED)
		
		// Loop to Wait 1 Second (on)
		move.l #3333333,d3
ON:  	SUBI.l #1,d3
		BNE ON
		
		// Displaying Nothing
		move.b d0,0x4010000F 	// Displaying Nothing - #0 into PORTTC (LED)
		
		// Loop to Wait 1 Second (off)
		move.l #3333333,d3
OFF: 	SUBI.l #1,d3
		BNE OFF
		
		BRA AGAIN 				// LOOP FOREVER - Keeps Blinking
		
		rts // never reaches here...
