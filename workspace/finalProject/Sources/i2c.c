/*
 * i2c.c
 *
 *  Created on: Apr 4, 2011
 *      Author: Mark
 */

#include "support_common.h"
#include "i2c.h"

void i2c_init()
{
	/* Set I2C0 pins for I2C */
    MCF_GPIO_PASPAR = MCF_GPIO_PASPAR_SCL0_SCL0 | MCF_GPIO_PASPAR_SDA0_SDA0;
	
	// set clock divider to 320 (13.333MHz / 320 = 41666 Kbps)
	MCF_I2C0_I2FDR = MCF_I2C_I2FDR_IC(0x34);
	// Init slave address, even though this is unneeded
	MCF_I2C0_I2ADR = 0x10;
	MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN;

}

void i2c_write(unsigned char dev_addr, unsigned char *dat, int n)
{
	uint8 x;
	// wait for bus to be free
	// if bus is busy, reset it per the instructions
	//  in MCF52259 Reference Manual
	if(MCF_I2C0_I2SR & MCF_I2C_I2SR_IBB){
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN | MCF_I2C_I2CR_MSTA;
		x = MCF_I2C0_I2DR;
		MCF_I2C0_I2SR = 0x00;
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN;
	}
		
	// wait for a previous transmission to complete
	//while(! (MCF_I2C0_I2SR & MCF_I2C_I2SR_ICF) );
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_MTX;
	if (MCF_I2C0_I2CR & MCF_I2C_I2CR_MSTA)
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_RSTA;
	else
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_MSTA;
	MCF_I2C0_I2DR = dev_addr;
	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	while((MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK));

	while(n > 0){
		if(!(MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK))
			break;
		MCF_I2C0_I2DR = *dat++;
		//while(MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK);
		while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
		MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
		n--;
	}
	MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_MSTA;
	
}

void i2c_read(unsigned char dev_addr, unsigned char *dat, int n)
{
	unsigned char x;
	int num_bytes;
//	x = MCF_I2C0_I2DR;
	
	// if bus is busy, reset it per the instructions
	//  in MCF52259 Reference Manual
	if(MCF_I2C0_I2SR & MCF_I2C_I2SR_IBB){
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN | MCF_I2C_I2CR_MSTA;
		x = MCF_I2C0_I2DR;
		MCF_I2C0_I2SR = 0x00;
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN;
	}
		 
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_MTX;
	if (MCF_I2C0_I2CR & MCF_I2C_I2CR_MSTA)
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_RSTA;
	else
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_MSTA;
	
	// Send device address & read op.
	MCF_I2C0_I2DR = dev_addr | 0x01;
	
	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	while((MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK));
	
	// Switch to Master Receive
	MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_MTX;
	if(n==1)
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_TXAK;		
	x = MCF_I2C0_I2DR;
	
//	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
//	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	//if (n==1)
	//	n++;

	num_bytes = n;
	while(n > 0){
		while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
		MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
		n--;
		if(n==0){
			MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_MSTA;
			MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_TXAK;
		}
		else if (n==1) {
			MCF_I2C0_I2CR |= MCF_I2C_I2CR_TXAK;
		}
		x = MCF_I2C0_I2DR;
		*dat++ = x;
//		if(num_bytes == n-1){
//			*dat = MCF_I2C0_I2DR;
//		}
//		else {
//			*dat++ = MCF_I2C0_I2DR;
//		}
		//if(n>0){
		//	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
		//	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
		//}
	}

}

void i2c_read_w_ptr(unsigned char dev_addr, unsigned char pointer, unsigned char *dat, int n)
{
	unsigned char x;
	int num_bytes;
//	x = MCF_I2C0_I2DR;
	
	// if bus is busy, reset it per the instructions
	//  in MCF52259 Reference Manual
	if(MCF_I2C0_I2SR & MCF_I2C_I2SR_IBB){
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN | MCF_I2C_I2CR_MSTA;
		x = MCF_I2C0_I2DR;
		MCF_I2C0_I2SR = 0x00;
		MCF_I2C0_I2CR = 0x00;
		MCF_I2C0_I2CR = MCF_I2C_I2CR_IEN;
	}
		 
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_MTX;
	if (MCF_I2C0_I2CR & MCF_I2C_I2CR_MSTA)
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_RSTA;
	else
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_MSTA;
	// Send device address & write op.
	MCF_I2C0_I2DR = dev_addr | 0x00;
	
	// Wait for transmission to complete
	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	while((MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK));

	// Transmit the pointer address to the LM75A
	MCF_I2C0_I2DR = pointer;
	
	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	while((MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK));

	// Generate a Re-Start 
	MCF_I2C0_I2CR |= MCF_I2C_I2CR_RSTA;
	
	// Send device address & read op.
	MCF_I2C0_I2DR = dev_addr | 0x01;
	
	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
//	while((MCF_I2C0_I2SR & MCF_I2C_I2SR_RXAK));
	
	// Switch to Master Receive
	MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_MTX;
	if(n==1)
		MCF_I2C0_I2CR |= MCF_I2C_I2CR_TXAK;		
	x = MCF_I2C0_I2DR;
	
//	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
//	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
	//if (n==1)
	//	n++;

	num_bytes = n;
	while(n > 0){
		while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
		MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
		n--;
		if(n==0){
			MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_MSTA;
			MCF_I2C0_I2CR &= ~MCF_I2C_I2CR_TXAK;
		}
		else if (n==1) {
			MCF_I2C0_I2CR |= MCF_I2C_I2CR_TXAK;
		}
		x = MCF_I2C0_I2DR;
		*dat++ = x;
//		if(num_bytes == n-1){
//			*dat = MCF_I2C0_I2DR;
//		}
//		else {
//			*dat++ = MCF_I2C0_I2DR;
//		}
		//if(n>0){
		//	while( !(MCF_I2C0_I2SR & MCF_I2C_I2SR_IIF) );
		//	MCF_I2C0_I2SR &= ~MCF_I2C_I2SR_IIF;
		//}
	}

}
