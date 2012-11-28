#include "RPS_BldOptions.h"

#include "TypeDef.h"
#include "lcdCntrlDef.h"

#include "EnumDef.h"
#include "lcdConstDef.h"

#include "grphSCL.h"
#include "grphRS.h"
#include "grphSI.h"
#include "lcdBKLITE.h"

#include "snip_str.h"

#include "lcd128x64.h"

#include "fonts.h"
#include "EnumDef.h"
#include "grphEnumDef.h"

#include "lcdStructDef.h"

#include "SwitchStructDef.h"
#include "ExtSwitchDeclar.h"

#include "ExtGlobalVarDeclar.h"

#include "FcnDef.h"
#include "lcdFcnDef.h"

#ifdef RPS_LCDDISPFCNS_OPT_LIB
    #include "lcdDispFcns_opt.c"
    #include "IncrDispFcns_opt.c"
#endif

uint8                           grphCursorStatus,
                                grphLeftCursor,
                                grphRightCursor,
                                grphTopCursor,
                                grphBottomCursor;


/*============================================================================================================
                                $RCSfile: lcdDispFcns.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.23 $
       $Date: 2007/06/20 03:11:52 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/lcdDispFcns.c,v $

=============================================================================================================*/
//===========================================================================================================
uint8                           grphDispRam[grphMAX_LINES_IN_WINDOW][grphMAX_CHARS_PER_LINE_IN_WINDOW + 1];

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( DispLineSet ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:  DispLineSet

    Input:     pointer to array to display

    Return:     none
    
    Notes:
----------------------------------------------------------------------------------------------------*/
void vFn_DispLineSet (uint8    *mssg_set_ptr,
                      uint8     line_length,
                      uint8     font_size,
                      uint8     clear_screen)
{

uint8 i;

    if(clear_screen == ge_CLEAR_SCREEN){
       vFn_grphClrView(font_size);                      // clean draw outline
    }
 
    for(i = 0; i < grphMAX_LINES_IN_WINDOW; i++){
        DispStringNoCursor
            (i,
             START_OF_LINE,        
             mssg_set_ptr,
             ge_CLEAR_LINE,           
             font_size);

        mssg_set_ptr = mssg_set_ptr + line_length;
    }

    return;
}
      
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( lcdBKLITE ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:  cogBkLite

    Input:     status to set back lite to

    Return:     none
    
    Notes:
----------------------------------------------------------------------------------------------------*/
void vFn_lcdBkLite (sint16 light_status)
{
        if(light_status == ge_LED_ON){
            lcdBKLITE_SetVal();
        }else {
        if(light_status == ge_LED_OFF){
            lcdBKLITE_ClrVal();
        }}

    return;
}                           // end-fcn_cogBkLite

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( u8Fn_grphFontRemap ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:  u8Fn_grphFontRemap

    Input:     font selection

    Return:    remapped index
    
    Notes:      a separate enum must used apart from enum used in fonts.h,c because of indexing in font.c is linear
                the font sizes in the input string are decimal and correspond directly with font size. That is a font size of
                10 is decimal ten in the file but index 4 in font table;
                therefore for the present these are defined as a different enum set and do not directly match
                with the enum set used by the font.c,h
--------------------------------------------------------------------------------------------------*/
uint8 u8Fn_grphFontRemap (uint8 font_selection)
{
int8 font_remapped;

    switch(font_selection){
#ifdef EN_FONT_FIVE_DOT
        case FONT_FIVE_DOT_MESS:
            font_remapped = FONT_FIVE_DOT;
            u8Fn_grphSetFont(FONT_FIVE_DOT);                 // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_SIX_DOT
        case FONT_SIX_DOT_MESS:
            font_remapped = FONT_SIX_DOT;
            u8Fn_grphSetFont(FONT_SIX_DOT);                  // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_SEVEN_DOT
        case FONT_SEVEN_DOT_MESS:
            font_remapped = FONT_SEVEN_DOT;
            u8Fn_grphSetFont(FONT_SEVEN_DOT);                // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_NINE_DOT
        case FONT_NINE_DOT_MESS:
            font_remapped = FONT_NINE_DOT;
            u8Fn_grphSetFont(FONT_NINE_DOT);                 // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_TEN_DOT
        case FONT_TEN_DOT_MESS:
            font_remapped = FONT_TEN_DOT;
            u8Fn_grphSetFont(FONT_TEN_DOT);                  // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_FIFTEEN_DOT
        case FONT_FIFTEEN_DOT_MESS:
            font_remapped = FONT_FIFTEEN_DOT;
            u8Fn_grphSetFont(FONT_FIFTEEN_DOT);              // must map between grph font list and message, to be changed
        break;
#endif

#ifdef EN_FONT_EIGHTEEN_DOT
        case FONT_EIGHTEEN_DOT_MESS:
            font_remapped = FONT_EIGHTEEN_DOT;
            u8Fn_grphSetFont(FONT_EIGHTEEN_DOT);             // must map between grph font list and message, to be changed
        break;
#endif

        default:
            font_remapped = FONT_NINE_DOT;
            u8Fn_grphSetFont(FONT_NINE_DOT);              // must map between grph font list and message, to be changed
        break;
    }

    return(font_remapped);
}                           // end-fcn_u8Fn_grphFontRemap

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( u8Fn_grphSetFont )))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

    Function:           Sets gobal variables for max chars per line, max lines per screen,

    Input:              uint8  font_size
    Return:             none
    Notes:              calculations based on EnumDef.h for current physical grph layout
                            number of chars per line is stored in a table for each font
--------------------------------------------------------------------------------------------------*/
uint8 u8Fn_grphSetFont (uint8 font_size)
{
uint8 width, grph_max_chars_per_line;
uint8 max_chars_per_line[FONT_COUNT] = {FIVE_DOT_SIZE, SIX_DOT_SIZE,
                                        SEVEN_DOT_SIZE, NINE_DOT_SIZE,
                                        TEN_DOT_SIZE, FIFTEEN_DOT_SIZE,
                                        EIGHTEEN_DOT_SIZE};

    width = fonts[font_size].fixed_width;               // check if it is a fixed width

    if(width == 0){
        grph_max_chars_per_line = max_chars_per_line[font_size];    // proportional space assume default width from EnumDef.h
    }else {
        grph_max_chars_per_line = ((SCRN_RIGHT + 1) - ((uint8)grphMARGIN_LEFT + grphMARGIN_RIGHT))/width; // fixed width start within the box outline
    }                                                   // add one to SCRN_RIGHT because we are counting columns not indexs

    return(grph_max_chars_per_line);    
}                           // end-grpSetFont

/*------------------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((( DispBuffLine ))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
--------------------------------------------------------------------------------------------------------------

   Write Buffer
    Function:           Write a string of bytes to the HD44780
    Input Parameters:   uint8 lcd_line_num,
                        uint8 lcd_char_addr,
                        uint8 *buffer
    Return Type:        None

    Notes:              grph_left, and grph_top are not defined the same for grphText and grphUpdate, grphClrArea
                        grphText,   (x, y) == (grph_left, grph_top)
                        grphUpdate  (x, y), (x,y) == (left, top), (right, bottom)
                        grphText --> left is char position, top is line position, the entire line must be displayed;
                            because the char position is dependant on the number of bits in the font and the
                            spacing between chars... the margins top,bottom and font_size are added for box's
                            or other effects that might occur before the text is written; the defines are in EnumDef.h
                            height is the rows per glyph, set by FontGen, see fonts.c
                            grphROW_SPACES is the blank rows under the glyph
                            grphMARGIN_LEFT is the offset to the left of the glyph
                            grphMARGIN_TOP is the offset from the first row to the glyph
                        grphText (x, y)
                            x = (grphFONT_SIZE * lcd_char_addr) + grphMARGIN_LEFT //   only    for fixed width fonts
                            y = (lcd_line_num * (grphFONT_SIZE + grphROW_SPACES)) + grphMARGIN_TOP
                        grphUpdate (top,bottom)    two corners of a rectangle (x, Y)
                            top = x = (grphFONT_SIZE * lcd_char_addr) + grphMARGIN_LEFT
                            bottom = ((lcd_line_num * (grphFONT_SIZE + grphROW_SPACES)) + grphMARGIN_TOP);

                        grph_left = (lcd_char_addr * height) + grphMARGIN_LEFT;  // only for fixed width fonts, for now always 0 by calling fcn

                        ***rwl, need to account for reads from file not being null terminated, for now termination is here
                            but should be in calling rtn; caller should block the data first
--------------------------------------------------------------------------------------------------*/
void DispBuffLine (uint8 lcd_line_num,
                   uint8 lcd_char_addr,
                   uint8 *buffer,
                   uint8 font_size)
{
#ifndef DISP_LCD_BYPASS
uint8 grph_left, grph_top, grph_update_top, grph_update_bottom, height, i, j,
             accum_length, width, grph_max_chars_per_line;
uint16 string_length, buffer_length;

    string_length = strlen((char *)buffer);

    width = fonts[font_size].fixed_width;               // check if it is a fixed width
    grph_max_chars_per_line = u8Fn_grphSetFont(font_size);  // get num of chars in line

    if(string_length > grph_max_chars_per_line){
        string_length = grph_max_chars_per_line;          // truncate or variable areas will be overwritten
    }

    accum_length = strlen((char *)buffer) + lcd_char_addr;      // lcd_char_addr pointer for 1 char of string
    if(accum_length > grph_max_chars_per_line){
        buffer_length = grph_max_chars_per_line - lcd_char_addr;
        accum_length = grph_max_chars_per_line;             // limit to max width of screen, used for NULL termination
    }else {
        buffer_length = strlen((char *)buffer);                 // null terminator is not counted
    }

    for(i = 0; i < (grph_max_chars_per_line); i++){         // clear out nulls if any
        if(grphDispRam[lcd_line_num][i] == ge_NULL){  // make sure null doesn't occur internal to string, or disp will fail
            grphDispRam[lcd_line_num][i] = ' ';
        }
    }

    j = lcd_char_addr;
    for(i = 0; i < string_length; i++){               // copy length of string, into start address indexed into string
        grphDispRam[lcd_line_num][j++] = *buffer++;     // copy string unterminated into row column grphDispRam buffer 
    }

    grphDispRam[lcd_line_num][grph_max_chars_per_line] = ge_NULL;// null term string at end of string that accounts for char_addr
//    grphDispRam[lcd_line_num][accum_length] = ge_NULL;// null term string at end of string that accounts for char_addr

    height = fonts[font_size].glyph_height;

    grph_left = (uint8)grphMARGIN_LEFT;                        // only for fixed width fonts, for now  lcd_char_addr always 0 by calling fcn
    grph_top  = (lcd_line_num * (height + (uint8)grphROW_SPACES)) + (uint8)grphMARGIN_TOP;

    grphText
        (grph_left,
         grph_top,
         font_size,
         &grphDispRam[lcd_line_num][0]);

    grph_update_top    = (uint8)grphMARGIN_TOP + (lcd_line_num * (height + (uint8)grphROW_SPACES));
    grph_update_bottom = grph_update_top + height;      // find lower rt corner, which is just font size away
    grphUpdate
        (grph_update_top,                               // just reshow the used area to reduce I/O
         grph_update_bottom);
#endif
    return;
}                                                       //end-fcn_DispBuffLine

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DispString ))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

    Function:           clear the line first
                        Display prompt at current uint8 address, at line passed
                        home cursor, set for no increment
    Input Parameter:    uint8 line_number,
                        uint8 address,
                        pointer to string buffer,
                        uint8 clear line y/n
                        cursor cntrl (ON or OFF),
                        cursor_line_num,
                        cursor addr
    Return Type:    None
    
   Notes: allows string postion based on line, char address, and cursor control, uses only one line for display
    
--------------------------------------------------------------------------------------------------*/
void DispString (uint8 lcd_line_num,
                 uint8 lcd_char_addr,
                 uint8 *buffer,
                 uint8 clear_line,
                 uint8 cursor_status,
                 uint8 cursor_line_num,
                 uint8 cursor_addr,
                 uint8 font_size)
{
#ifndef DISP_LCD_BYPASS                                 // see RPS_BldOptions.h
int16 string_length;

    string_length = strlen((char *)buffer);
    if(clear_line == ge_YES){
        grphClrLine
            (lcd_line_num,
             font_size);                                // clear line first
    }                                                   //end-if

    DispBuffLine
        (lcd_line_num,                                  // DispBuffLine fills in string into grphDispRam
         lcd_char_addr,                                 //      then displays that line from grphDispRam based on Line Number
         buffer,
         font_size);                                    // display the char
    
    if(cursor_status == grphCURSOR_ON){
        DispCursor
            (cursor_line_num,
             cursor_addr,
             cursor_status,
             font_size,
             (uint8)string_length);                            // set cursor
    }
#endif
    return;
}                           //end-fcn_DispString

/*------------------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((( DispStringNoCursor ))))))))))))))))))))))))))))))))))))))))))))))))))
--------------------------------------------------------------------------------------------------------------
    Function:           if required by calling fcn, clear the line   
                           Display prompt at current char address, at line passed
    Input Parameter:    unsigned lcd_line_number,
                        uint8 address,
                        pointer to string buffer,
                        uint8 clear line y/n
    Return Type:        None
    
   Notes: allows string postion based on line, and cursor control, uses only one line for display
    
--------------------------------------------------------------------------------------------------*/
void DispStringNoCursor (uint8 lcd_line_num,
                         uint8 lcd_char_addr,
                         uint8 *buffer,
                         uint8 clear_line,
                         uint8 font_size)
{
#ifndef DISP_LCD_BYPASS                                 // see RPS_BldOptions.h

    DispString
        (lcd_line_num,
         lcd_char_addr,
         buffer,
         clear_line,
         grphCURSOR_OFF,
         ge_NULL,
         ge_NULL,
         font_size);
#endif
    return;
}                                                       //end-fcn_DispStringNoCursor

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DispCursor.c )))))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

    Function:           draw a box around the current char
    Input:              line number, addr on line, cursor control attrib
    Return:             none
    Notes: 
    
--------------------------------------------------------------------------------------------------*/
void DispCursor (uint8 lcd_line_num,
                 uint8 lcd_char_addr,
                 uint8 cursor_cntrl,
                 uint8 font_size,
                 uint8 string_length)
{
uint8        width, height, grph_left, grph_right, grph_top, grph_bottom, glyph;

    if(cursor_cntrl == grphCURSOR_ON){
         glyph = grphDispRam[lcd_line_num][lcd_char_addr];// get char to box in

                                                        // check to make sure the symbol is a legal one
                                                        // if not then just replace it with the default character
        if((glyph < fonts[font_size].glyph_beg) || (glyph > fonts[font_size].glyph_end))
        {
            glyph = fonts[font_size].glyph_def;
        }

                                                        // make zero based index into the font data arrays
        glyph -= fonts[font_size].glyph_beg;
        width = fonts[font_size].fixed_width;           // check if it is a fixed width
        if(width == 0)
        {
            width = fonts[font_size].width_table[glyph];// get the variable width instead
        }

        height = fonts[font_size].glyph_height;

        grph_left   = ((lcd_char_addr * width) + (uint8)grphMARGIN_LEFT) - 1;   // sub vert line space
        grph_right  = grph_left + (string_length *width) + 1;             // add vert line space
        grph_top    = ((uint8)grphMARGIN_TOP + (lcd_line_num * (height + (uint8)grphROW_SPACES))) - (uint8)grphROW_SPACES;
        grph_bottom = (grph_top + height) + 1;          // find lower rt corner, which is just font size away

        grphLeftCursor  = grph_left;                    // save cursor co-ord's, for DispCursorOff fcn
        grphRightCursor = grph_right;
        grphTopCursor   = grph_top;
        grphBottomCursor= grph_bottom;

        grphVertLine
            (grph_top,
             grph_bottom,
             grph_left);
        grphVertLine
            (grph_top,
             grph_bottom,
             grph_right);
        grphHorizLine
            (grph_left,
             grph_right,
             grph_top);
        grphHorizLine
            (grph_left,
             grph_right,
             grph_bottom);

        grphHorizLine
            (grph_left,
             grph_right,
             grph_bottom + 1); // double bottom line

        grphUpdate
            (grph_top,                                  // just reshow the used area to reduce I/O
             grph_right + grph_bottom);                 // lower rt corner

        grphCursorStatus = grphCURSOR_ON;               // mark cursor as on
    }
    return;
}                                                       //end-fcn_DispCursor

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DispCursorOff )))))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

    Function:           deletes the box around the current char
    Input:              none uses global variables listed below
    Return:             none
    Notes: 
                        grphLeftCursor,
                        grphRightCursor,
                        grphTopCursor,
                        grphBottomCursor;
--------------------------------------------------------------------------------------------------*/
void DispCursorOff (void)
{
    if(grphCursorStatus == grphCURSOR_ON){
        grphClrVertLine
            (grphTopCursor,
             grphBottomCursor,
             grphLeftCursor);
        grphClrVertLine
            (grphTopCursor,
             grphBottomCursor,
             grphRightCursor);
        grphClrHorizLine
            (grphLeftCursor,
             grphRightCursor,
             grphTopCursor);
        grphClrHorizLine
            (grphLeftCursor,
             grphRightCursor,
             grphBottomCursor);

        grphClrHorizLine
            (grphLeftCursor,
             grphRightCursor,
             grphBottomCursor + 1);                         // double bottom line

        grphUpdate
            (grphTopCursor,                                 // just reshow the used area to reduce I/O
             grphRightCursor + grphBottomCursor);           // lower rt corner

        grphCursorStatus = grphCURSOR_OFF;                  // mark cursor as off
    }

    return;
}                                                       //end-fcn_DispCursorOff

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_grphClrView ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       vFn_grphClrView

    Input:          None

    Return:         None

    Notes:          clears the shadow display lines, these is the lines used to redisplay the a particular menu function 
                    erases lcd controller memory
                    
----------------------------------------------------------------------------------------------------*/
void vFn_grphClrView (uint8 font_size)
{
uint8        i, j, grph_max_chars_per_line;

    grph_max_chars_per_line = u8Fn_grphSetFont(font_size);
    for(i = 0; i < grphMAX_LINES_IN_WINDOW; i++){       // clear out nulls if any from entire grph window
        for(j = 0; j < grph_max_chars_per_line; j++){   // number chars in line depends on font size
            grphDispRam[i][j] = ' ';                    // write spaces to disp ram
        }
    }

    grphErase();                                        // erase lcd ram on controller
    grphOutline();                                      // erases physical screen, redraws the tablet outline, and does update
                                                        //      does update to display controller memory containing outline

    lcdCharAddr = START_OF_LINE;                        // init global char and line address's
    lcdLineAddr = grphLINE_ONE;


    return;
}                           // end-fcn_vFn_grphClrView

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( grphClrLine ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       grphClrLine

    Input:          uint8 lcd_line_num
                    uint8 font_size

    Return:         None

    Notes:         clears one line of chars on grph screen based on font size, an line num
                        grphMARGIN_TOP is the number of rows reserved for top margin at the, relates to screen border
                        grphMARGIN_BOTTOM is the number of rows reserved for bottom margin, relates to screen border
                        grphGRAPH_ROW_SPACES is the number of rows beneath the char for spacing
                       a char frame is defined to be the top left corner to the bottom right corner + the grphROW_SPACES

----------------------------------------------------------------------------------------------------*/
void grphClrLine (uint8 lcd_line_num,
                  uint8 font_size)
{
uint8        grph_left, grph_right, grph_top, grph_bottom, height, size_error_top = FALSE;

    height = fonts[font_size].glyph_height;

    grph_left   = (uint8)grphMARGIN_LEFT;                      // start within the box outline
    grph_right  = SCRN_RIGHT - grphMARGIN_RIGHT;        // sub rt margin if any

    grph_top    = (uint8)grphMARGIN_TOP + (lcd_line_num * (height + (uint8)grphROW_SPACES));
    grph_bottom = ( (lcd_line_num + 1) * height) + (uint8)grphMARGIN_TOP;   // find lower rt corner, which is just font size + grpROW_SPACES away

    if(grph_top > (SCRN_BOTTOM - (uint8)grphMARGIN_BOTTOM)){
        size_error_top = TRUE;                          // error in top calc, bypass
    }

    if(grph_bottom > (SCRN_BOTTOM - (uint8)grphMARGIN_BOTTOM)){
        grph_bottom = SCRN_BOTTOM - (uint8)grphMARGIN_BOTTOM;
    }

    if(size_error_top == FALSE){                        // don't clear if top calc wrong
        grphClrArea
            (grph_left,
             grph_right,
             grph_top,
             grph_bottom);
    }

    grphUpdate
        (grph_top,                                      // just reshow the used area to reduce I/O
         grph_bottom);                                  // lower rt corner


    return;
}                           // end-fcn_grphClrLine



