/*============================================================================================================
                                $RCSfile: lcd128x64.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.4 $
       $Date: 2007/06/05 20:22:39 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/lcd128x64.h,v $

=============================================================================================================*/
#ifndef _LCD128x64_H
    #define _LCD128x64_H

/*// command function equates for S6B0724 LCD Display Controller
#define LCD_DISP_OFF        0xAE    // turn LCD panel OFF
#define LCD_DISP_ON         0xAF    // turn LCD panel ON
#define LCD_SET_LINE        0x40    // set line for COM0 (6 lsbs = ST5:ST4:ST3:ST2:ST1:ST0)
#define LCD_SET_PAGE        0xB0    // set page address (4 lsbs = P3:P2:P1:P0)
#define LCD_SET_COL_HI      0x10    // set column address (4 msbs = Y7:Y6:Y5:Y4)
#define LCD_SET_COL_LO      0x00    // set column address (4 lsbs = Y3:Y2:Y1:Y0)
#define LCD_SET_ADC_NOR     0xA0    // ADC set for normal direction
#define LCD_SET_ADC_REV     0xA1    // ADC set for reverse direction
#define LCD_DISP_NOR        0xA6    // normal pixel display
#define LCD_DISP_REV        0xA7    // reverse pixel display
#define LCD_EON_OFF         0xA4    // normal display mode
#define LCD_EON_ON          0xA5    // entire dsplay on
#define LCD_SET_BIAS_0      0xA2    // set LCD bias select to 0
#define LCD_SET_BIAS_1      0xA3    // set LCD boas select to 1
#define LCD_SET_MODIFY      0xE0    // set modfy read mode
#define LCD_CLR_MODIFY      0xEE    // clear modify read mode
#define LCD_RESET           0xE2    // soft reset command
#define LCD_SET_SHL_NOR     0xC0    // set COM direction normal
#define LCD_SET_SHL_REV     0xC8    // set COM direction reverse
#define LCD_PWR_CTL         0x28    // set power control (3 lsbs = VC:VR:VF)
#define LCD_REG_RESISTOR    0x20    // set regulator resistor (3 lsbs = R2:R1:R0)
#define LCD_REF_VOLT_MODE   0x81    // set reference voltage mode (first of dual command)
#define LCD_REF_VOLT_REG    0x00    // set reference voltage register (6 lsbs = SV5:SV4:SV3:SV2:SV1:SV0)
#define LCD_ST_IND_MODE_0   0xAC    // set static indicator mode to 0
#define LCD_ST_IND_MODE_1   0xAD    // set static indicator mode to 1
#define LCD_ST_IND_REG      0x00    // set the static indicator register (2 lsbs = S1:S0)
#define LCD_NOP             0xE3    // LCD controller NOP command
#define LCD_TEST_CMD_1      0xF0    // LCD Test Instruction 1.. do not use
#define LCD_TEST_CMD_2      0x90    // LCD Test Instruction 2.. do not use

// LCD screen and bitmap image array consants
#define X_BYTES             128
#define Y_BYTES             8
#define SCRN_LEFT           0
#define SCRN_TOP            0
#define SCRN_RIGHT          127
#define SCRN_BOTTOM         63
*/
// LCD Global data arrays
extern const uint8 l_mask_array[8];
extern uint8 l_display_array[Y_BYTES][X_BYTES];

#endif
