/*============================================================================================================
                                $RCSfile: lcd128x64.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.9 $
       $Date: 2007/06/08 23:20:36 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/lcd128x64.c,v $

=============================================================================================================*/
/***************************************************************************
 *                                                                         *
 *   FILE: LCD106x56.C                                                     *
 *   LCD Display Controller Interface Routines for use with Tian-ma        *
 *   128x64 Graphics module with onboard S6B0724X01-B0CY controller        *
 *                                                                         *
 *   Copyright (C) 2003 by Carousel Design Solutions                       *
 *                                                                         *
 *                                  Written by:                            *
 *                                  Michael J. Karas                       *
 *                                  Carousel Design Solutions              *
 *                                  4217 Grimes Ave South                  *
 *                                  Edina MN 55416                         *
 *                                  (952) 929-7537                         *
 *                                                                         *
 ***************************************************************************/

#include "TypeDef.h"
#include "lcdCntrlDef.h"
#include "lcdConstDef.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "grphSCL.h"
#include "grphRS.h"
#include "grphSI.h"
#include "grphRS.h"
#include "grphRESET.h"

#include "lcd128x64.h"

#include "lcdStructDef.h"

#include "ExtGlobalVarDeclar.h"

#include "FcnDef.h"
#include "lcdFcnDef.h"

// pixel level bit masks for display
// this array is setup to map the order
// of bits in a byte to the vertical order
// of bits at the LCD controller

//rwl #pragma DATA_SEG DEFAULT

const uint8 l_mask_array[8] =

         {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

// the LCD display image memory
// buffer arranged so page memory is sequential in RAM


uint8 l_display_array[Y_BYTES][X_BYTES];


/* 
**
** Low level LCD Controller Interface Support Routines
** The LCD Controller interfaces to the microcontroller
** using the connections as shown below. 

   Notes:           S6B0724 controller for HDG12864L-7 graphics module
                            RS - register mode select
                                    0 - command
                                    1 - data
                            CS1 - chip select
                                    0 - always
                            SI  - serial input
                            SCL - serial clock
                            RES - reset device, low to reset
*/

/*
** 
** routine to initialize the operation of the LCD display subsystem
**
*/

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphInit )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       inits device port, and resets controller
    
    Input:          none

    Return:         none
    
    Notes:          Recommended by Hantronix, but doesn't seem to correct startup problem, not in use
----------------------------------------------------------------------------------------------------*/
void grphInit (void)
{
//uint16        lcd_contrast = LCD_CONTRAST, lcd_background = LCD_BACKGROUND;
//

    grphRESET_SetVal();                                 // set RST signal high off output
    TimeDelay(ge_T_50_MS);                              // wait
                                                        
    grphRS_ClrVal();                                    // set the CD line low as output
    TimeDelay(ge_T_50_MS);                              // wait

    grphSCL_SetVal();                                   // set SCLK line high
    TimeDelay(ge_T_50_MS);                              // wait

    grphSI_ClrVal();                                    // set SDAT line low
    TimeDelay(ge_T_50_MS);                              // wait

    grphRESET_ClrVal();                                 // reset the LCD controller chip, set the reset line low
    TimeDelay(ge_T_50_MS);                              // wait
                                                       
    grphRESET_SetVal();                                 // release reset to back high
    TimeDelay(ge_T_50_MS);                              // wait for convertor on graphic display

    grphOutCtrl(LCD_SET_ADC_REV);                       // set ADC reverse
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_SHL_NOR);                       // set SHL normal
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_BIAS_0);                        // set for the low bias mode
    TimeDelay(ge_T_100_MS);                             // wait

    grphOutCtrl(LCD_PWR_CTL+5);                         // turn on the VC bit
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_PWR_CTL+6);                         // now turn on VC+VR bits
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_PWR_CTL+7);                         // now turn on the VC+VR+VF
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_REG_RESISTOR + 4);                  // set default resistor ratio
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_REF_VOLT_MODE);                     // prime for the reference voltage
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_REF_VOLT_REG + 0x22);               // set default reference voltage select
    TimeDelay(ge_T_50_MS);                              // wait for regulator on graphic display

    grphOutCtrl(LCD_DISP_ON);                           // put the display on
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_LINE+0);                        // set line for row 0 of display
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_PAGE+0);                        // set page 0
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_COL_HI+0);                      // set column 0
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_SET_COL_LO+0);
    TimeDelay(ge_T_50_MS);                              // wait

    grphBackground(LCD_BACKGROUND);
    TimeDelay(ge_T_50_MS);                              // wait

    grphContrast(LCD_CONTRAST);
    TimeDelay(ge_T_50_MS);                              // wait

    return;
}                           //end-fcn_grphInit 

void grphInit_new (void)
{
    grphRESET_SetVal();                                 // set RST signal high off output
                                                        
    grphRESET_ClrVal();                                 // reset the LCD controller chip, set the reset line low
    grphRS_ClrVal();                                    // set the CD line low as output


    grphSCL_ClrVal();                                   // set SCLK line high
    grphSI_ClrVal();                                    // set SDAT line low
    grphRESET_SetVal();                                 // set RST signal high off output

    grphFirstCharInit(LCD_SET_ADC_REV);                 // set ADC reverse
    TimeDelay(ge_T_50_MS);                              // wait for convertor on graphic display

//    grphOutCtrl(LCD_SET_ADC_REV);                     // set ADC reverse
    grphOutCtrl(LCD_SET_SHL_NOR);                       // set SHL normal

    grphOutCtrl(LCD_SET_BIAS_0);                        // set for the low bias mode
    grphOutCtrl(LCD_PWR_CTL+5);                         // turn on the VC bit
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_PWR_CTL+6);                         // now turn on VC+VR bits
    TimeDelay(ge_T_50_MS);                              // wait

    grphOutCtrl(LCD_PWR_CTL+7);                         // now turn on the VC+VR+VF
    grphOutCtrl(LCD_REG_RESISTOR + 4);                  // set default resistor ratio
    grphOutCtrl(LCD_REF_VOLT_MODE);                     // prime for the reference voltage
    grphOutCtrl(LCD_REF_VOLT_REG + 0x22);               // set default reference voltage select
    TimeDelay(ge_T_50_MS);                              // wait for regulator on graphic display

    grphOutCtrl(LCD_DISP_ON);                           // put the display on

    grphOutCtrl(LCD_SET_LINE+0);                        // set line for row 0 of display
    grphOutCtrl(LCD_SET_PAGE+0);                        // set page 0
    grphOutCtrl(LCD_SET_COL_HI+0);                      // set column 0
    grphOutCtrl(LCD_SET_COL_LO+0);


    grphBackground(LCD_BACKGROUND);
    TimeDelay(ge_T_50_MS);                              // wait

    grphContrast(LCD_CONTRAST);
    TimeDelay(ge_T_50_MS);                              // wait


    return;
}                           //end-fcn_grphInit 

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphFirstCharInit ))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphFirstCharInit

    Input:          char dat

    Return:         None

    Notes:         special routine to send the first byte out after power up.
                   first bit output must be done when RES line low 
                   then send next 7 bits normally
----------------------------------------------------------------------------------------------------*/
void grphFirstCharInit (uint8 dat)
{
uint8 i = 7;                                            // serial bit counter

    grphSCL_SetVal();                                   // set clock line high      
    grphRESET_SetVal();                                   // set RST signal high off output
    grphRESET_ClrVal();                                   // LCD controller chip into reset state, set the reset line low
    TimeDelay(ge_T_50_MS);                                 // wait
    grphRS_ClrVal();                                    // set the RS line low for control cmd

    grphSCL_ClrVal();                                   // arm the clock      
    if(dat & 0x80){                                     // get 1st bit
        grphSI_SetVal();
    }else {
        grphSI_ClrVal();
    }
    TimeDelay(ge_T_10_MS);                                 // wait
    grphRESET_SetVal();                                   // set RST signal high off output
    TimeDelay(ge_T_10_MS);                                 // wait
    grphSCL_SetVal();                                   // clock first bit out      
//    TimeDelay(ge_T_50_MS);                               // wait

    while(i--){
        grphSCL_ClrVal();                               // arm the clock      
        dat <<= 1;                                      // shift data left 1 place
        if(dat & 0x80){                                 // get 1st bit
            grphSI_SetVal();
        }else {
            grphSI_ClrVal();
        }
        TimeDelay(ge_T_10_MS);                              // wait
        grphSCL_SetVal();                               // clock the bit out      
    }

    return;
}                           // end-fcn_grphOutCtrl
 

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphBackground.h ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Sets background ratio

    Input:          uint8 grph_background

    Return:         None

    Notes:
----------------------------------------------------------------------------------------------------*/
void grphBackground (uint8 grph_background)
{
    grphOutCtrl(LCD_REG_RESISTOR + grph_background);    // set resistor ratio

    return;                 //end-fcn_Background
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphContrast.h ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Sets contrast ratio

    Input:          uint8 grph_contrast

    Return:         None

    Notes:
----------------------------------------------------------------------------------------------------*/
void grphContrast (uint8 grph_contrast)
{
        grphOutCtrl(LCD_REF_VOLT_MODE);                 // prime for the reference voltage
        grphOutCtrl(LCD_REF_VOLT_REG + grph_contrast);  // set reference voltage select

    return;                 //end-fcn_grphContrast
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphOutDat ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Sets background ratio

    Input:          char dat

    Return:         None

    Notes:          low level routine to send a byte value out the serial bus
                    to the LCD controller data register. entry argument
                    is the data to output.
----------------------------------------------------------------------------------------------------*/
void grphOutDat (char dat)
{
    uint8 i=8;                                  // serial bit counter

    grphRS_SetVal();                                // select register for data port

    while(i--){
        if(dat & 0x80){
            grphSI_SetVal();                        // transmit data from MSB 
        }else {
            grphSI_ClrVal();
        }

        grphSCL_ClrVal();                               // clr RW signal for data write

        grphSCL_ClrVal();                          // turn the clock on      
        dat <<= 1;                                      // shift data left 1 place
        grphSCL_SetVal();                           // clock back off         
    }


    return;
}                           // end-fcn_grphOutDat

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphOutCtrl ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphOutCtrl

    Input:          char dat

    Return:         None

    Notes:         low level routine to send a byte value out the serial bus 
                   to the LCD controller control register. entry argument is 
                   the data to output.                                       
----------------------------------------------------------------------------------------------------*/
void grphOutCtrl (char dat)
{
    uint8 i = 8;                                          // serial bit counter

    grphRS_ClrVal();                                      // select register for command port

    while(i--){
        if(dat & 0x80){
            grphSI_SetVal();
        }else {
            grphSI_ClrVal();
        }
        grphSCL_ClrVal();                               // turn the clock on      
        dat <<= 1;                                      // shift data left 1 place
        grphSCL_SetVal();                               // clock back off         
    }

        grphSCL_SetVal();                               // force chip select back off

    return;
}                           // end-fcn_grphOutCtrl
 
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphErase ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphErase

    Input:          none

    Return:         None

    Notes:         routine to erase the LCD screen, This erases whole
                   display memory of the S6B0724 LCD controller.     
----------------------------------------------------------------------------------------------------*/
void grphErase (void)
{
uint8 p, i;

    for(p=0; p<9; p++)
    {
        grphOutCtrl(LCD_SET_PAGE+p);    // set page
        grphOutCtrl(LCD_SET_COL_HI+0);  // set column 0
        grphOutCtrl(LCD_SET_COL_LO+0);
        for(i=0; i<132; i++)
        {
            grphOutDat(0);             // clear the data
        }
    }
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphTest ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphTest

    Input:          none

    Return:         None

    Notes:         routine to display a test pattern on the LCD screen,
----------------------------------------------------------------------------------------------------*/
uint8 testpat[4][8]={
                   {0x0F,0x0F,0x0F,0x0F,0xF0,0xF0,0xF0,0xF0},
                   {0xF0,0xF0,0xF0,0xF0,0x0F,0x0F,0x0F,0x0F},
                   {0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0x81,0xFF},
                   {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00}
                  };

void grphTest (uint8 pattern)
{
    uint8 p;
    uint8 i;

    for(p=0; p<7; p++)
    {
        grphOutCtrl(LCD_SET_PAGE+p);    // set page
        grphOutCtrl(LCD_SET_COL_HI+26/16);  // set column 0
        grphOutCtrl(LCD_SET_COL_LO+26%16);
        for(i=0; i<106; i++)
        {
            grphOutDat(testpat[pattern][i%8]);
        }
    }
    return;
}                           // end-fcn_grphTest

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphClrArea ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphClrArea

    Input:          uint8 left, 
                    uint8 top,  
                    uint8 right,
                    uint8 bottom

    Return:         None

    Notes:         Clears the display memory starting at the left/top  and going to
                   the right/bottom . No runtime error checking is performed. It is
                   assumed that left is less than right and that top is less than  
                   bottom                                                          
----------------------------------------------------------------------------------------------------*/
void grphClrArea (uint8 left,
                  uint8 right,
                  uint8 top,    
                  uint8 bottom)
{
uint8        bit_pos;
uint8        x;
uint8        byte_offset;
uint8        y_bits;
uint8        remaining_bits;
uint8        mask;

uint8        t_bit_pos;
uint8        t_x;
uint8        t_byte_offset;
uint8        t_y_bits;
uint8        t_remaining_bits;
uint8        t_mask;

uint8        t_left;
uint8        t_top;
uint8        t_right;
uint8        t_bottom;

    t_left   = left;   
    t_top    = top;    
    t_right  = right;  
    t_bottom = bottom; 

    t_bit_pos        = bit_pos;       
    t_x              = x;             
    t_byte_offset    = byte_offset;   
    t_y_bits         = y_bits;        
    t_remaining_bits = remaining_bits;
    t_mask           = mask;          

    t_bit_pos = top & 0x07;                   // get starting bit offset into byte

    for(t_x = left; t_x <= right; t_x++){
        t_byte_offset = top >> 3;             // get byte offset into y direction
        t_y_bits = (bottom - top) + 1;        // get length in the y direction to write
        t_remaining_bits = 8 - t_bit_pos;       // number of bits left in byte
        t_mask = l_mask_array[t_bit_pos];       // get mask for this bit

        while(t_y_bits){                      // while there are still bits to write
            if((t_remaining_bits == 8) && (t_y_bits > 7)){
                // here if we are byte aligned and have at least 1 byte to write
                // do the entire byte at once instead of bit by bit
                while(t_y_bits > 7){          // while there are at least 8 more bits to do
                    l_display_array[t_byte_offset][t_x] = 0x00;
                    t_byte_offset++;
                    t_y_bits -= 8;
                }
            }
            else{
                // here if not byte aligned or an entire byte does not need written
                // thus do bit by bit
                l_display_array[t_byte_offset][x] &= ~t_mask;
                if(l_mask_array[0] & 0x80){
                    t_mask >>= 1;
                }
                else{
                    t_mask <<= 1;
                }
                t_y_bits--;
                t_remaining_bits--;
                if(t_remaining_bits == 0){
                    // might have bust gotton byte aligned
                    // so reset for beginning of a byte
                    t_remaining_bits = 8;
                    t_byte_offset++;
                    t_mask = l_mask_array[0];
                }
            }
        }
    }
    return;
}                           // end-fcn_grphClrArea

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphInvertArea ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphInvertArea

    Input:          uint8 left,
                    uint8 top,    
                    uint8 right,
                    uint8 bottom

    Return:         None

    Notes:         Inverts the display memory starting at the left/top and going to
                   the right/bottom. No runtime error checking is performed. It is 
                   assumed that left is less than right and that top is less than  
                   bottom                                                          
----------------------------------------------------------------------------------------------------*/
void grphInvertArea (uint8 left,
                      uint8 top,    
                      uint8 right,
                      uint8 bottom)
{
    uint8 bit_pos;
    uint8 x;
    uint8 byte_offset;
    uint8 y_bits;
    uint8 remaining_bits;
    uint8 mask;

    bit_pos = top & 0x07;                   // get starting bit offset into byte

    for(x = left; x <= right; x++)
    {
        byte_offset = top >> 3;             // get byte offset into y direction
        y_bits = (bottom - top) + 1;        // get length in the x direction to write
        remaining_bits = 8 - bit_pos;       // number of bits left in byte
        mask = l_mask_array[bit_pos];       // get mask for this bit

        while(y_bits)                       // while there are still bits to write
        {
            if((remaining_bits == 8) && (y_bits > 7))
            {
                // here if we are byte aligned and have at least 1 byte to write
                // do the entire byte at once instead of bit by bit
                while(y_bits > 7)           // while there are at least 8 more bits to do
                {
                    l_display_array[byte_offset][x] ^= 0xFF;
                    byte_offset++;
                    y_bits -= 8;
                }
            }
            else
            {
                // here if not byte aligned or an entire byte does not need written
                // thus do bit by bit
                l_display_array[byte_offset][x] ^= mask;
                if(l_mask_array[0] & 0x80)
                {
                    mask >>= 1;
                }
                else
                {
                    mask <<= 1;
                }
                y_bits--;
                remaining_bits--;
                if(remaining_bits == 0)
                {
                    // might have bust gotton byte aligned
                    // so reset for beginning of a byte
                    remaining_bits = 8;
                    byte_offset++;
                    mask = l_mask_array[0];
                }
            }
        }
    }
    return;
}                           // end-fcn_grphInvertArea

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphHorizLine ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphHorizLine

    Input:          none

    Return:         None

    Notes:          Draws a line into the display memory starting at left going to  
                    right, on the given row. No runtime error checking is performed.
                    It is assumed that left is less than right.                     
----------------------------------------------------------------------------------------------------*/
void grphHorizLine (uint8 left,
                    uint8 right,
                    uint8 row)
{
    uint8 bit_pos;
    uint8 byte_offset;
    uint8 mask;
    uint8 col;

    bit_pos = row & 0x07;           // get the bit offset into a byte
    byte_offset = row >> 3;         // get the byte offset into x array
    mask = l_mask_array[bit_pos];   // get the mask for this bit

    for(col = left; col <= right; col++)
    {
        l_display_array[byte_offset][col] |= mask;
    }
    return;
}                           // end-fcn_grphHorizLine

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphVertLine ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphVertLine

    Input:          uint8 top,  
                    uint8 bottom
                    uint8 column

    Return:         None

    Notes:          Draws a vertical line into display memory starting at the top        
                    going to the bottom in the given column. No runtime error checking   
                    is performed. It is assumed that top is less than bottom and that    
                    the column is in range.                                              
----------------------------------------------------------------------------------------------------*/
void grphVertLine (uint8 top,
                    uint8 bottom,
                    uint8 column)
{
    uint8 bit_pos;
    uint8 byte_offset;
    uint8 y_bits;
    uint8 remaining_bits;
    uint8 mask;

    bit_pos = top & 0x07;          // get starting bit offset into byte

    byte_offset = top >> 3;        // get byte offset into y direction
    y_bits = (bottom - top) + 1;   // get length in the x direction to write
    remaining_bits = 8 - bit_pos;  // number of bits left in byte
    mask = l_mask_array[bit_pos];  // get mask for this bit

    while(y_bits)                  // while there are still bits to write
    {
        if((remaining_bits == 8) && (y_bits > 7))
        {
            // here if we are byte aligned and have at least 1 byte to write
            // do the entire byte at once instead of bit by bit
            while(y_bits > 7)           // while there are at least 8 more bits to do
            {
                l_display_array[byte_offset][column] = 0xFF;
                byte_offset++;
                y_bits -= 8;
            }
        }
        else
        {
            // we are not byte aligned or an entire byte does not need written
            // do each individual bit                                         
            l_display_array[byte_offset][column] |= mask;
            if(l_mask_array[0] & 0x80)
            {
                mask >>= 1;
            }
            else
            {
                mask <<= 1;
            }
            y_bits--;
            remaining_bits--;
            if(remaining_bits == 0)
            {
                // might have bust gotton byte aligned
                // so reset for beginning of a byte
                remaining_bits = 8;
                byte_offset++;
                mask = l_mask_array[0];
            }
        }
    }
    return;
}                           // end-fcn_grphVertLine

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphClrHorizLine ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphClrHorizLine

    Input:          uint8 left,
                    uint8 right
                    uint8 row 

    Return:         None

    Notes:          Clears a line into the display memory starting at left going to 
                    right, on the given row. No runtime error checking is performed.
                    It is assumed that left is less than right.                     
----------------------------------------------------------------------------------------------------*/
void grphClrHorizLine (uint8 left,
                       uint8 right,
                       uint8 row)
{
    uint8 bit_pos;
    uint8 byte_offset;
    uint8 mask;
    uint8 col;

    bit_pos = row & 0x07;           // get the bit offset into a byte
    byte_offset = row >> 3;         // get the byte offset into x array
    mask = l_mask_array[bit_pos];   // get the mask for this bit

    for(col = left; col <= right; col++)
    {
        l_display_array[byte_offset][col] &= ~mask;
    }
    return;
}                           // end-fcn_grphClrHorizLine

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphClrVertLine ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphClrVertLine

    Input:          none

    Return:         None

    Notes:          Clears a vertical line into display memory starting at the top    
                    going to the bottom in the given column. No runtime error checking
                    is performed. It is assumed that top is less than bottom and that 
                    the column is in range.                                           
----------------------------------------------------------------------------------------------------*/
void grphClrVertLine (uint8 top,
                        uint8 bottom,
                        uint8 column)
{
    uint8 bit_pos;
    uint8 byte_offset;
    uint8 y_bits;
    uint8 remaining_bits;
    uint8 mask;

    bit_pos = top & 0x07;          // get starting bit offset into byte

    byte_offset = top >> 3;        // get byte offset into y direction
    y_bits = (bottom - top) + 1;   // get length in the x direction to write
    remaining_bits = 8 - bit_pos;  // number of bits left in byte
    mask = l_mask_array[bit_pos];  // get mask for this bit

    while(y_bits)                  // while there are still bits to write
    {
        if((remaining_bits == 8) && (y_bits > 7))
        {
            // here if we are byte aligned and have at least 1 byte to write
            // do the entire byte at once instead of bit by bit
            while(y_bits > 7)           // while there are at least 8 more bits to do
            {
                l_display_array[byte_offset][column] = 0x00;
                byte_offset++;
                y_bits -= 8;
            }
        }
        else
        {
            // we are not byte aligned or an entire byte does not need written
            // do each individual bit                                         
            l_display_array[byte_offset][column] &= ~mask;
            if(l_mask_array[0] & 0x80)
            {
                mask >>= 1;
            }
            else
            {
                mask <<= 1;
            }
            y_bits--;
            remaining_bits--;
            if(remaining_bits == 0)
            {
                // might have bust gotton byte aligned
                // so reset for beginning of a byte
                remaining_bits = 8;
                byte_offset++;
                mask = l_mask_array[0];
            }
        }
    }
    return;
}                           // end-fcn_grphClrVertLine

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphBox ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphBox

    Input:          uint8 left, 
                    uint8 top,  
                    uint8 right,
                    uint8 bottom

    Return:         None

    Notes:          Draws a box in display memory starting at the left/top and going 
                    to the right/bottom. No runtime error checking is performed.     
                    It is assumed that left is less than right and that top is less  
                    than bottom.                                                     
----------------------------------------------------------------------------------------------------*/
void grphBox (uint8 left,
              uint8 top,
              uint8 right,
              uint8 bottom)
{
                                                        // to draw a box requires two vertical lines
    grphVertLine(top,bottom,left);
    grphVertLine(top,bottom,right);

                                                        // and two horizonal lines
    grphHorizLine(left,right,top);
    grphHorizLine(left,right,bottom);

    return;
}                           // end-fcn_grphBox

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphClrBox ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphClrBox

    Input:          uint8 left, 
                    uint8 top,  
                    uint8 right,
                    uint8 bottom

    Return:         None

    Notes:          Clears a box in display memory starting at the Top left and going 
                    to the bottom right. No runtime error checking is performed and   
                    it is assumed that Left is less than Right and that Top is less   
                    than Bottom.                                                      
----------------------------------------------------------------------------------------------------*/
void grphClrBox (uint8 left,
                  uint8 top,
                  uint8 right,
                  uint8 bottom)
{
                                                        // to undraw the box undraw the two vertical lines
    grphClrVertLine(top,bottom,left);
    grphClrVertLine(top,bottom,right);

                                                        // and the two horizonal lines that comprise it
    grphClrHorizLine(left,right,top);
    grphClrHorizLine(left,right,bottom);

    return;
}                           // end-fcn_grphClrBox

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphText ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphText

    Input:          uint8 left
                    uint8 top,
                    int8 font
                    uint8 *str        

    Return:         None

    Notes:          Prints the given string at location x,y in the specified font.       
                    Prints each character given via calls to grphGlyph. The entry string 
                    is null terminated and non 0x20->0x7e characters are ignored.        
                                                                                         
                    Arguments are:                                                       
                        left       coordinate of left start of string.                   
                        top        coordinate of top of string.                          
                        font       font number to use for display                        
                        str        text string to display                                
----------------------------------------------------------------------------------------------------*/
void grphText (uint8 left,
               uint8 top,
               int8  font,
               uint8 *str)
{
    uint8 x = left;
    uint8  glyph;
    uint8  width;
    uint8  height;
    uint8  store_width;
    uint8  *glyph_ptr;

    while(*str != 0x00)
    {
        glyph = (uint8)*str;

                                                        // check to make sure the symbol is a legal one
                                                        // if not then just replace it with the default character
        if((glyph < fonts[font].glyph_beg) || (glyph > fonts[font].glyph_end))
        {
            glyph = fonts[font].glyph_def;
        }

                                                        // make zero based index into the font data arrays
        glyph -= fonts[font].glyph_beg;
        width = fonts[font].fixed_width;                // check if it is a fixed width
        if(width == 0)
        {
            width=fonts[font].width_table[glyph];       // get the variable width instead
        }

        height = fonts[font].glyph_height;
        store_width = fonts[font].store_width;

        glyph_ptr = (uint8 *)(fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height));

                                                        // range check / limit things here
        if(x > SCRN_RIGHT)
        {
            x = SCRN_RIGHT;
        }
        if((x + width) > SCRN_RIGHT+1)
        {
            width = SCRN_RIGHT - x + 1;
        }
        if(top > SCRN_BOTTOM)
        {
            top = SCRN_BOTTOM;
        }
        if((top + height) > SCRN_BOTTOM+1)
        {
            height = SCRN_BOTTOM - top + 1;
        }

        grphGlyph
            (x,
             top,
             width,
             height,
             glyph_ptr,
             store_width);  // plug symbol into buffer

        x += width;                                     // move right for next character
        str++;                                          // point to next character in string
    }
    return;
}                           // end-fcn_grphText

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphGlyph ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphGlyph

    Input:          uint8 left,      
                    uint8 top,       
                    uint8 width,     
                    uint8 height,    
                    uint8 *glyph,    
                    uint8 store_width

    Return:         None

    Notes:          Writes a glyph to the display at location x,y               
                                                                                
                    Arguments are:                                              
                       column     - x corrdinate of the left part of glyph      
                       row        - y coordinate of the top part of glyph       
                       width      - size in pixels of the width of the glyph    
                       height     - size in pixels of the height of the glyph   
                       glyph      - an uint8 pointer to the glyph pixels
                                    to write assumed to be of length "width"    
----------------------------------------------------------------------------------------------------*/
void grphGlyph  (uint8 left,
                 uint8 top,
                 uint8 width,
                 uint8 height,
                 uint8 *glyph,
                 uint8 store_width)
{
    uint8 bit_pos;
    uint8 byte_offset;
    uint8 y_bits;
    uint8 remaining_bits;
    uint8 mask;
    uint8 char_mask;
    uint8 x;
    uint8 *glyph_scan;
    uint8 glyph_offset;

    bit_pos = top & 0x07;                               // get the bit offset into a byte

    glyph_offset = 0;                                   // start at left side of the glyph rasters
    char_mask = 0x80;                                   // initial character glyph mask

    for (x = left; x < (left + width); x++)
    {
        byte_offset = top >> 3;                         // get the byte offset into y direction
        y_bits = height;                                // get length in y direction to write
        remaining_bits = 8 - bit_pos;                   // number of bits left in byte
        mask = l_mask_array[bit_pos];                   // get mask for this bit
        glyph_scan = glyph + glyph_offset;              // point to base of the glyph

                                                        // boundary checking here to account for the possibility of 
                                                        // write past the bottom of the screen.                       
        while((y_bits) && (byte_offset < Y_BYTES))      // while there are bits still to write
        {
                                                        // check if the character pixel is set or not
            if(*glyph_scan & char_mask)
            {
                l_display_array[byte_offset][x] |= mask; // set image pixel
            }
            else
            {
                l_display_array[byte_offset][x] &= ~mask;// clear the image pixel
            }

            if(l_mask_array[0] & 0x80)
            {
                mask >>= 1;
            }
            else
            {
                mask <<= 1;
            }
            
            y_bits--;
            remaining_bits--;
            if(remaining_bits == 0)
            {
                                                        // just crossed over a byte boundry, reset byte counts
                remaining_bits = 8;
                byte_offset++;
                mask = l_mask_array[0];
            }

                                                        // bump the glyph scan to next raster
            glyph_scan += store_width;
        }

                                                        // shift over to next glyph bit
        char_mask >>= 1;
        if(char_mask == 0)                              // reset for next byte in raster
        {
            char_mask = 0x80;
            glyph_offset++;
        }
    }
    return;
}                           // end-fcn_grphGlyph

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphUpdate ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphUpdate

    Input:          uint8 top,  
                    uint8 bottom

    Return:         None

    Notes:          Updates area of the display. Writes data from display
                    RAM to the lcd display controller.                   
                                                                         
                    Arguments Used:                                      
                       top     top line of area to update.               
                       bottom  bottom line of area to update.            

----------------------------------------------------------------------------------------------------*/
void grphUpdate (uint8 top,
                 uint8 bottom)
{
uint8 x,
      y,
      yt,
      yb,
      col_hi,
      col_low,
      *colptr;

    yt = top >> 3;              // setup bytes of range
    yb = bottom >> 3;

    for(y = yt; y <= yb; y++)
    {
        // setup the page number for the y direction
        grphOutCtrl(LCD_SET_PAGE + y);    // set page
    
        // setup column of update to left side
        col_hi = LCD_SET_COL_HI + (4/16);
        grphOutCtrl(col_hi); // set column 4

        col_low = LCD_SET_COL_LO + (4%16);
        grphOutCtrl(col_low);

        colptr = &l_display_array[y][0];
        for (x=0; x < X_BYTES; x++)
        {
            grphOutDat(*colptr++);
        }
    }
    return;
}                           // end-fcn_grphUpdate

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphOutline ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       erases display, then draws double box

    Input:          none

    Return:         None
    
    Notes:          //rwl, box currently hard coded, should be changed to calc based on grphMARGIN_RIGHT, ...LEFT etc

----------------------------------------------------------------------------------------------------*/
void grphOutline (void)
{
    grphClrArea
        (SCRN_LEFT,
         SCRN_RIGHT,
         SCRN_TOP,
         SCRN_BOTTOM);

    grphBox(SCRN_LEFT,SCRN_TOP,SCRN_RIGHT,SCRN_BOTTOM);
    grphBox(SCRN_LEFT+2,SCRN_TOP+2,SCRN_RIGHT-2,SCRN_BOTTOM-2);

    grphUpdate(SCRN_TOP,SCRN_BOTTOM);

    return;
}                           //end-fcn_grphOutline


