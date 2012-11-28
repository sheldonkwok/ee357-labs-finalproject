#ifndef DEF_GRPH_ENUM_DEF_H
    #define DEF_GRPH_ENUM_DEF_H

enum grphTypeList
{
    GRPH_CMND_MODE,
    GRPH_DATA_MODE
};

//============================================================================================================

enum grphCmndList
{
    GRPH_SOFT_RESET     = 0xE2,
    GRPH_CONTRAST_RATIO = 0x24,
    GRPH_POWER_MODE     = 0x2F,
    GRPH_CONTRAST_MODE  = 0x81,
    GRPH_PAGE_SET       = 0xB0,
    GRPH_COL_HIGH_SET   = 0x10,
    GRPH_COL_LOW_SET    = 0x00,
    GRPH_DISP_ON        = 0xAF
};

enum grphCharLineList
{
    grphLINE_ONE    = 0,
    grphLINE_TWO    = 1,
    grphLINE_THREE  = 2,
    grphLINE_FOUR   = 3
};

enum grphCursorStatusList
{
    grphCURSOR_ON,
    grphCURSOR_OFF
};

enum grphFontRemapList
{
    FONT_FIVE_DOT_MESS      = 5,
    FONT_SIX_DOT_MESS       = 6,
    FONT_SEVEN_DOT_MESS     = 7,
    FONT_NINE_DOT_MESS      = 9,
    FONT_TEN_DOT_MESS       = 10,
    FONT_FIFTEEN_DOT_MESS   = 15,
    FONT_EIGHTEEN_DOT_MESS  = 18
};

enum ScrollCntrlList
{
    SCROLL_ON,
    SCROLL_OFF,
    SINGLE_LINE
};

#endif
