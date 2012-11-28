/*============================================================================================================
                                $RCSfile: CnvMiscFcns.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.3 $
       $Date: 2007/06/20 03:11:52 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/CnvMiscFcns.c,v $

=============================================================================================================*/
#include "TypeDef.h"

#include <string.h>
#include "snip_str.h"

#include <stdio.h>

#include <errno.h>

void itoa(char *buf, unsigned int i, int base);

#define HUNDRED_DEC                                     0x64 // 100 decimal
#define TEN_DEC                                         0x0A // 10 decimal

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( sstrcpy )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       copies string allows overlapping buffers
    
    Input:          char *to, char *from

    Return:         ptr to start of string
    
    Notes:          from CodeSnippets
                        memmove(to, from, 1+strlen(from));
----------------------------------------------------------------------------------------------------*/
char *sstrcpy (char *to, char *from)
{
    memmove(to, from, strlen(from));
    return to;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( sstrcat )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Concatenates string allows overlapping buffers
    
    Input:          char *to, char *from

    Return:         ptr to start of string
    
    Notes:          from CodeSnippets
----------------------------------------------------------------------------------------------------*/
char *sstrcat (char *to, char *from)
{
    sstrcpy(to + strlen(to), from);
    return to;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( Char2AsciiDigits )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Converts an Char to three Ascii Digits
    
    Input:          char, pointer to destination string of conversion

    Return:         none
    
    Notes:          Leading zeros are filled, 3 ascii chars are always converted,
                      input range is 0 - 0xFF, 0 - 255.
                      string is NOT NULL terminated
                      destination array is assumed to be 3 chars
----------------------------------------------------------------------------------------------------*/
void Char2AsciiDigits (uint8  char_2_cnv,
                       sint8 *destptr,
                       sint8  cnv_size)
{
sint16 digit3, digit2, digit1, tmp;

    if(char_2_cnv > 0x63)                               // if greater then 99 then 3 digits
    {
       digit3 = char_2_cnv / HUNDRED_DEC;               // Hundreds
       tmp    = char_2_cnv - (digit3 * HUNDRED_DEC);
       digit2 = tmp / TEN_DEC;                          // tens
       digit1 = tmp - (digit2 * TEN_DEC);               // ones
    }else {

    if(char_2_cnv > 0x09)                               // greater then 9 then two digits
    {   
       digit3 = 0;
       digit2 = char_2_cnv / TEN_DEC;                   // tens
       digit1     = char_2_cnv - (digit2 * TEN_DEC);
    }else {
    
    digit3 = 0;
    digit2 = 0;
    digit1 = char_2_cnv;                                // just convert to ascii
    }}
    
    if(cnv_size > 0x02)                                 // even if size wrong only 3 digits moved
    {
       *destptr++ = (sint8)digit3 + '0';
       *destptr++ = (sint8)digit2 + '0';
       *destptr   = (sint8)digit1 + '0';
    }else {
    
    if(cnv_size == 2)
    {
       *destptr++ = (sint8)digit2 + '0';
       *destptr   = (sint8)digit1 + '0';
    }else {
    
    if(cnv_size == 1)                                   // if size 0 do nothing
    {
       *destptr   = (sint8)digit1 + '0';
    }}}
    
    return;                                             // end-Char2AsciiDigits 
}    

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( BCDDouble2Hex )))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Converts from BCD to hex
    
    Input:          BCD char

    Return:         Hex char
    
    Notes:          
----------------------------------------------------------------------------------------------------*/
uint8 BCDDouble2Hex (uint8 bcd_num)
{
uint8 hex_num;

    hex_num = ((bcd_num >> 4) * 10) + (bcd_num & 0x0F);

    return(hex_num);
}                                                       //end-fcn_BCDSingle2Hex

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( Hex2BCDDouble )))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Converts from hex to BCD
    
    Input:          Hex char

    Return:         BCD char
    
    Notes:
----------------------------------------------------------------------------------------------------*/
uint8 Hex2BCDDouble (uint8 hex_num)
{
uint8 bcd_num, i;

    i = 0;
    while(hex_num >= 0x0A) {
        hex_num = hex_num - 0x0A;
        i++;
    }
    bcd_num = (i * 0x10) + hex_num;

    return(bcd_num);
}                                                       //end-fcn_Hex2BCDDouble

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( itoa )))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Converts from int to ascii string
    
    Input:          integer

    Return:         char *
    
    Notes:
----------------------------------------------------------------------------------------------------*/

void itoa(char *buf, unsigned int i, int base)
{
char *s;
#define LEN 10
int rem;
char rev[LEN+1];

    if (i == 0)
        s = "0";
    else
        {
        rev[LEN] = 0;
        s = &rev[LEN];
        while (i)
            {
            rem = i % base;
            if (rem < 10)
                *--s = rem + '0';
            else if (base == 16)
                *--s = "abcdef"[rem - 10];
            i /= base;
            }
        }
    strcpy(buf, s);
}
