/*
 * EE357 - Lab 3
 * Johnny Jung / Sheldon Kwok
 *
 */
 
// allocating space for variables not stored in registers
   		.data
NSAMP	.equ 3
arr1:	.space NSAMP
signa:	.space 4
signb:	.space 4
signnew:.space 4
expa:	.space 4
expb:	.space 4
expnew: .space 4
fraca:	.space 4
fracb:	.space 4
fracnew:.space 4
agtb:	.space 1
output:	.space 4
output2:.space 4
blink:	.space 4
flag:	.space 1
subbing: .space 4

		.text 
		.global _main
		.global main
		.include "../Project_Headers/ee357_asm_lib_hdr.s"
_main:
main:	

		CLR.L D0
		CLR.L D1
		CLR.L D3
		MOVE.L 	D0,subbing // initialize subbing to 0 (to do addition first)
		
/* initialize D0 / D1 here */
		MOVE.L	#0x426,D0
		MOVE.L	#0x35E,D1
		//MOVE.L #0,D1
		//MOVE.L #0,D0
		//MOVE.L #%001110110101,D0
		//MOVE.L #%010000010110,D0
		//MOVE.L #%001110110101,D1
		//MOVE.L #%010000100110, D0 // change these to test
		//MOVE.L #%001101011110, D1 // change these to test
		//MOVE.L #%010111101011, D0 // change these to test
		//MOVE.L #%110111101100, D1 // change these to test
/* end initializing D0 / D1 */

		BSR LED_initialization
		BSR FP_addition
		BSR FP_subtraction	
		BSR Display_result

LED_initialization:

		move.l #0x0f,d7
		move.b d7,0x40100074	// 00001111 into PDDPAR (Switch)
		move.b d7,0x4010002C 	// 00001111 into DDRDD (Switch)
		move.b d7,0x40100027	// 00001111 into DDRTC (LED)
		move.l #0,d7
		move.b d7,0x4010006F	// 00000000 into PTCPAR (LED)
		move.b d7,0x4010000F	// 00000000 into PORTTC (LED)
		
		RTS	// return 
		
FP_addition:
		BSR Cal_expo // Calculate the exponent
		BSR Cal_frac // Calculate the fraction
		BSR Cal_sign // Calculate (determine) the sign
		BSR Do_normal // Do the (re)normalization if necessary
		BSR Do_round // Do the round to the nearest
		BSR Final_result // Finalize and format the result into D2 or D3
		
		RTS // return

Cal_expo: // Calculate the exponent
		//Get exponent values required
		MOVE.L D0,D3			// load opa into temp register d3
		LSL.L	#8,D3			// erase sign bit of opa
		LSL.L	#8,D3			// erase sign bit of opa
		LSL.L	#5,D3			// erase sign bit of opa
		LSR.L	#8,D3			// erase sign bit of opa
		LSR.L	#8,D3			// erase sign bit of opa
		LSR.L	#5,D3			// erase sign bit of opa
		LSR.L	#6,D3			// erase sign bit of opa
		SUBI.L	#15,D3			// subtract 15 because of excess 15
		MOVE.L	D3,expa			// move exponent to expa
		
		MOVE.L 	D1,D3			// load opb into temp register d3
		LSL.L	#8,D3			// erase sign bit of opb
		LSL.L	#8,D3			// erase sign bit of opb
		LSL.L	#5,D3			// erase sign bit of opb
		LSR.L	#8,D3			// erase sign bit of opb
		LSR.L	#8,D3			// erase sign bit of opb
		LSR.L	#5,D3			// erase sign bit of opb
		LSR.L	#6,D3			// erase sign bit of opb
		SUBI.L	#15,D3			// subtract 15 because of excess 15
		MOVE.L	D3,expb			// move exponent to expb
		
		//Get fraction values required
		MOVE.L D0,D3			// load opa into temp register d3
		LSL.L	#8,D3			// erase sign bit of opa
		LSL.L	#8,D3			// erase sign bit of opa
		LSL.L	#8,D3			// erase sign bit of opa
		LSL.L	#2,D3			// erase sign bit of opa
		LSR.L	#8,D3			// shift bits back to get exponent of a
		LSR.L	#8,D3			// shift bits back to get exponent of a
		LSR.L	#8,D3			// shift bits back to get exponent of a
		LSR.L	#2,D3			// shift bits back to get exponent of a
		MOVE.L	D3,fraca
		
		MOVE.L D1,D3			// load opa into temp register d3
		LSL.L	#8,D3			// erase sign bit of opb
		LSL.L	#8,D3			// erase sign bit of opb
		LSL.L	#8,D3			// erase sign bit of opb
		LSL.L	#2,D3			// erase sign bit of opb
		LSR.L	#8,D3			// shift bits back to get exponent of b
		LSR.L	#8,D3			// shift bits back to get exponent of b
		LSR.L	#8,D3			// shift bits back to get exponent of b
		LSR.L	#2,D3			// shift bits back to get exponent of b
		MOVE.L	D3,fracb
		
		//Get sign values required
		MOVE.L D0,D3			//load opa into temp register d3
		LSR.L	#8,D3			// shift opa by 8
		LSR.L	#3,D3			// shitt opa by 5
		MOVE.L D3,signa			//load sign of opa into signa
		
		MOVE.L D1,D3			//load opa into temp register d3
		LSR.L	#8,D3			// shift opb by 8
		LSR.L	#3,D3			// shitt opb by 5
		MOVE.L D3,signb			//load sign of opa into signb
		
		//Compare exponents 
		MOVE.L	expa,D3			// Check to see which operator has a greater value
		CMP.L	expb,D3
		BEQ		CMP_EQ			// if expa=expb, goto cmp_eq to compare frac fields		
		BGT		CMP_GT			// elsif expa>expb, go to CMP_GT to set agtb high
		BLT 	CMP_LT
		RTS

CMP_GT:	// compare exp greater than		
		MOVE.L	#1,D6			// set agtb flag to be true
		MOVE.L	D6,agtb
		MOVE.L	expa,D4			// set expa as the final exponent
		MOVE.L	D4,expnew
		RTS
CMP_LT: // compare exp less than 
		MOVE.L	#0,D6			// set agtb flat to be false
		MOVE.L	D6,agtb
		MOVE.L	expb,D4			// set expb as teh final exponent
		MOVE.L	D4,expnew
		RTS
CMP_EQ: // compare exp equals
		MOVE.L	fraca, D3		// expb == expa, compare fraca with fracb
		CMP.L	fracb, D3
		BGT		CMP_GT			// if fraca>fracb, set agtb high
		BLT		CMP_LT			// if fraca<fracb, set agtb low
		BEQ		CMP_LT			// if fraca==fracb, treat a<b			
		RTS

Cal_frac: // Calculate the fraction
		MOVE.L	#1, D6
		CMP.L	agtb, D6
		BEQ		SHFTB			// if a>b flag then shift b
		BNE		SHFTA			// else shift a

SUMDIF:		
		MOVE.L	signa, D3		// store sign values in D3
		MOVE.L	signb, D4
		CMP.L	D3, D4			// compare signa with signb
		BEQ		SUMFRAC			// if signs are the same, sum the magnitudes
		BNE		DIFFRAC			// if signs are different, compute the difference
		
		RTS

SUMFRAC:
		MOVE.L	fraca, D3		// store fraca in D3
		ADD.L	fracb, D3		// add fracb and fraca, stores in D3
		MOVE.L	D3,fracnew		// save the new fraction in fracnew
		RTS

DIFFRAC:
		MOVE.L	agtb, D5
		CMP.L	#1, D5			// compare agtb tag with 1
		BEQ		AMINUSB			// if a>b, AMINUSB
		BNE		BMINUSA			// if b>a, BMINUSA

AMINUSB:	
		MOVE.L	fraca,D3		// store fraca in D3
		SUB.L	fracb,D3		// d3 = d3 - fracb --> fraca(temp) = fraca - fracb
		MOVE.L	D3,fracnew		// save the new fraction in fracnew
		RTS
			
BMINUSA:
		MOVE.L	fracb,D3		// store fracb in D3
		SUB.L	fraca,D3		// d3 = d3 - fraca --> fracb(temp) = fracb - fraca
		MOVE.L	D3,fracnew		// save the new fraction in fracnew
		RTS


SHFTB:		
		MOVE.L	expa,D3			// save expa to D3
		SUB.L	expb,D3			// subtract expb from expa and store difference in D3
		MOVE.L	expa,D4			// 
		MOVE.L	D4, expb		// set expb = expa
		
		MOVE.L	fraca,D4
		MOVE.L	fracb,D5
		ADD.L	#%1000000,D4
		ADD.L	#%1000000,D5
		LSL.L	#3,D4			// add GRS
		LSL.L	#3,D5
		LSR.L	D3,D5			// shift to account for exponent
		
		MOVE.L	D4,fraca
		MOVE.L	D5,fracb
		BRA SUMDIF				//to SUMDIF

SHFTA:
		MOVE.L	expb, D3		// save expb to D3 
		SUB.L	expa,D3			// subtract expa from expb and store difference in D3
		MOVE.L	expb,D4
		MOVE.L	D4, expa		// set expa = expb
		
		MOVE.L	fraca,D4		// save fracs to  D4/5
		MOVE.L	fracb,D5
		ADD.L	#%1000000,D5	// add 1 before fractions
		ADD.L	#%1000000,D4
		LSL.L	#3,D5
		LSL.L	#3,D4			// shift fraca by D3
		LSR.L	D3,D4			// shift to account for exponent
		
		MOVE.L	D4,fraca		// store new numbers into fraca/b
		MOVE.L	D5,fracb
		BRA SUMDIF	

Cal_sign: // Calculate (determine) the sign
		MOVE.L	agtb,D4			// Check agtb tg
		CMPI.L	#1,D4			
		BEQ		AGTB	
		BNE		ALTB
		RTS

AGTB:	
		MOVE.L	signa,D3		// a>b, signnew = signa
		MOVE.L	D3, signnew
		RTS
			
ALTB:	
		MOVE.L	signb,D3		// a<b or a=b, signnew = signb
		MOVE.L	D3,signnew
		RTS	
		
Do_normal: // Do the (re)normalization if necessary
		MOVE.L	fracnew,D3
		LSR.L	#8,D3			// shift to farthest bit
		LSR.L	#2,D3
		CMPI.L	#1,D3			// check if 1 is there
		BEQ		HNDLCARRY
		
		MOVE.L	#%00000000000000000000001000000000,D5
		AND.L	fracnew,D5
		TST.L	D5
		BEQ		RENORMLEFT
		
		RTS

RENORMLEFT:
		MOVE.L 	fracnew,D3
		LSL.L	#1,D3
		MOVE.L	D3,fracnew
		
		MOVE.L	expnew,D4
		SUB.L	#1,D4
		MOVE.L	D4,expnew
		
		MOVE.L	#%00000000000000000000001000000000,D5 // test for proper alignment
		AND.L	fracnew,D5
		TST.L	D5
		BEQ		RENORMLEFT // loop renormleft
		RTS

HNDLCARRY:		
		MOVE.L 	#1,D5
		AND.L   subbing,D5		// check if subtraction
		TST.L 	D5				// compare
		BEQ		CARRYSUBTRACTION
		BNE 	CARRYADDITION

CARRYADDITION:
		MOVE.L 	fracnew,D3
		LSR.L	#1,D3			// shift frac 1 right
		MOVE.L	D3,fracnew
		
		MOVE.L	expnew,D4
		ADD.L	#1,D4			// add 1 to exp
		MOVE.L	D4,expnew

		RTS

CARRYSUBTRACTION:
		MOVE.L 	fracnew,D4
		LSL.L 	#8,D4  			// drop the carry		
		LSL.L 	#8,D4
		LSL.L 	#5,D4
		LSR.L 	#8,D4  			// shift back		
		LSR.L 	#8,D4
		LSR.L 	#5,D4

		RTS
		
Do_round: // Do the round to the nearest
		MOVE.L	fracnew,D3
		LSL.L	#8,D3			// remove 1 before decimal
		LSL.L	#8,D3
		LSL.L	#7,D3
		LSR.L	#8,D3			// shift back
		LSR.L	#8,D3
		LSR.L	#7,D3
		MOVE.L	D3,fracnew
		
		MOVE.L	#%00000000000000000000000000000100,D5		// check for g bit
		AND.L	fracnew,D5
		TST.L	D5				// test for g bit
		BEQ		TRUNCATE		// g=0 truncate
		BNE		CHECKRS			// g!=0 check rs bits

CHECKRS:
		MOVE.L	#%00000000000000000000000000000011,D5		// check rs bits		
		AND.L	fracnew,D5
		TST.L	D5
		BNE		ROUNDUP
		BEQ		MIDROUND
		
ROUNDUP:
		MOVE.L	#%00000000000000000000000000001000,D4		// round up value
		ADD.L	D4,fracnew
		
		MOVE.L	#%00000000000000000000001000000000,D5			// mask for carry
		AND.L	fracnew,D5		// test for carry
		TST.L	D5
		BEQ 	TRUNCATE		// if no carry bit,truncate
		MOVE.L	fracnew,D6
		LSR.L	#1,D6
		MOVE.L	D6, fracnew								
		MOVE.L	expnew, D4
		ADDI.L	#1,D4			// update exponent b/c of carry
		MOVE.L	D4, expnew
		BRA		TRUNCATE		

MIDROUND:
		MOVE.L	#0x00000008,D5	// check lsb w/ mask
		AND.L	fracnew,D5		// check fracnew with mask
		TST.L	D5				// check if lsb = 0
		BEQ		TRUNCATE		// if lsb = 0, TRUNCATE
		MOVE.L	#0x8, D6		
		ADD.L	D6,fracnew		// else, roundup
		MOVE.L	#%00000000000000000000001000000000,D5	// check carry-bit
		AND.L	fracnew,D5								
		TST.L	D5				// check if carry-bit = 0
		BEQ		TRUNCATE		// if carry-bit = 0, TRUNCATE
		MOVE.L	fracnew, D7
		LSR.L	#1,D7			// else, shift fracnew to the right by one
		MOVE.L	D7, fracnew
		MOVE.L	expnew, D4
		ADDI.L	#1,D4			// update exponent b/c of carry
		MOVE.L	D4, expnew
		BRA		TRUNCATE

TRUNCATE:		
		MOVE.L	fracnew,D4
		LSR.L	#3,D4			// drop grs bits
		MOVE.L	D4,fracnew
		RTS
		
Final_result: // Finalize and format the result into D2 or D3
		MOVE.L 	#1,D4
		AND.L   subbing,D4		// check if subtraction
		TST.L 	D4				// compare
		BNE		FINALS
		BEQ		FINALA

FINALA:
		MOVE.L	signnew, D5		// load sign of output	
		LSL.L	#8,D5			// shift left
		LSL.L	#3,D5			// shift left
		MOVE.L	D5,	output
		
		MOVE.L 	expnew,D5
		ADDI.L	#15,D5			// excess 15
		LSL.L	#6,D5			// shift to exponent
		ADD.L	D5, output		
				
		MOVE.L	fracnew, D5	
		ADD.L	D5, output		// just add to output
		MOVE.L	output, D2		// output to D2			
		RTS
FINALS:
		MOVE.L	signnew, D5		// load sign of output	
		LSL.L	#8,D5			// shift left
		LSL.L	#3,D5			// shift left
		MOVE.L	D5,	output2
		
		MOVE.L 	expnew,D5
		ADDI.L	#15,D5			// excess 15
		LSL.L	#6,D5			// shift to exponent
		ADD.L	D5, output2		
				
		MOVE.L	fracnew, D5	
		ADD.L	D5, output2		// just add to output
		MOVE.L	output2, D3		// output to D3			
		RTS
		
FP_subtraction: // don't change this!
		BSR Conv_sub // Convert subtraciton to addition
		BSR FP_addition // Now it's an addition problem
		// figure out a way to store it into d3 instead of d2 though...
		RTS
		
Conv_sub:
		MOVE.L 	signb,D4
		ADDI.L 	#1,D4			// flip sign by adding
		LSL.L  	#8,D4			// pop off excess by shifting
		LSL.L  	#8,D4
		LSL.L  	#8,D4
		LSL.L  	#7,D4
		LSR.L  	#8,D4
		LSR.L  	#8,D4
		LSR.L  	#8,D4
		LSR.L  	#7,D4
		MOVE.L 	D4,signb		// move new sign to signb
		
		
		MOVE.L 	#1,D4
		MOVE.L  D4,subbing
		
		MOVE.L	D1,D5			// resassign values to D1
		ADDI.L	#%100000000000,D5
		LSL.L	#8,D5			// proper shifting
		LSL.L	#8,D5
		LSL.L	#4,D5
		LSR.L	#8,D5
		LSR.L	#8,D5
		LSR.L	#4,D5
		MOVE.L	D5,D1
		
		RTS
		
Display_result:

BLINK: 	move.l d2,d7
		lsr.l #8,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move first 4 bits into LED
				
		move.l #6666666,d4 // Loop To Wait (Show Add 4MSB) 2 Seconds
LOOP1: 	SUBI.l #1,d4
		BNE LOOP1
	
		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
		
		move.l #6666666,d4 // Loop To Wait (Show Nothing) 2 Seconds
LOOP2: 	SUBI.l #1,d4
		BNE LOOP2
		
		move.l d2,d7
		lsr.l #4,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move next 4 bits into LED	
		
		move.l #6666666,d4 // Loop To Wait (Show Next 4MSB) 2 Seconds
LOOP3: 	SUBI.l #1,d4
		BNE LOOP3
	
		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
				
		move.l #6666666,d4 // Loop To Wait (Show Nothing) 2 Seconds
LOOP4: 	SUBI.l #1,d4
		BNE LOOP4
	
		move.l d2,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move last 4 bits into LED			
		
		move.l #6666666,d4 // Loop To Wait (Show Next 4MSB) 2 Seconds
LOOP5: 	SUBI.l #1,d4
		BNE LOOP5
		
		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
		
		move.l #13333333,d4 // Loop To Wait (Show Nothing) 4 Seconds
LOOP6: 	SUBI.l #1,d4
		BNE LOOP6

	// showing 2nd number now
		
		move.l d3,d7
		lsr.l #8,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move first 4 bits into LED
				
		move.l #6666666,d4 // Loop To Wait (Show Add 4MSB) 2 Seconds
LOOP7: 	SUBI.l #1,d4
		BNE LOOP7
	
		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
		
		move.l #6666666,d4 // Loop To Wait (Show Nothing) 2 Seconds
LOOP8: 	SUBI.l #1,d4
		BNE LOOP8
		
		move.l d3,d7
		lsr.l #4,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move next 4 bits into LED	
		
		move.l #6666666,d4 // Loop To Wait (Show Next 4MSB) 2 Seconds
LOOP9: 	SUBI.l #1,d4
		BNE LOOP9

		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
				
		move.l #6666666,d7 // Loop To Wait (Show Nothing) 2 Seconds
LOOP10: 	SUBI.l #1,d7
		BNE LOOP10
	
		move.l d3,d7
		andi.l #0x0f,d7		// Clear 1st # except for 4 LSB
		move.b d7,0x4010000F; // move last 4 bits into LED			
		
		move.l #6666666,d4 // Loop To Wait (Show Next 4MSB) 2 Seconds
LOOP11: 	SUBI.l #1,d4
		BNE LOOP11

		move.b #0,d7
		move.b d7,0x4010000F; // move 0 into LED	
		
		move.l #13333333,d4 // Loop To Wait (Show Nothing) 2 Seconds
LOOP12: 	SUBI.l #1,d4
		BNE LOOP12

		BRA BLINK
