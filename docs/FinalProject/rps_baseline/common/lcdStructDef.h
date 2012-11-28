#ifndef DEF_GLOBALVARDEFINITIONS_H
    #define DEF_GLOBALVARDEFINITIONS_H

/*============================================================================================================
                                $RCSfile: GlobalVarDef2.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.4 $
       $Date: 2007/05/20 23:06:05 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/GlobalVarDef2.h,v $

    Note: For the dynamic and static structures, the main fcn of the structure is to hold the lcd line and char
            address. Enable of the line is added in some structures as well as the min and max values for the field
            If text is to be initialized in the structure a separate text string structure is needed
=============================================================================================================*/

#include "lcdConstDef.h"
#include "grphEnumDef.h"

/*
typedef union u1
{
    struct testflds1
    {
        uint8 test1;
    }tstfld1;
    
    struct testflds2
    {
        uint8 test1;
    }tstfld2;
};
*/
//---------------------------------------------------------------------------------------------------------------

#ifndef DEF_LCDADDRESSING
    #define DEF_LCDADDRESSING
    typedef struct lcdAddressing
    {
        uint8 lcdLine;
        uint8 lcdAddr;
        uint8 lcdFldEnable;
    }lcdAddressing;
#endif

#ifndef DEF_LCDDYNFLDS
    #define DEF_LCDDYNFLDS
typedef struct lcdDynFlds
    {
        uint8 lcdLine;
        uint8 lcdAddr;
        uint8 lcdFldEnable;
        uint8 MinValue;
        uint8 MaxValue;
    }lcdDynFlds;
#endif

#ifndef DEF_LCDADDRESS2
    #define DEF_LCDADDRESS2
    typedef struct lcdAddressing2
    {
        uint8 Line;
        uint8 Char;
        uint8 FldEnable;
    }lcdAddressing2;
#endif

#ifndef DEF_LCDDYNFLDS2
    #define DEF_LCDDYNFLDS2
typedef struct lcdDynFlds2
    {
        struct lcdAddressing2 Addr; 
        uint8  MinValue;
        uint8  MaxValue;
    }lcdDynFlds2;
#endif


#ifndef DEF_LCDMENUCNTRL
    #define DEF_LCDMENUCNTRL
    typedef struct lcdMenuCntrl
    {
        uint8                       FcnSel;                    // used by menu incr fcn as loop storage
        uint8                       FirstSel;                  // index limits, must be set in init struct
        uint8                       LastSel;
    }lcdMenuCntrlStruct, *lcdMenuCntrlStructPtr;
#endif

//======================================= Time Static ==============================================

//----------------------------------------------0123456789ABCDEF0123--------------------------------
//                                              HH:MM.51
//----------------------------------------------0123456789ABCDEF0123--------------------------------
//                                              (Thur)Sept/30/2002  ";
//--------------------------------------------------------------------------------------------------
#ifndef DEF_LCDTIMETXTSTR
    #define DEF_LCDTIMETXTSTR
    typedef struct lcdTimeTxtStr
    {
        uint8   Hours[HOURS_STATIC_SIZE];                    // init'd in lcdTimeDateStructInit
        uint8   Mins [MINS_STATIC_SIZE];                     // init'd 
        uint8   Secs [SECS_STATIC_SIZE];                     // init'd 
    }lcdTimeTxtStr;
#endif

#ifndef DEF_LCDDATETXTSTR
    #define DEF_LCDDATETXTSTR
    typedef struct lcdDateTxtStr
    {
        uint8   WeekDay[WEEKDAY_STATIC_SIZE];                // init'd 
        uint8   Month  [MONTH_STATIC_SIZE];                  // init'd 
        uint8   Date   [DATE_STATIC_SIZE];                   // init'd 
        uint8   Year   [YEAR_STATIC_SIZE];                   // init'd 
    }lcdDateTxtStr;
#endif

//====================================== Time Date =======================================================
#ifndef DEF_LCDTIMEDATESTATIC
    #define DEF_LCDTIMEDATESTATIC
typedef struct lcdTimeDateStatic
{
    //--------------------------------------------------------------------------------------------------
        struct lcdTimeTxtStr TimeTxtStr;
    //--------------------------------------------------------------------------------------------------
        struct lcdAddressing2 Hours;                         // 0x00
        struct lcdAddressing2 Mins;                          // 0x04
        struct lcdAddressing2 Secs;                          // 0x07
    //==================================================================================================

        struct lcdDateTxtStr DateTxtStr;
    //--------------------------------------------------------------------------------------------------
        struct lcdAddressing2 WeekDay;                       // 0x01
        struct lcdAddressing2 Month;                         // 0x06
        struct lcdAddressing2 Date;                          // 0x0C
        struct lcdAddressing2 Year;                          // 0x11
    }lcdTimeDateStaticStruct, *lcdTimeDateStaticStructPtr;
#endif

//======================================= Dynamic ==================================================
#ifndef DEF_LCDTIMEDATEDYN
    #define DEF_LCDTIMEDATEDYN
    typedef struct lcdTimeDateDyn
    {
        struct lcdDynFlds2 MsHours;                          // 0x00
        struct lcdDynFlds2 LsHours;                          // 0x01

        struct lcdDynFlds2 MsMins;                           // 0x03
        struct lcdDynFlds2 LsMins;                           // 0x04

        struct lcdDynFlds2 MsSecs;                           // 0x06
        struct lcdDynFlds2 LsSecs;                           // 0x07
    //-------------------------------------------------------------------------------------------------
        struct lcdDynFlds2 WeekDay;                          // 0x01
        struct lcdDynFlds2 Month;                            // 0x06
        struct lcdDynFlds2 MsDate;                           // 0x0B
        struct lcdDynFlds2 LsDate;                           // 0x0C
        struct lcdDynFlds2 MsYear;                           // 0x10
        struct lcdDynFlds2 LsYear;                           // 0x11
    }lcdTimeDateDynStruct, *lcdTimeDateDynStructPtr;
#endif

//========================================================================================================

#ifndef DEF_LCDTITLEFLDS
    #define DEF_LCDTITLEFLDS
    typedef struct lcdTitleFlds
    {
        struct  lcdAddressing Title;
        uint8   TitleTxt[grphMAX_CHARS_PER_LINE + 1];             // add NULL
        uint8   Blink;
        uint8   BlinkCnt;
        uint8   Startup;
        uint8   StartupState;
    }lcdTitleFlds;
#endif

#endif                      //end-def_DEF_GLOBALVARDEFINITIONS_H
