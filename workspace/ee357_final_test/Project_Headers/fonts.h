/*============================================================================================================
                                $RCSfile: fonts.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.3 $
       $Date: 2007/05/17 20:19:32 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/fonts.h,v $

=============================================================================================================*/
#ifndef _FONTS_H
#define _FONTS_H
#include "support_common.h"
#include "lcd128x64.h"
/* select desired fonts. (Simply comment out those not needed) */
//#define EN_FONT_FIVE_DOT                // 0
//#define EN_FONT_SIX_DOT                 // 1
#define EN_FONT_SEVEN_DOT               // 2
#define EN_FONT_NINE_DOT                // 3 
//#define EN_FONT_TEN_DOT                 // 4
//#define EN_FONT_FIFTEEN_DOT             // 5
//#define EN_FONT_EIGHTEEN_DOT            // 6

#define EN_MONO8_DOT

enum
{
    ge_NULL = 0,
    ge_BASE_ASCII,
    ge_BASE_BINARY
};

/* define number labels for the font selections */
typedef enum
{
#ifdef EN_FONT_FIVE_DOT
    FONT_FIVE_DOT,
#endif

#ifdef EN_FONT_SIX_DOT
    FONT_SIX_DOT,
#endif

#ifdef EN_FONT_SEVEN_DOT
    FONT_SEVEN_DOT,
#endif

#ifdef EN_FONT_NINE_DOT
    FONT_NINE_DOT,
#endif

#ifdef EN_FONT_TEN_DOT
    FONT_TEN_DOT,
#endif

#ifdef EN_FONT_FIFTEEN_DOT
    FONT_FIFTEEN_DOT,
#endif

#ifdef EN_FONT_EIGHTEEN_DOT
    FONT_EIGHTEEN_DOT,
#endif

#ifdef EN_FONT_MONO8_DOT
    FONT_MONO8_DOT,
#endif

    FONT_COUNT
} FONT_BASE;

typedef struct 
{
   const uint8 store_width;               /* glyph storage width in bytes */
   const uint8 glyph_height;              /* glyph height for storage */
   const uint8 *glyph_table;              /* font table start address in memory */
   const uint8 fixed_width;               /* fixed width of glyphs. If zero */
                                          /* then use the width table. */
   const uint8 *width_table;              /* variable width table start adress */
   const uint8 glyph_beg;                 /* start ascii offset in table */
   const uint8 glyph_end;                 /* end ascii offset in table */
   const uint8 glyph_def;                 /* code for undefined glyph code */
}FONT_DEF;

/* font definition tables for the fonts */
extern const FONT_DEF fonts[FONT_COUNT];

/* glyph bitmap and width tables for the fonts */ 
#ifdef EN_FONT_FIVE_DOT
  extern const uint8 five_dot_glyph_table[];
  extern const uint8 five_dot_width_table[];
#endif

#ifdef EN_FONT_SIX_DOT
  extern const uint8 six_dot_glyph_table[];
  extern const uint8 six_dot_width_table[];
#endif

#ifdef EN_FONT_SEVEN_DOT
  extern const uint8 seven_dot_glyph_table[];
  extern const uint8 seven_dot_width_table[];
#endif

#ifdef EN_FONT_NINE_DOT
  extern const uint8 nine_dot_glyph_table[];
#endif

#ifdef EN_FONT_TEN_DOT
  extern const uint8 ten_dot_glyph_table[];
#endif

#ifdef EN_FONT_FIFTEEN_DOT
  extern const uint8 fifteen_dot_glyph_table[];
  extern const uint8 fifteen_dot_width_table[];
#endif

#ifdef EN_FONT_EIGHTEEN_DOT
  extern const uint8 eighteen_dot_glyph_table[];
  extern const uint8 eighteen_dot_width_table[];
#endif

#ifdef EN_FONT_MONO8_DOT
  extern const uint8 mono8_dot_glyph_table[];                // fixed width
#endif

#endif                      //end-def__FONTS_H

