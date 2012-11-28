/*
 * lcd128x64.h
 *
 *  Created on: Mar 28, 2011
 *      Author: Mark
 */

#ifndef LCD128X64_H_
#define LCD128X64_H_
#include "support_common.h"
#include "usc_support.h"

#define ge_T_10_MS 10000
#define ge_T_50_MS 50000
#define ge_T_100_MS 100000

#define FALSE 0
#define TRUE 1
                                                        
// command function equates for S6B0724 LCD Display Controller
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

#define LCD_BACKGROUND      0x04    // empirically determined
#define LCD_CONTRAST        0x1E    // empirically determined

// LCD screen and bitmap image array consants
#define X_BYTES             128
#define Y_BYTES             8
#define SCRN_LEFT           0
#define SCRN_TOP            0
#define SCRN_RIGHT          127
#define SCRN_BOTTOM         63

#define FIVE_DOT_SIZE                                        25  // estimated number of chars per line, temp until calc can be done
#define SIX_DOT_SIZE                                         25
#define SEVEN_DOT_SIZE                                       17
#define NINE_DOT_SIZE                                        15
#define TEN_DOT_SIZE                                         13
#define FIFTEEN_DOT_SIZE                                     11
#define EIGHTEEN_DOT_SIZE                                     7



extern const uint8 l_mask_array[8];
extern uint8 l_display_array[Y_BYTES][X_BYTES];

void grphSCL_ClrVal(); 
void grphSCL_SetVal(); 
void grphRS_ClrVal(); 
void  grphRS_SetVal(); 
void grphSI_ClrVal(); 
void grphSI_SetVal(); 
void grphRESET_ClrVal(); 
void grphRESET_SetVal(); 
//void lcdBKLITE_ClrVal(); 
//void lcdBKLITE_SetVal();
void TimeDelay(uint32 t);


//============================================================================================================

void	init_lcd();

void    grphContrast            (uint8 grph_contrast);
void    grphBackground          (uint8 grph_background);
void    grphOutline             (void);


void    grphInit                (void);
void    grphOutDat              (char dat);
void    grphOutCtrl             (char dat);
void    grphFirstCharInit       (uint8 dat);
void    grphErase               (void);
void    grphTest                (uint8 pattern);
//void    grphClrArea             (uint8 left,
//                                 uint8 top,    
//                                 uint8 right,
//                                 uint8 bottom);

void    grphInvertArea          (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphHorizLine           (uint8 left,
                                 uint8 right,
                                 uint8 row);

void    grphVertLine            (uint8 top,
                                 uint8 bottom,
                                 uint8 column);

void    grphClrHorizLine        (uint8 left,
                                 uint8 right,
                                 uint8 row);

void    grphClrVertLine         (uint8 top,
                                 uint8 bottom,
                                 uint8 column);

void    grphBox                 (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphClrBox              (uint8 left,
                                 uint8 top,
                                 uint8 right,
                                 uint8 bottom);

void    grphGlyph               (uint8 left,
                                 uint8 top,
                                 uint8 width,
                                 uint8 height,
                                 uint8 *glyph,
                                 uint8 store_width);

void    grphText                (uint8 left,
                                 uint8 top,
                                 int8  font,
                                 uint8 *str);

void    grphUpdate              (uint8 top,
                                 uint8 bottom);

#endif /* LCD128X64_H_ */
