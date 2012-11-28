#ifndef DEF_TIMEDATESTRUCTUREINIT_H
    #define DEF_TIMEDATESTRUCTUREINIT_H

/*============================================================================================================
                                $RCSfile: TimeDateStructInit.h,v $

       $Author: blewis $
       $Revision: 1.2 $
       $Date: 2006/05/18 05:19:07 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/TimeDateStructInit.h,v $

=============================================================================================================*/

/*---------------------------0123456789ABCDEF0123---------------------------------------------------
                             L014(01:23) P(00:00)
                             L013(01:23.456)>L014
                             L012(01:23)            
                             L011(01:23)            
// note:    The structures are related to line positioning and properties of the line, ie enable disable, the dynamic
                flds have the min and max values included. A separate structure is required for the string variable
                
typedef struct lcdDynFlds           // used by dynamic structures, defines where the dynamic fields are located on the display, 
{                                   //   if enabled, and their min and max values
    sint8 lcdLine;                   //   lcdFldEnable, MinValue, MaxValue defaulted (not filled in) if not used
    sint8 lcdAddr;
    sint8 lcdFldEnable;
    sint8 MinValue;
    sint8 MaxValue;
};

typedef struct lcdAddr       // used by static structures
{
    sint8 lcdLine;
    sint8 lcdAddr;
    sint8 lcdFldEnable;
};

---------------------------------------0123456789ABCDEF0123--------------------------------------
      0        1         2
      01234567890123456789          // char position , LINE_ONE is the Title
      GT:1234.5  AT:123.56          // LINE_TWO
      LT:123.45  CT:123.56          // LINE_THREE
      SpeedOff:Y Clr:Y              // LINE_FOUR
      01234567890123456789             char postion
----------------------------------------------------------------------------------------------------*/

lcdTimeDateStaticStruct lcdTimeDateStatic = 
{
//---------------------------------------------------------------------------------------------------
    {"12:","34",":56"},                                 // TimeTxtStr hours, mins, secs
//---------------------------------------------------------------------------------------------------
    {HOURS_STATIC_LINE,                                 // Hours  line #
     HOURS_STATIC_ADDR,                                 // addr column
     ge_YES},                                              // enable fld

    {MINS_STATIC_LINE,                                  // Mins  line #
     MINS_STATIC_ADDR,                                  // addr column
     ge_YES},                                              // enable fld

    {SECS_STATIC_LINE,                                  // Secs  line #
     SECS_STATIC_ADDR,                                  // addr column
     ge_YES},                                              // enable fld
//---------------------------------------------------------------------------------------------------
//===================================================================================================
    {"(Thur)","June","/30/","2007"},                    // DateTxtStr  WeekDay, Month, Date, Year
//---------------------------------------------------------------------------------------------------
    {WEEKDAY_STATIC_LINE,
     WEEKDAY_STATIC_ADDR,
     ge_YES},
     
    {MONTH_STATIC_LINE,
     MONTH_STATIC_ADDR,
     ge_YES},
     
    {DATE_STATIC_LINE,
     DATE_STATIC_ADDR,
     ge_YES},
     
    {YEAR_STATIC_LINE,
     YEAR_STATIC_ADDR,
     ge_YES}
};

lcdTimeDateDynStruct lcdTimeDateDyn = 
{
    {HOURS_STATIC_LINE,
     MS_HOURS_DYN_ADDR,
     ge_YES,
     MS_HOURS_MIN_VALUE,
     MS_HOURS_MAX_VALUE},                                 // MS_HOURS 0x00

    {HOURS_STATIC_LINE,
     LS_HOURS_DYN_ADDR,
     ge_YES,
     LS_HOURS_MIN_VALUE,
     LS_HOURS_MAX_VALUE},                                 // LS_HOURS 0x01
//----------------------
    {MINS_STATIC_LINE,
     MS_MINS_DYN_ADDR,
     ge_YES,
     MS_MINS_MIN_VALUE,
     MS_MINS_MAX_VALUE},                                  // MS_MINS  0x03

    {MINS_STATIC_LINE,
     LS_MINS_DYN_ADDR,
     ge_YES,
     LS_MINS_MIN_VALUE,
     LS_MINS_MAX_VALUE},                                  // LS_MINS  0x04
//----------------------
    {SECS_STATIC_LINE,
     MS_SECS_DYN_ADDR,
     ge_YES,
     MS_SECS_MIN_VALUE,
     MS_SECS_MAX_VALUE},                                  // MS_SECS 0x06

    {SECS_STATIC_LINE,
     LS_SECS_DYN_ADDR,
     ge_YES,
     LS_SECS_MIN_VALUE,
     LS_SECS_MAX_VALUE},                                  // LS_SECS 0x07
//------------------------------------------------------------------------------------------------------------------
    {WEEKDAY_STATIC_LINE,
     WEEKDAY_DYN_ADDR,
     ge_YES,
     NULL_CHAR,
     NULL_CHAR},                                 // WEEKDAY 0x01
//----------------------
    {MONTH_STATIC_LINE,
     MONTH_DYN_ADDR,
     ge_YES,
     NULL_CHAR,
     NULL_CHAR},                                   // MONTH   0x06
//----------------------
    {DATE_STATIC_LINE,
     MS_DATE_DYN_ADDR,
     ge_YES,
     MS_DATE_MIN_VALUE,
     MS_DATE_MAX_VALUE},                                 // MS_DATE 0x0B

    {DATE_STATIC_LINE,
     LS_DATE_DYN_ADDR,
     ge_YES,
     LS_DATE_MIN_VALUE,
     LS_DATE_MAX_VALUE},                                 // LS_DATE 0x0C
//----------------------
    {YEAR_STATIC_LINE,
     MS_YEAR_DYN_ADDR,
     ge_YES,
     MS_YEAR_MIN_VALUE,
     MS_YEAR_MAX_VALUE},                                 // MS_YEAR 0x10

    {YEAR_STATIC_LINE,
     LS_YEAR_DYN_ADDR,
     ge_YES,
     LS_YEAR_MIN_VALUE,
     LS_YEAR_MAX_VALUE}                                  // LS_YEAR 0x11
//----------------------
};

#endif


