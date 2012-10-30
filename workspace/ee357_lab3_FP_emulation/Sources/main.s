/*
 * EE357 - Lab 3
 * Johnny Jung / Sheldon Kwok
 *
 */
 
   		.data
NSAMP	.equ 3
arr1:	.space NSAMP
/*
signa:	
signb:
expa:
expb:
fraca:
fracb:	
a_ge_b:*/
		.text 
		.global _main
		.global main
		.include "../Project_Headers/ee357_asm_lib_hdr.s"
_main:
main:	// don't change except for below values
		move.l #110100001110, D0 // change these to test
		move.l #010010110011, D1 // change these to test
		BSR LED_initialization
		BSR FP_addition
		BSR FP_subtraction
		BSR Display_result

LED_initialization:

		// code from lab 2
		RTS
		
FP_addition: // don't change this!
		BSR Cal_expo // Calculate the exponent
		BSR Cal_frac // Calculate the fraction
		BSR Cal_sign // Calculate (determine) the sign
		BSR Do_normal // Do the (re)normalization if necessary
		BSR Do_round // Do the round to the nearest
		BSR Final_result // Finalize and format the result into D2 or D3
		RTS

Cal_expo: // Calculate the exponent
	
		// code here
		RTS

Cal_frac: // Calculate the fraction

		// code here
		RTS

Cal_sign: // Calculate (determine) the sign

		// code here
		RTS
		
Do_normal: // Do the (re)normalization if necessary

		// code here
		RTS
Do_round: // Do the round to the nearest

		// code here
		RTS
		
Final_result: // Finalize and format the result into D2 or D3

		// code here
		RTS

FP_subtraction: // don't change this!
		BSR Conv_sub // Convert subtraciton to addition
		BSR FP_addition // Now it's an addition problem
		// figure out a way to store it into d3 instead of d2 though...
		RTS
		
Conv_sub:

		// 1. Compare sign bits
		// 2. Load two sign bits from d0, d1
		RTS
		
Display_result:

		// code here
		// final result must be stored in last (LSB) 12 results of D2 and D3 respectively.
		// blinking code in lab 2
		RTS
