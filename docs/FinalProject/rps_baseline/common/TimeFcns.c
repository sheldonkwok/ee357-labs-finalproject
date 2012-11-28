/*============================================================================================================
                                $RCSfile: TimeFcns.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
        Revision = "$WCREV$"
        Modified = "$WCMODS?Modified:Not modified$"
        Date     = "$WCDATE$"
        RevRange = "$WCRANGE$"
        Mixed    = "$WCMIXED?Mixed revision WC:Not mixed$"
        URL      = "$WCURL$"  

=============================================================================================================*/
#include "cpu.h"
#include "RPS_BldOptions.h"

#include "RxTx_spi.h"
#include "TimerDelay.h"
#include "TypeDef.h"
#include "rtcConstDef.h"
#include "lcdConstDef.h"
#include "DateTimeConstDef.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "lcdStructDef.h"

#include "DeviceRegStructDef.h"
#include "ExtTimeDeclar.h"

#include "SwitchStructDef.h"
#include "ExtSwitchDeclar.h"

#include "ExtGlobalVarDeclar.h"
#include "rtcAlarmStructDef.h"

#include "FcnDef.h"

#include "scaldate.h"
#include "datetime.h"

#if ISO_CAL       /* monday == 0 */
 #define ADJ 5
#else             /* sunday == 0 */
 #define ADJ 6
#endif

void                        vFn_rtcCntrl                  (uint16         u16_cntrl_state);

void                        vFn_TestReadAlarm                   (void);

void                        vFn_lcdBkLite                       (sint16         light_status);

void                        rtcDisableClock                     (void);
void                        rtcClkMenu                          (uint8          font_size);

unsigned                    dow                                 (unsigned       yr,
                                                                 unsigned       mo,
                                                                 unsigned       day);
uint8   lcdStaticDisplayedFlag;
uint8   SaveUpdate[] = "N";

s_rtcAlarmStruct    s_rtc_alarm ={
                                    "fwm822.wav",               // wave file
                                    "AlrmClk1.txt",             // script file
                                    ge_RTC_ALARM_ON,            // turn the alarm on
                                    ge_RTC_ALARM_ON_SECS, // alarm on secs match
                                    0x05,                       // Month
                                    0x04,                       // day
                                    0x03,                       // hours
                                    0x02,                       // mins
                                    0x14                        //secs
                                    };

ps_rtcAlarmStruct           ps_rtc_alarm;


lcdMenuCntrlStructPtr        lcdTimeDateMenuCntrlPtr,
                             lcdAlarmMenuCntrlPtr,
                             rtcClkMenuCntrlPtr;

lcdMenuCntrlStruct lcdTimeDateMenuCntrl =
{
    0,                                                  // index used to cntrl menu operation
    ge_FIRST_MENU_SEL,                                  // first index
    ge_LAST_TIME_MENU_SEL - 1                           // last index, adjust by -1 due to enum
};

lcdMenuCntrlStruct rtcClkMenuCntrl =
{
    0,                                                  // index used to cntrl menu operation
    ge_FIRST_MENU_SEL,                                  // first index
    ge_LAST_RTCCLK_MENU_SEL - 1                         // last index
};


#ifdef HW_i2c_RTC_BQ32000
    rtc_bq32000_union           u_rtc_bq32000;
    rtc_bq32000_union_ptr       pu_rtc_bq32000;
#endif

#ifdef HW_i2c_RTC_M41T81S
    m41t81s_rtc_union           u_rtc_m41t81s;
    m41t81s_rtc_union_ptr       pu_rtc_m41t81s;
#endif

//--------------------------------------------------------------------------------------------------
#define AFE_M41T81S         0x07                        // alarm enable bit
#define SQWE_M41T81S        0x06                        // square wave enable bit
#define HT_M41T81S          0x06                        // clock enable bit
#define OF_M41T81S          0x02                        // oscillator fail flag

#define ALARM_RPT5_MASK     0x40
#define ALARM_RPT4_MASK     0x80
#define ALARM_RPT3_MASK     0x80
#define ALARM_RPT2_MASK     0x80
#define ALARM_RPT1_MASK     0x80

//--------------------------------------------------------------------------------------------------


/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( vFn_AlarmFcns ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:          Alarm set from alarm struct, values retrieved from user menu via LCD
    Input Parameter:   uint8  font_size
    Return Type:        none
    
    Notes:  Alarm on Year & Date not added, must also add date to LCD menu
            Within the Exit_Function - Sel switch exits to next major fcn,
            Rt or Lft moves forward and back between major functions
            within a minor time fcn 
            up, down increments and decrements entries, rt, lft moves through entry, sel exits minor fcn
--------------------------------------------------------------------------------------------------*/
void vFn_AlarmFcns (uint8 font_size)  
{
//                                123456789012345678901
uint8   alarm_mssg1[4][25] =  { {"Alarm on match"},
                                {"00:00:SS  secs"},
                                {"00:MM:SS  mins"},
                                {"HH:MM:SS  hrs"}
                              };

uint8   alarm_mssg2[4][25] =  { {"To Activate alarm"},
                                {"Set Save = Y"},
                                {"then push rt"},
                                {"then sel (vert)"}
                              };
uint8   alarm_mssg3[4][25] =  { {"Right or Left"},
                                {"to continue"},
                                {" "},
                                {" "}
                              };

TimeDateStruct     lcd_system_time_table;                // must work on a copy else, if validation fails, system time is corrupted
TimeDateStructPtr  lcd_system_time_table_ptr;

    lcd_system_time_table        = SystemTimeTable;      // capture system time 
    lcd_system_time_table_ptr    = &lcd_system_time_table;

    lcdTimeDateMenuCntrlPtr = &lcdTimeDateMenuCntrl;
    ps_rtc_alarm            = &s_rtc_alarm;

#ifdef DISP_RTC_ALARM_INSTRUCTIONS
    vFn_grphClrView(FontSizeCurr);                      // clean draw outline
    
    vFn_DispLineSet
        ( &(alarm_mssg1[0][0]),                         // where mssg starts
         sizeof(alarm_mssg1[0]),                        // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    
    TimeDelay(ge_T_4_SEC);
    
    vFn_DispLineSet
        ( &(alarm_mssg2[0][0]),                         // where mssg starts
         sizeof(alarm_mssg2[0]),                        // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    
    TimeDelay(ge_T_4_SEC);
#endif

    rtcReadDateTime(lcd_system_time_table_ptr);         // read discard current SystemTimeTable

    vFn_grphClrView(FONT_NINE_DOT);

    MenuFlag = ge_CONT_MENU;
    lcdStaticDisplayedFlag = FALSE;                     // display static information (mark flag as not displayed yet)
    SaveUpdate[0] = 'N';                                // start in new state each time

    while (MenuFlag != ge_EXIT_MENU){ 
       TimeMenu
          (lcdTimeDateStaticPtr,                        // must push sel to exit and set or not set alarm
           lcdTimeDateDynPtr,                           // after return, changed alarm is in SystemTimeTable
           lcd_system_time_table_ptr,                    // working copy of system time
           lcdTimeDateMenuCntrlPtr,
           ge_ALARM_DISPLAY,
           font_size);
    }

    if(SaveUpdate[0] == 'Y'){

        vFn_rtcAlarmTimeSet
            (ps_rtc_alarm,
             lcd_system_time_table_ptr,
             ge_RTC_ALARM_ON_MIN);                        // set alarm mode, time

        u8_rtcAlarmIntFlag = ge_RTC_ALARM_INT_RESET;    // clear flag in events.c, preparation for alarm
        vFn_rtcAlarmOper(ge_RTC_ALARM_ENABLE_0);        // enable alarm

        DispClockStatic
           (lcdTimeDateStaticPtr,
            ge_TIME_ENABLED,
            ge_DATE_ENABLED,
            font_size);

        while(u8_rtcAlarmIntFlag == ge_RTC_ALARM_INT_RESET){
            vFn_rtcClockDispUpdate
                (lcd_system_time_table_ptr,
                 lcdTimeDateDynPtr,
                 FONT_NINE_DOT); 
        };                                              // wait for interrupt, or remove and do via interrupt pointer to fcn

        gu8_rtcFlags = vFn_ReadReg_i2c(ge_M41T81S_ALARM_FLAGS); //clear the interrupt, shouldn't be done in int

        vFn_rtcAlarmAction(ps_rtc_alarm);               // execute alarm action
    }else {
        vFn_rtcAlarmOper(ge_RTC_ALARM_DISABLE_0);       // disable alarm int
    }

    vFn_DispLineSet
        ( &(alarm_mssg3[0][0]),                         // where mssg starts
         sizeof(alarm_mssg3[0]),                        // size of line
         FONT_NINE_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines

    ExitOperationPrompt(font_size);                     // find out which direction to go, sel restarts same fcn
                                                        // left and right moves to next major entry, left or rt
    
    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( TimeFcns ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:          Time Date all set from single set of entries
    Input Parameter:   uint8  font_size
    Return Type:        none
    
    Notes:  Within the Exit_Function - Sel switch exits to next major fcn,
            Rt or Lft moves forward and back between major functions
            within a minor time fcn 
            up, down increments and decrements entries, rt, lft moves through entry, sel exits minor fcn
--------------------------------------------------------------------------------------------------*/
void TimeFcns (uint8 font_size)  
{
TimeDateStruct     lcd_system_time_table;                // must work on a copy else, if validation fails, system time is corrupted
TimeDateStructPtr  lcd_system_time_table_ptr;

    rtcReadDateTime(SystemTimeTablePtr);                // get rtc system time

    lcd_system_time_table        = SystemTimeTable;      // capture system time 
    lcd_system_time_table_ptr    = &lcd_system_time_table;

    lcdTimeDateMenuCntrlPtr = &lcdTimeDateMenuCntrl;

    MenuFlag = ge_CONT_MENU;
    lcdStaticDisplayedFlag = FALSE;                     // display static information (mark flag as not displayed yet)
    SaveUpdate[0] = 'N';                                // start in new state each time

    while (MenuFlag != ge_EXIT_MENU){ 
       TimeMenu
          (lcdTimeDateStaticPtr,                        // need to pass time date and time locations on lcd
           lcdTimeDateDynPtr,
           lcd_system_time_table_ptr,                    // work with a copy
           lcdTimeDateMenuCntrlPtr,
           ge_TIME_DATE_DISPLAY,
           font_size);
    }

    if(SaveUpdate[0] == 'Y'){
        SystemTimeTable = lcd_system_time_table;         // re-write to RTC is done only on exit, and if save = Y
        rtcWriteTime(SystemTimeTablePtr);               // set the system time, all is well
        rtcWriteDate(SystemTimeTablePtr);               //  it will have been set in TimeMenu
    }else {
        rtcReadDateTime(SystemTimeTablePtr);            // read, discard other values
    }

    ExitOperationPrompt(font_size);                     // find out which direction to go, sel restarts same fcn
                                                        // left and right moves to next major entry, left or rt
    
    return;
}

//==================================================================================================
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( TimeMenu )))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Allow user change of Time and Date
    
    Input:          (struct   lcdTimeDateStatic *lcd_time_date_static_ptr,
                     struct   lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                     enum_16  ge_alarm_dislay_type,
                     uint8    font_size)

    Return:         None
    
    Notes:          work with a copy of system time struct
----------------------------------------------------------------------------------------------------*/
void TimeMenu (struct   lcdTimeDateStatic    *lcd_time_date_static_ptr,
               struct   lcdTimeDateDyn       *lcd_time_date_dyn_ptr,
               struct   TimeDate             *lcd_system_time_table_ptr,
               struct   lcdMenuCntrl         *lcd_menu_cntrl_ptr,
               uint16   ge_alarm_dislay_type,
               uint8    font_size)
{
uint8 volatile  error_code, day_of_week;
uint8 *digit_pair_ptr, fcn_sel_merged;
uint16 selection_type;

    if(lcdStaticDisplayedFlag == FALSE){                // display static information (mark flag as not displayed yet)
         DispTimeStatic
            (lcd_time_date_static_ptr,
             font_size);

        DispDateStatic
           (lcd_time_date_static_ptr,
            font_size);

        if(ge_alarm_dislay_type == ge_ALARM_DISPLAY){
            DispStringNoCursor
               (lcd_time_date_dyn_ptr->MsHours.Addr.Line,
                MID_LINE,
                (byte *)"Alarm",                        // this displays string upto first NULL incountered        
                ge_CLEAR_LINE,
                font_size);
        }

        DispStringNoCursor
           (ge_TIME_DATE_SAVE_STATIC_LINE,              // line address is not relative 
            ge_TIME_DATE_SAVE_STATIC_ADDR,              // base + offset within line is absolute LCD address of Hours fld
            (byte *)"Save:N",                           // this displays string upto first NULL incountered        
            ge_DONOT_CLEAR_LINE,
            font_size);
        
        lcdStaticDisplayedFlag = TRUE;                  // mark as displayed
    }

    DispTimeStringDyn
       (lcd_system_time_table_ptr,                       // update the time template 
        lcd_time_date_dyn_ptr,
        font_size);

    DispDateStringDyn
       (lcd_system_time_table_ptr,
        lcd_time_date_dyn_ptr,
        font_size);
//------------------------------------------------------------------------------------------------------------
    switch (lcd_menu_cntrl_ptr->FcnSel){
       case ge_TIME_HOURS_MS_DIGIT:

         if(lcd_time_date_dyn_ptr->MsHours.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_hours); // work with Hours

            IncrDispDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->MsHours.Addr.Line,
                lcd_time_date_dyn_ptr->MsHours.Addr.Char ,// this points to MS digit
                lcd_time_date_dyn_ptr->MsHours.MinValue,
                lcd_time_date_dyn_ptr->MsHours.MaxValue,
                MS_DIGIT,
                font_size);                   // MS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;

       case ge_TIME_HOURS_LS_DIGIT:

         if(lcd_time_date_dyn_ptr->LsHours.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_hours); // work with Hours

            IncrDispHoursLSDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->LsHours.Addr.Line,
                lcd_time_date_dyn_ptr->LsHours.Addr.Char,
                lcd_time_date_dyn_ptr->LsHours.MinValue,
                lcd_time_date_dyn_ptr->LsHours.MaxValue,
                font_size);                             // always LS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;
//------------------------------------------------------------------------------------------------------------

       case ge_TIME_MINUTES_MS_DIGIT:

         if(lcd_time_date_dyn_ptr->MsMins.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_mins); // work with Mins

            IncrDispDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->MsMins.Addr.Line,
                lcd_time_date_dyn_ptr->MsMins.Addr.Char,
                lcd_time_date_dyn_ptr->MsMins.MinValue,
                lcd_time_date_dyn_ptr->MsMins.MaxValue,
                MS_DIGIT,
                font_size);                             // MS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;

       case ge_TIME_MINUTES_LS_DIGIT:
 
         if(lcd_time_date_dyn_ptr->LsMins.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_mins); // work with Mins

            IncrDispDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->LsMins.Addr.Line,
                lcd_time_date_dyn_ptr->LsMins.Addr.Char,
                lcd_time_date_dyn_ptr->LsMins.MinValue,
                lcd_time_date_dyn_ptr->LsMins.MaxValue,
                LS_DIGIT,
                font_size);                             // LS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;
//------------------------------------------------------------------------------------------------------------

       case ge_TIME_SECONDS_MS_DIGIT:

         if(lcd_time_date_dyn_ptr->MsSecs.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_secs); // work with Secs

            IncrDispDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->MsSecs.Addr.Line,
                lcd_time_date_dyn_ptr->MsSecs.Addr.Char,
                lcd_time_date_dyn_ptr->MsSecs.MinValue,
                lcd_time_date_dyn_ptr->MsSecs.MaxValue,
                MS_DIGIT,
                font_size);                             // MS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;

       case ge_TIME_SECONDS_LS_DIGIT:

         if(lcd_time_date_dyn_ptr->LsSecs.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_secs); // work with Secs

            IncrDispDigit
               (digit_pair_ptr,                         // pointer to hex digit pair
                lcd_time_date_dyn_ptr->LsSecs.Addr.Line,
                lcd_time_date_dyn_ptr->LsSecs.Addr.Char,
                lcd_time_date_dyn_ptr->LsSecs.MinValue,
                lcd_time_date_dyn_ptr->LsSecs.MaxValue,
                LS_DIGIT,
                font_size);                             // LS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;
//------------------------------------------------------------------------------------------------------------

       case ge_DATE_MONTH:
         if(lcd_time_date_dyn_ptr->Month.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            IncrDispMonth
                (lcd_system_time_table_ptr,
                 lcd_time_date_dyn_ptr,
                 font_size);
            UpdateWeekDay
                (lcd_system_time_table_ptr,
                 lcd_time_date_dyn_ptr,
                 font_size);                            // calc the week day, update and display
         }else {
                   selection_type = ge_BYPASS_FIELD;
         }

       break;
//------------------------------------------------------------------------------------------------------------

       case ge_DATE_DATE_MS_DIGIT:

         if(lcd_time_date_dyn_ptr->MsDate.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            IncrDispDateMSDigit
                (lcd_system_time_table_ptr,               // work with a copy, not the active system time
                 lcd_time_date_dyn_ptr->MsDate.Addr.Line,
                 lcd_time_date_dyn_ptr->MsDate.Addr.Char,
                 font_size);
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;

       case ge_DATE_DATE_LS_DIGIT:

         if(lcd_time_date_dyn_ptr->LsDate.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            IncrDispDateLSDigit
                (lcd_system_time_table_ptr,               // work with a copy, not the active system time
                 lcd_time_date_dyn_ptr->LsDate.Addr.Line,
                 lcd_time_date_dyn_ptr->LsDate.Addr.Char,
                 font_size);
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

      break;
//------------------------------------------------------------------------------------------------------------
       case ge_DATE_YEAR_MS_DIGIT:

         if(lcd_time_date_dyn_ptr->MsYear.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_year); // work with Year

            IncrDispDigit
                (digit_pair_ptr,                         // pointer to hex digit pair
                 lcd_time_date_dyn_ptr->MsYear.Addr.Line,
                 lcd_time_date_dyn_ptr->MsYear.Addr.Char,
                 lcd_time_date_dyn_ptr->MsYear.MinValue,
                 lcd_time_date_dyn_ptr->MsYear.MaxValue,
                 MS_DIGIT,
                 font_size);                            // LS
         }else {
                   selection_type = ge_BYPASS_FIELD;
               }    

       break;

       case ge_DATE_YEAR_LS_DIGIT:

         if(lcd_time_date_dyn_ptr->LsYear.Addr.FldEnable == ge_YES){
            selection_type = ge_NORMAL_FIELD;

            digit_pair_ptr = &(lcd_system_time_table_ptr->u8_year); // work with Year

            IncrDispDigit
                (digit_pair_ptr,                         // pointer to hex digit pair
                 lcd_time_date_dyn_ptr->LsYear.Addr.Line,
                 lcd_time_date_dyn_ptr->LsYear.Addr.Char,
                 lcd_time_date_dyn_ptr->LsYear.MinValue,
                 lcd_time_date_dyn_ptr->LsYear.MaxValue,
                 LS_DIGIT,
                 font_size);                            // LS
         }else {
                   selection_type = ge_BYPASS_FIELD;             // if not enabled, set to bypass
               }    

       break;

       case ge_TIME_DATE_SAVE:

        SaveUpdate[0] = DispPrompt
                             (ge_SW_LFT,                 // check if we are supposed to save
                              ge_SW_RT,
                              ge_SW_UP,
                              ge_SW_DOWN,
                              ge_SW_SEL,
                              ge_TIME_DATE_SAVE_STATIC_LINE,
                              ge_TIME_DATE_SAVE_DYN_ADDR,
                              'Y',
                              'N',
                              &SaveUpdate[0],
                              font_size);            //basic sw's needed for menu fcns

       break;
    }                       // end-Switch

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
    error_code = IncrDateCheck(lcd_system_time_table_ptr);// check if date is correct versus month

    if(error_code == ge_VALID){
        day_of_week = UpdateWeekDay
                          (lcd_system_time_table_ptr,
                           lcd_time_date_dyn_ptr,
                           font_size);                  // calc the week day, update and display

        MenuFlag = MenuNextSelection
                      (lcd_menu_cntrl_ptr,              // adjust the menu, exit was because of rt, or lft
                       selection_type);
    }else {                                             // do not move off of selection, error has occured
            lcd_system_time_table_ptr->u8_date = 0x01; // reset date error occured

            DispStringNoCursor
               (lcd_time_date_dyn_ptr->MsDate.Addr.Line,      
                lcd_time_date_dyn_ptr->MsDate.Addr.Char,       
                (uint8 *)"1",           
                ge_DONOT_CLEAR_LINE,
                font_size);

            DispStringNoCursor
            (STATUS_LINE,
             START_OF_LINE,
             (uint8 *)"Date Error",
             ge_CLEAR_LINE,
             font_size);

            grphClrLine
                (STATUS_LINE,
                 font_size);                 // clear line first

    }

    return;                                             //end-fcn
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( UpdateWeekDay )))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       derives the week day based on year, month, date

    Input:          pointer to the time structure

    Return:         none
    
    Notes:         based on code_snippets

----------------------------------------------------------------------------------------------------*/
uint8 UpdateWeekDay (struct TimeDate         *system_time_ptr,
                     struct lcdTimeDateDyn   *lcd_time_date_dyn_ptr,
                     uint8  font_size)
{
uint8 hex_year, day_of_week;

    hex_year = BCDDouble2Hex(system_time_ptr->u8_year); // convert to hex, SystemTimeTable is bcd

    day_of_week = dow
                      (2000 + hex_year,                 // we only keep last two digits, need full integer
                       BCDDouble2Hex(system_time_ptr->u8_month),
                       BCDDouble2Hex(system_time_ptr->u8_date));

    system_time_ptr->u8_day = (uint8)day_of_week;

    DispDateWeekDayDyn
       (system_time_ptr,
        lcd_time_date_dyn_ptr,
        font_size);

    return((uint8)day_of_week);    // end-fcn_UpdateWeekDay
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( rtcClk ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:          Enter rtcClk menu
    Input Parameter:    uint8  font_size
    Return Type:        none
    
    Notes:  Within the Exit_Function -Sel switch exits to next major fcn,
            Rt or Lft moves forward and back between major functions
            within a minor  fcn 
            up, down increments and decrements entries, rt, lft moves through entry, sel exits minor fcn
--------------------------------------------------------------------------------------------------*/
void rtcClk (uint8 font_size)
{
    rtcClkMenuCntrlPtr    = &rtcClkMenuCntrl;

    MenuFlag = ge_CONT_MENU;
    lcdStaticDisplayedFlag = FALSE;                     // display static information (mark flag as not displayed yet)
    while (MenuFlag != ge_EXIT_MENU){ 

        rtcClkMenu(font_size);
    }

    ExitOperationPrompt(font_size);                     // find out which direction to go, sel restarts same fcn
                                                        // left and right moves to next major entry, left or rt
    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( rtcClkMenu ))))))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

    Function:   rtcClkMenu
    
    Input:      uint8  font_size
    
    Return:     none
    
    Notes:      Only one fcn currently, but to maintain consistancy, Sel key is used for exit    
--------------------------------------------------------------------------------------------------*/
void rtcClkMenu (uint8 font_size)
{
int selection_type = ge_NORMAL_FIELD;                            // not currently used here but required by fcn

    switch (rtcClkMenuCntrlPtr->FcnSel){

       case ge_RTC_CLOCK_DISP:

         rtcClockDisp(font_size);

       break;

    }

    MenuFlag = MenuNextSelection
                  (rtcClkMenuCntrlPtr,                  // adjust the profile menu
                   selection_type);

    return;                                             //end-fcn
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( rtcClockDisp ))))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       continuously displays time and date until a switch is pushed

    Input:          uint8  font_size

    Return:         none
    
    Notes:

----------------------------------------------------------------------------------------------------*/
void rtcClockDisp (uint8 font_size)
{
uint8        led_status, last_sec;

    swResetFlags
       (ge_SW_LFT,
        ge_SW_RT,
        ge_SW_UP,
        ge_SW_DOWN,
        ge_SW_SEL);                                      // use switch to exit, therefore init flags

    swInterruptEnable
      (ge_SW_LFT,
       ge_SW_RT,
       ge_NULL,
       ge_NULL,
       ge_SW_SEL);

    rtcReadDateTime(SystemTimeTablePtr);            // re-sync rt clock, here after system time is basis for time, interrupt driven by alarm0
    ValidateTimeDate(SystemTimeTablePtr);               // correct for valid digits,

    DispClockStatic
       (lcdTimeDateStaticPtr,
        ge_TIME_ENABLED,
        ge_DATE_ENABLED,
        font_size);

    vFn_lcdBkLite(ge_LED_ON);

    do{                                                 // wait for switch push, system interrupt should be updating clock
        vFn_rtcClockDispUpdate
            (SystemTimeTablePtr,
             lcdTimeDateDynPtr,
             FONT_NINE_DOT); 
    #ifdef NAV_SW_POLLED
        vFn_swPoll
            ( &(swStatusFlags.LftOrRtOrUpOrDwnOrSel.Status),
             ge_READ_ONCE );                            // wait on one of 5 sw's
    #endif
    }while(swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status != ge_SW_FLAG_SET); // wait for SEL switch to be pushed, then must be rt or lft*/

    return;
}                           // end-rtcClockDisp


/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_rtcClockDispUpdate ))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       displays time and date once

    Input:          pointer to time struct
                    pointer to lcd disp struct
                    uint8  font_size

    Return:         none
    
    Notes:          displays dynamic time
                        blinks alternate LEDs each update

----------------------------------------------------------------------------------------------------*/
void vFn_rtcClockDispUpdate (struct     TimeDate        *system_time_ptr,
                             struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                             uint8      font_size)
{
uint8        led_status, last_sec;

    rtcReadDateTime(system_time_ptr);        // re-sync rt clock, here after system time is basis for time, interrupt driven by alarm0

    if(last_sec != (system_time_ptr->u8_secs & 0x0F) + '0'){
        last_sec = (system_time_ptr->u8_secs & 0x0F) + '0'; // convert to ascii
        if(led_status == ge_ON){
            ledIndicator                                        // explicitly set LED's off at start up
                (ge_LED_ON,
                 ge_LED_RED,
                 ge_NULL);
            ledIndicator
                (ge_LED_OFF,
                 ge_LED_GREEN,
                 ge_NULL);
            led_status = ge_OFF;
        }else {
            ledIndicator                                        // explicitly set LED's off at start up
               (ge_LED_OFF,
                 ge_LED_RED,
                 ge_NULL);
             ledIndicator
                (ge_LED_ON,
                 ge_LED_GREEN,
                 ge_NULL);
            led_status = ge_ON;
        }
        DispClockDyn
             (system_time_ptr,                       // the lines can move but the start location is fixed within line
              lcd_time_date_dyn_ptr,
              ge_TIME_ENABLED,
              ge_DATE_ENABLED,
              font_size);
    }

    return;
}

/*--------------------------------------------------------------------------------------------------
                                  rtcFileStamp
----------------------------------------------------------------------------------------------------
    Function:       write a specific flash profile record to an sd file 

    Input:          profile index of profile to write

    Return:         None

    Notes:          The system time ptr is hard coded here, because this is an emulation of a second party 
                        rtn used in the file system
--------------------------------------------------------------------------------------------------*/
char rtcFileStamp (uint8 *hour,
                   uint8 *min,
                   uint8 *sec,
                   uint8 *date,
                   uint8 *month,
                   uint16 *year)
{
    *sec    = SystemTimeTablePtr->u8_secs ;       // BCD time values, system clock is always set by RTC
    *min    = SystemTimeTablePtr->u8_mins ;
    *hour   = SystemTimeTablePtr->u8_hours;
    *date   = SystemTimeTablePtr->u8_date ;
    *month  = SystemTimeTablePtr->u8_month;
    *year   = SystemTimeTablePtr->u8_year;

    return(ge_SUCCESS); 
}

/*----------------------------------- ValidateTimeDate ------------------------------------------
  
   ValidateTimeDate  
    Function:           check range of all positions, correct clock time and date if in error
    Input Parameter:    pointer to system time
    Return Type:        char, status
  
   Notes:               all digit positions are BCD, there should never be a field >9
--------------------------------------------------------------------------------------------------*/
sint16 ValidateTimeDate (struct TimeDate *system_time_ptr)
{
sint16 error_code;
uint16 day_of_week;
uint8 month_ms, month_ls;

    error_code = ge_SUCCESS;

//----------------------------------------- Sec -----------------------------------------------------
    if(((system_time_ptr->u8_secs & 0xF0)>> 4) > 0x05){   // shift it right or compare fails due to BCD, must check one digit at a time but in lsb
        error_code = ge_FAIL;                                          //     digit position is bounded by base 10, also hours and date is base 24 and base 31
        system_time_ptr->u8_secs = 0x00;
    }else {
    if((system_time_ptr->u8_secs & 0x0F) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_secs = 0x00;
    }}

//----------------------------------------- Mins ----------------------------------------------------
    if(((system_time_ptr->u8_mins & 0xF0) >> 4) > 0x05){
        error_code = ge_FAIL;
        system_time_ptr->u8_mins = 0x00;
    }else {
    if((system_time_ptr->u8_mins & 0x0F) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_mins = 0x00;
    }}

//----------------------------------------- Hours ---------------------------------------------------
    if(((system_time_ptr->u8_hours & 0xF0) >> 4) > 0x02){     // check that MSD not greater then 20 BCD
        error_code = ge_FAIL;
        system_time_ptr->u8_hours = 0x00;
    }else {
    if(((system_time_ptr->u8_hours & 0xF0) >> 4) == 0x02){    // if in second decade of hours, check that not greater then  23 BCD
        if((system_time_ptr->u8_hours & 0x0F) > 0x03){
            error_code = ge_FAIL;
            system_time_ptr->u8_hours = 0x00;
        }
    }else {
    if((system_time_ptr->u8_hours & 0x0F) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_hours = 0x00;
    }}}

//----------------------------------------- Date ---------------------------------------------------
    if(((system_time_ptr->u8_date & 0xF0) >> 4) > 0x03){      // can't exceed third decade in BCD
        error_code = ge_FAIL;
        system_time_ptr->u8_date = 0x01;
    }else {
    if(((system_time_ptr->u8_date & 0xF0) >> 4) == 0x03){     // can't exceed 31
        if((system_time_ptr->u8_date & 0x0F) > 0x01){
            error_code = ge_FAIL;
            system_time_ptr->u8_date = 0x01;
        }
    }else {
    if((system_time_ptr->u8_date & 0x0F) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_date = 0x01;
    }}}

//----------------------------------------- Month ---------------------------------------------------
    month_ms = (system_time_ptr->u8_month & 0xF0) >> 4;
    month_ls = system_time_ptr->u8_month & 0x0F;
    if(month_ms > 1){
        error_code = ge_FAIL;
        system_time_ptr->u8_month = 0x01;
    }else {
    if(month_ms == 1){
        if(month_ls > 2){
            error_code = ge_FAIL;
            system_time_ptr->u8_month = 0x01;
        }
    }else {
    if(month_ms == 0){
        if(month_ls > 9){
            error_code = ge_FAIL;
            system_time_ptr->u8_month = 0x01;
        }
    }}}

//----------------------------------------- Year ---------------------------------------------------
    if(((system_time_ptr->u8_year & 0xF0) >> 4) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_year = 0x00;
    }else {
    if((system_time_ptr->u8_year & 0x0F) > 0x09){
        error_code = ge_FAIL;
        system_time_ptr->u8_year = 0x00;
    }}

    if(error_code != ge_SUCCESS){                          // rewrite if error detected
        rtcWriteTime(system_time_ptr);
        rtcWriteDate(system_time_ptr);
    }

//----------------------------------------- Day ---------------------------------------------------
                                                        // always set the week day, this will never error
    day_of_week = dow
                      (2000 + system_time_ptr->u8_year,
                       BCDDouble2Hex(system_time_ptr->u8_month),
                       BCDDouble2Hex(system_time_ptr->u8_date));
    day_of_week++;                                      // scale to rtc range which is 1 -7
    system_time_ptr->u8_day = (uint8)day_of_week;

//-------------------------------------------------------------------------------------------------
    return(error_code);
}                           //end-fcn_ValidateTimeDate
                     
/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DispClockStatic ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    
    Function:   Display the Date and Time static outlines

    Input:      pointer to structure of static Date Time
                date: line number to display date on
                      char addr to display date at
                time: line number to display time on
                      char addr to display time at
                font_size
    
    Return:     none
    
    Notes:          
                if date_enabled == ge_YES then date outline displayed
                if time_enabled == ge_YES then time outline displayed
--------------------------------------------------------------------------------------------------*/
void DispClockStatic (struct lcdTimeDateStatic *lcd_time_date_static_ptr,
                      uint8                      time_enabled,             
                      uint8                      date_enabled,
                      uint8  font_size)
{
    if(time_enabled == ge_YES){
        DispTimeStatic
            (lcd_time_date_static_ptr,
             font_size);
    }

    if(date_enabled == ge_YES){
        DispDateStatic
            (lcd_time_date_static_ptr,
             font_size);
    }

    return;
}                                                       //end-fcn_DispClockStatic

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DispClockDyn ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    
    Function:   Display the Date and Time retrieved from SPI RT clock

    Input:      pointer to structure to store Date Time
                date: line number to display date on
                      char addr to display date at
                time: line number to display time on
                      char addr to display time at
                font size
    
    Return:     none
    
    Notes:          
                if date_enabled == ge_YES then date displayed
                if time_enabled == ge_YES then time displayed
--------------------------------------------------------------------------------------------------*/
void DispClockDyn (struct TimeDate          *system_time_ptr, // this is usually System Time
                   struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                   uint8                      time_enabled,             
                   uint8                      date_enabled,
                   uint8  font_size)
{
    if(time_enabled == ge_YES){
        DispTimeStringDyn
            (system_time_ptr,                             // display only therefore use static struct
             lcd_time_date_dyn_ptr,
             font_size);
    }

    if(date_enabled == ge_YES){
        DispDateStringDyn
            (system_time_ptr,
             lcd_time_date_dyn_ptr,
             font_size);
    }

    return;
}                                                       //end-DispClockDyn

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispTimeStringDyn ))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispTimeStringDyn
    Function:           Display real time clock Time that is held in the time buffer

    Input Parameter:    pointer to structure to store Date Time
                        uint8 lcd_clk_time_line,
                        uint8 lcd_clk_time_addr,
                        uint8  font_size
    
    Return Type:        none
    
    Notes:
    
       cnvt the BCD date to an ascii string
       clear the display
       display the time on Line 2

           uint8 TimeBuffer[] =       "17:00.00 00 *  "
                                      0123456789ABCDEF

}
--------------------------------------------------------------------------------------------------*/
void DispTimeStringDyn (struct TimeDate          *system_time_ptr,
                        struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                        uint8  font_size)
{
    if(lcd_time_date_dyn_ptr->MsHours.Addr.FldEnable == ge_YES){
        DispTimeHoursDyn
            (system_time_ptr,
             lcd_time_date_dyn_ptr,
             font_size);
    }

    if(lcd_time_date_dyn_ptr->MsMins.Addr.FldEnable == ge_YES){
         DispTimeMinutesDyn
             (system_time_ptr,
              lcd_time_date_dyn_ptr,
              font_size);
     }

    if(lcd_time_date_dyn_ptr->MsSecs.Addr.FldEnable == ge_YES){
         DispTimeSecondsDyn
             (system_time_ptr,
              lcd_time_date_dyn_ptr,
              font_size);
    }

    return;                                             //end-fcn_DispTimeString
}

/*--------------------------------------------------------------------------------------------------
 (((((((((((((((((((((((((((((((((((( DispDateStringDyn ))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispDateStringDyn
    Function:           Display real time clock Date, displays only date, does not display time
    Input Parameter:    pointer to time date structure
                        uint8 lcd_clk_date_line,
                        uint8 lcd_clk_date_addr,
                        uint8 cursor_status,
                        uint8  font_size
    Return Type:        none
    
    Notes:
       Read the date_from the structure pointed to by caller    
       which converts BCD date to an ascii string
       store the string components in the display buffer
       clear the display

// buffer[] =       "(Thur)Sept/30/02";
//                   0123456789ABCDEF

--------------------------------------------------------------------------------------------------*/
void DispDateStringDyn (struct TimeDate          *system_time_ptr,
                        struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                        uint8  font_size)
{
    if(lcd_time_date_dyn_ptr->WeekDay.Addr.FldEnable == ge_YES){
       DispDateWeekDayDyn
          (system_time_ptr,
           lcd_time_date_dyn_ptr,
           font_size);
    }

    if(lcd_time_date_dyn_ptr->Month.Addr.FldEnable == ge_YES){
       DispDateMonthDyn
          (system_time_ptr,
           lcd_time_date_dyn_ptr,
           grphCURSOR_OFF,
           font_size);
    }

    if(lcd_time_date_dyn_ptr->MsDate.Addr.FldEnable == ge_YES){
       DispDateDateDyn
          (system_time_ptr,
           lcd_time_date_dyn_ptr,
           font_size);
    }

    if(lcd_time_date_dyn_ptr->MsYear.Addr.FldEnable == ge_YES){
       DispDateYearDyn                                  // no interactive input so static struct is used
          (system_time_ptr,
           lcd_time_date_dyn_ptr,
           font_size);
    }


    return;
}                                                       //end-fcn_DispDateString

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispTimeHoursDyn ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

   DispTimeHoursDyn
    Function:           Cnv BCD Hrs to ascii and Display the hours
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void DispTimeHoursDyn (struct TimeDate          *system_time_ptr,
                       struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                       uint8  font_size)
{
uint8 time_buffer_hours[]     =      "17";         // avoid hardcoding buffer sizes

    time_buffer_hours[0]   = ((system_time_ptr->u8_hours & 0xF0)>>4) + '0'; // convert to ascii
    time_buffer_hours[1]   =  (system_time_ptr->u8_hours & 0x0F) + '0'; // convert to ascii

    DispStringNoCursor                                  // display the time template first
       (lcd_time_date_dyn_ptr->MsHours.Addr.Line,
        lcd_time_date_dyn_ptr->MsHours.Addr.Char,         // use dyn ptr for dynamic data because of offset caused with formatting
        time_buffer_hours,
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispTimeHoursDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispTimeMinutesDyn ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispTimeMinutesDyn
    Function:           Display the minutes
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void DispTimeMinutesDyn (struct TimeDate          *system_time_ptr,
                         struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                         uint8  font_size)
{
byte time_buffer_minutes[]     =      "17";      // avoid hardcoding buffer sizes

    time_buffer_minutes[0]   = ((system_time_ptr->u8_mins & 0xF0)>>4) + '0'; // convert to ascii
    time_buffer_minutes[1]   = (system_time_ptr->u8_mins & 0x0F) + '0'; // convert to ascii

    DispStringNoCursor
       (lcd_time_date_dyn_ptr->MsMins.Addr.Line,
        lcd_time_date_dyn_ptr->MsMins.Addr.Char,          // use dyn ptr for dynamic data because of offset caused with formatting
        time_buffer_minutes,  
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispTimeMinutesDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispTimeSecondsDyn ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispTimeSecondsDyn
    Function:           Display the seconds
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void DispTimeSecondsDyn (struct TimeDate          *system_time_ptr,
                         struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                         uint8  font_size)
{
uint8 time_buffer_secs[]     =      "17";         // avoid hardcoding buffer sizes

    time_buffer_secs[0]   = ((system_time_ptr->u8_secs & 0xF0)>>4) + '0'; // convert to ascii
    time_buffer_secs[1]   =  (system_time_ptr->u8_secs & 0x0F) + '0'; // convert to ascii

    DispStringNoCursor
       (lcd_time_date_dyn_ptr->MsSecs.Addr.Line,
        lcd_time_date_dyn_ptr->MsSecs.Addr.Char,          // use dyn ptr for dynamic data because of offset caused with formatting
        time_buffer_secs,     
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispTimeSecondsDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispDateYearDyn )))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispDateYearDyn
    Function:           Display the year
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:              Static is defined to be display only, dynamic is interactive
                        Static displays a string upto ge_NULL, dynamic works with nibbles (per digit)
--------------------------------------------------------------------------------------------------*/
void DispDateYearDyn (struct TimeDate          *system_time_ptr,
                      struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                      uint8  font_size)
{
uint8 year_buffer[]     =      "04";              // avoid hardcoding buffer sizes

    year_buffer[0]   = ((system_time_ptr->u8_year & 0xF0)>>4) + '0'; // convert to ascii
    year_buffer[1]   =  (system_time_ptr->u8_year & 0x0F) + '0'; // convert to ascii

    DispStringNoCursor
       (lcd_time_date_dyn_ptr->MsYear.Addr.Line,  
        lcd_time_date_dyn_ptr->MsYear.Addr.Char,           
        year_buffer,        
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DateBufferYear
}

/*--------------------------------------------------------------------------------------------------
 (((((((((((((((((((((((((((((((((((( DispDateMonthDyn )))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispDateMonthDyn
    Function:           Display the month as 4 char alpha string
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:              The NUM_MONTHS_YEAR, and NUM_CHARS_MONTH each have an extra
                            entry in the table to disregard the index of 0 for the first entry
                        Static is defined to be display only, dynamic is interactive
                        Static displays a string upto ge_NULL, dynamic works with nibbles (per digit)
--------------------------------------------------------------------------------------------------*/
void DispDateMonthDyn (struct TimeDate          *system_time_ptr,
                       struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                       uint8  cursor_status,
                       uint8  font_size)
{
char const month_of_year[13][5] =
    {"nada","Jan ","Feb ","Mar ","Apr ","May ","Jun ",
      "Jul ","Aug ","Sept","Oct ","Nov ","Dec "};
uint8 month[] = "Sept";
uint8 month_hex;

    month_hex = BCDDouble2Hex(system_time_ptr->u8_month);               // cnvt to hex or indexing with go wrong

    strncpy
       ((char *)month,                                  // convert month to alpha string in array
        (char *)(&month_of_year[month_hex][0]),                     // copy into buffer
        sizeof(month) - 1);                     // index into month string array
                                                        // subtract ge_NULL terminator


    DispString
       (lcd_time_date_dyn_ptr->Month.Addr.Line,
        lcd_time_date_dyn_ptr->Month.Addr.Char,
        month,
        ge_DONOT_CLEAR_LINE,
        cursor_status,
        lcd_time_date_dyn_ptr->Month.Addr.Line,
        lcd_time_date_dyn_ptr->Month.Addr.Char,
        font_size);

    return;                                             //end-fcn_DispDateMonthDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispDateWeekDayDyn ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   Disp_Date_Day
    Function:           Display the day of the week as 4 char alpha string
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:              Static is defined to be display only, dynamic is interactive
                        Static displays a string upto ge_NULL, dynamic works with nibbles (per digit)
--------------------------------------------------------------------------------------------------*/
void DispDateWeekDayDyn (struct TimeDate          *system_time_ptr,
                         struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                         uint8  font_size)
{
uint8 date_buffer_weekday[] = "Thur";
int8 DayOfWeek[NUM_DAYS_WEEK][NUM_CHARS_DAY]    =
    {
        "Mon ",
        "Tues",
        "Wed ",
        "Thur",
        "Fri ",
        "Sat ",
        "Sun "
    };
    strncpy
       ((char *)date_buffer_weekday,                    // convert day of week to alpha string
        DayOfWeek[system_time_ptr->u8_day],  
        sizeof(date_buffer_weekday) - 1);                   
                                                        // subtract ge_NULL terminator

    DispStringNoCursor
       (lcd_time_date_dyn_ptr->WeekDay.Addr.Line,  
        lcd_time_date_dyn_ptr->WeekDay.Addr.Char,           
        date_buffer_weekday,        
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispDateWeekDayDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispDateDateDyn ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   DispDateDateDyn
    Function:           Display the date
    Input Parameter:    pointer to system DateTime
                        pointer to structure to store Date Time
                        font size
    Return Type:        none
    
    Notes:              Static is defined to be display only, dynamic is interactive
                        Static displays a string upto ge_NULL, dynamic works with nibbles (per digit)
--------------------------------------------------------------------------------------------------*/
void DispDateDateDyn (struct TimeDate          *system_time_ptr,
                      struct lcdTimeDateDyn    *lcd_time_date_dyn_ptr,
                      uint8  font_size)
{
uint8 date_buffer_date[] = "30";

    date_buffer_date[0]     = ((system_time_ptr->u8_date   & 0xF0)>>4) + '0'; // convert to ascii 
    date_buffer_date[1]     = (system_time_ptr->u8_date   & 0x0F) + '0'; // convert to ascii 

    DispStringNoCursor
       (lcd_time_date_dyn_ptr->MsDate.Addr.Line,  
        lcd_time_date_dyn_ptr->MsDate.Addr.Char,           
        date_buffer_date,        
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispDateDateDyn
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispTimeStatic ))))))))))))))))))))))))))))))))))))))))))))) 
----------------------------------------------------------------------------------------------------
   DispTimeStatic
    Function:           Display the TimeTxtStr flds
    Input Parameter:    Pointer to Static structure, containing the base address (line, addr)
                        as well as the relative offset per field for Hours, Mins, Secs
                        uint8  font_size
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void DispTimeStatic (struct lcdTimeDateStatic *lcd_time_date_static_ptr,
                     uint8  font_size)
{
uint8        t_line, t_char, t_hours[1], t_mins[1], t_secs[1];

    DispStringNoCursor                                  // display hours template
       (lcd_time_date_static_ptr->Hours.Line        ,// line address is not relative 
        lcd_time_date_static_ptr->Hours.Char,        // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->TimeTxtStr.Hours[0]),// this displays string upto first NULL incountered
        ge_DONOT_CLEAR_LINE,
        font_size);

    DispStringNoCursor                                  // display mins template first
       (lcd_time_date_static_ptr->Mins.Line, 
        lcd_time_date_static_ptr->Mins.Char,          // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->TimeTxtStr.Mins[0]),// this displays string upto first NULL incountered
        ge_DONOT_CLEAR_LINE,
        font_size);

    DispStringNoCursor                                  // display secs template first
       (lcd_time_date_static_ptr->Secs.Line         ,// line address is not relative 
        lcd_time_date_static_ptr->Secs.Char,         // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->TimeTxtStr.Secs[0]),// this displays string upto first NULL incountered        
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispTimeStatic
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((((((( DispDateStatic ))))))))))))))))))))))))))))))))))))))))))))) 
----------------------------------------------------------------------------------------------------
   DispDateStatic
    Function:           Display the DateTextStr flds
    Input Parameter:    Pointer to Static structure, containing the base address (line, addr)
                        as well as the relative offset per field for WeekDay, Month, Date, Year
                        uint8  font_size
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void DispDateStatic (struct lcdTimeDateStatic *lcd_time_date_static_ptr,
                     uint8  font_size)
{
    DispStringNoCursor                                  // display the time template first
       (lcd_time_date_static_ptr->WeekDay.Line      ,// line address is not relative 
        lcd_time_date_static_ptr->WeekDay.Char,      // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->DateTxtStr.WeekDay[0]),// this displays string upto first NULL incountered
        ge_DONOT_CLEAR_LINE,
        font_size);

    DispStringNoCursor                                  // display mins template first
       (lcd_time_date_static_ptr->Month.Line, 
        lcd_time_date_static_ptr->Month.Char,        // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->DateTxtStr.Month[0]),// this displays string upto first NULL incountered
        ge_DONOT_CLEAR_LINE,
        font_size);

    DispStringNoCursor                                  // display secs template first
       (lcd_time_date_static_ptr->Date.Line         ,// line address is not relative 
        lcd_time_date_static_ptr->Date.Char,         // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->DateTxtStr.Date[0]),// this displays string upto first NULL incountered        
        ge_DONOT_CLEAR_LINE,
        font_size);

    DispStringNoCursor                                  // display secs template first
       (lcd_time_date_static_ptr->Year.Line         ,// line address is not relative 
        lcd_time_date_static_ptr->Year.Char,         // base + offset within line is absolute LCD address of Hours fld
        &(lcd_time_date_static_ptr->DateTxtStr.Year[0]),// this displays string upto first ge_NULL incountered        
        ge_DONOT_CLEAR_LINE,
        font_size);

    return;                                             //end-fcn_DispTimeStatic
}

/*----------------------------------- TimeDelay ------------------------------------------
  
   TimeDelay  
    Function:           10 millisec interrupt using modulus timer
    Input Parameter:    uint16 multiple of 10ms to wait
    Return Type:        none
  
   Notes:
--------------------------------------------------------------------------------------------------*/
void TimeDelay (uint16 timer_delay)
{
uint16 i, timer_loop_cntr;

    switch(timer_delay){

        case ge_T_10_MS:
            timer_loop_cntr = 1;
        break;

        case ge_T_50_MS:
            timer_loop_cntr = 5;
        break;

        case ge_T_100_MS:
            timer_loop_cntr = 10;
        break;

        case ge_T_500_MS:
            timer_loop_cntr = 50;
        break;

        case ge_T_1_SEC:
            timer_loop_cntr = 100;
        break;

        case ge_T_2_SEC:
            timer_loop_cntr = 200;
        break;

        case ge_T_3_SEC:
            timer_loop_cntr = 300;
        break;

        case ge_T_4_SEC:
            timer_loop_cntr = 400;
        break;

        case ge_T_5_SEC:
            timer_loop_cntr = 500;
        break;

        default:
            timer_loop_cntr = 1;
    }

    for(i = 0; i < timer_loop_cntr; i++){
        TimerDelay();
    }

    return;                 //end-fcn_ 
}

/*----------------------------------- TimerDelay ------------------------------------------
  
   TimerDelay  
    Function:           spin delay
    Input Parameter:    none
    Return Type:        none
  
   Notes:
--------------------------------------------------------------------------------------------------*/
void TimerDelay (void)
{
    TimerDelayFlag = FALSE;                             // mark interrupt pending
    TimerDelay_Enable();                                // Enable bean (timer)
    TimerDelay_EnableEvent();                           // turn on interrupt

    while (TimerDelayFlag == FALSE){};                  // spin unil FLAG set TRUE

    TimerDelay_DisableEvent();                          // turn off interrupt
    TimerDelay_Disable();                               // disable bean (timer)

    return;
}

/*----------------------------------- TimeDelayScript ------------------------------------------
  
   TimeDelayScript  
    Function:           integer mulitple of secs to wait
    Input Parameter:    uint8 multiple secs wait
    Return Type:        none
  
   Notes:               Note this function is totally dependant on enum, if the enum is changed
                        the mapping must be manually changed
                        to remap the script number to a real time delay
--------------------------------------------------------------------------------------------------*/
void TimeDelayScript(uint16 time_delay_EOL)
{
uint16 timer_delay;

    switch(time_delay_EOL)
{
        case ge_ONE_SEC:
            timer_delay = ge_T_1_SEC;
        break;

        case ge_TWO_SEC:
            timer_delay = ge_T_2_SEC;
        break;

        case ge_THREE_SEC:
            timer_delay = ge_T_3_SEC;
        break;

        case ge_FOUR_SEC:
            timer_delay = ge_T_4_SEC;
        break;

        case ge_FIVE_SEC:
            timer_delay = ge_T_5_SEC;
        break;

        default:
            timer_delay = ge_T_1_SEC;
        break;
}

        TimeDelay(timer_delay);

     return;                // end-fcn_TimeDelayScript
}

/*-------------------------swDebounce.c----------------------------------------------------------
    
    Function:           delay for switch bounce
      Input:            none
      Return:             none
      Notes:            hard coded, use TimeDelay bean
--------------------------------------------------------------------------------------------------*/
void swDebounce (void)
{
uint8 cpu_speed;
uint16 i, j, debounce_time = 0x3FFF;


    for (i = 0; i < 4; i++){
        for(j = 0; j < debounce_time; j++){}
    }

    return;
}
//..................................................................................................
//                                              Code Snippets
//..................................................................................................

/*
**  DAYNUM.C - Functions to return statistics about a given date.
**
**  public domain by Bob Stout - uses Ray Gardner's SCALDATE.C
*/


static long jan1date;

/*
**  Determine if a given date is valid
*/

Boolean_T valiDate (unsigned yr, unsigned mo, unsigned day)
{
      unsigned int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

      if (1 > mo || 12 < mo)
            return False_;
      if (1 > day || day > (days[mo - 1] + (2 == mo && isleap(yr))))
            return False_;
      else  return True_;
}

/*
**  Return the day of the week
*/
unsigned dow (unsigned y, unsigned m, unsigned d)
{
      if (m < 3)
      {
            m += 13;
            y--;
      }
      else  m++;
      return (d + 26 * m / 10 + y + y / 4 - y / 100 + y / 400 + ADJ) % 7;
}


/*
**  Return the day of the year (1 - 365/6)
*/

int daynum (int year, int month, int day)
{
      jan1date = ymd_to_scalar(year, 1, 1);
      return (int)(ymd_to_scalar(year, month, day) - jan1date + 1L);
}

/*
**  Return the week of the year (1 - 52, 0 - 52 if ISO)
*/

int weeknum (int year, int month, int day)
{
      int wn, j1n, dn = daynum(year, month, day);

      dn += (j1n = (int)((jan1date - (long)ISO_CAL) % 7L)) - 1;
      wn = dn / 7;
      if (ISO_CAL)
            wn += (j1n < 4);
      else  ++wn;
      return wn;
}

/*
**  Return the phase of the moon (0 -> 7, 0 = new, 4 = full)
*/

char *MoonPhaseText[8] = {"new", "waxing crescent", "first quarter",
      "waxing gibbous", "full", "waning gibbous", "third quarter",
      "waning crescent"
      };

unsigned moonphase (unsigned yr, unsigned mo, unsigned dy)
{
      unsigned long date = (unsigned long)ymd_to_scalar(yr, mo, dy);

      date *= 9693L;
      date /= 35780L;
      date -= 4L;
      date %= 8L;
      return (unsigned)date;
}

/*
** scalar date routines    --    public domain by Ray Gardner
** Numerically, these will work over the range 1/01/01 thru 14699/12/31.
** Practically, these only work from the beginning of the Gregorian 
** calendar thru 14699/12/31.  The Gregorian calendar took effect in
** much of Europe in about 1582, some parts of Germany in about 1700, in
** England and the colonies in about 1752ff, and in Russia in 1918.
*/

int isleap (unsigned yr)
{
   return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);
}

static unsigned months_to_days (unsigned month)
{
   return (month * 3057 - 3007) / 100;
}

static long years_to_days (unsigned yr)
{
   return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}

long ymd_to_scalar (unsigned yr, unsigned mo, unsigned day)
{
   long scalar;
   scalar = day + months_to_days(mo);
   if ( mo > 2 )                         /* adjust if past February */
      scalar -= isleap(yr) ? 1 : 2;
   yr--;
   scalar += years_to_days(yr);
   return scalar;
}

void scalar_to_ymd (long scalar, unsigned *yr, unsigned *mo, unsigned *day)
{
   unsigned n;                /* compute inverse of years_to_days() */

   for ( n = (unsigned)((scalar * 400L) / 146097L); years_to_days(n) < scalar;)
      n++;                          /* 146097 == years_to_days(400) */
   *yr = n;
   n = (unsigned)(scalar - years_to_days(n-1));
   if ( n > 59 ) {                       /* adjust if past February */
      n += 2;
      if ( isleap(*yr) )
         n -= n > 62 ? 1 : 2;
   }
   *mo = (n * 100 + 3007) / 3057;    /* inverse of months_to_days() */
   *day = n - months_to_days(*mo);
}

/*---------------------------------- StartRTClock ----------------------------------------
    Function:           Init ds1305 Alarm0 interrupt on a 1 sec basis 
    Input Parameter:    none
    Return Type:        none
    
    Notes:              Port bit must also be enabled for interrupt to occur and a call to rtcResetAlarm
                             must be made or next interrupt will not occur. Because time is being read from the System
                             Time structure, the SystemTimeTable updates will stop if the interrupt does not occur. See the alarm
                             interrupt service rtn
  ------------------------------------------------------------------------------------------------*/
void rtcStartClock (struct TimeDate *system_time_ptr)
{
sint16 error_code;
uint8  static u8_clock_reg;

    vFn_rtcAlarmOper(ge_RTC_RESET_ALARM_FLAG);
    vFn_rtcAlarmOper(ge_RTC_ALARM_DISABLE_0);
    vFn_rtcCntrl(ge_RTC_ENABLE_OSCILL);
    vFn_rtcCntrl(ge_RTC_CLEAR_OSCILL_FAIL);

    rtcReadDateTime(system_time_ptr);                   // updates global rtc struct with h/w rtc registers
    error_code = ValidateTimeDate(system_time_ptr);     // check if valid digits

    if(error_code == ge_FAIL){
        DispStringNoCursor
           (grphLINE_ONE,
            START_OF_LINE,
            (uint8 *)"rtcReadDateTime err",
            ge_CLEAR_LINE,
            FontSizeCurr);
    }

    return;
} 

/*---------------------------------- rtcEnableOscil ----------------------------------------
rtcEnableOscil
    Function:           starts clock oscillator 
    Input Parameter:    none
    Return Type:        none
    
    Notes:              turns on clock oscillator to allow clock to count
------------------------------------------------------------------------------------------------*/
void vFn_rtcCntrl (uint16 u16_cntrl_state)
{
uint8  static u8_clock_reg;

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup i2c and processor
    u8_clock_reg = vFn_ReadReg_i2c(ge_M41T81S_ALARM_HOURS); // get the current value, only want the clock enable

    if(u16_cntrl_state == ge_RTC_ENABLE_OSCILL){
        u8_clock_reg &= ~(1 << HT_M41T81S);             // clear HT, to enable oscillator
    }else {
    if(u16_cntrl_state == ge_RTC_DISABLE_OSCILL){
        u8_clock_reg |= (1 << HT_M41T81S);              // set HT, to disable oscil
    }else {
    if(u16_cntrl_state == ge_RTC_CLEAR_OSCILL_FAIL){
        u8_clock_reg |= ~(1 << OF_M41T81S);             // clear oscill flag flag
    }}}

    if( u16_cntrl_state == ge_RTC_ENABLE_OSCILL    ||
        u16_cntrl_state == ge_RTC_DISABLE_OSCILL ){
        vFn_WriteReg_i2c
                (ge_M41T81S_ALARM_HOURS,
                 u8_clock_reg);                         // write reg
    }else {
    if( u16_cntrl_state == ge_RTC_CLEAR_OSCILL_FAIL){
        vFn_WriteReg_i2c
                (ge_M41T81S_ALARM_FLAGS,
                 u8_clock_reg);                         // write reg
    }}

    return;
}

/*----------------------------------- rtcReadDateTime ------------------------------------------
  
   rtcReadDateTime  
    Function:           Read time from i2c clock registers, move to general struct
    Input Parameter:    none
    Return Type:        none
  
   Notes:               dow() returns 0 - 6 for the day of the week, but rtc, is 1 - 7, therefore adjust
                        day_of_week for 0 - 6 range; must also do on write of day
--------------------------------------------------------------------------------------------------*/
void rtcReadDateTime (struct TimeDate *system_time_ptr)
{
sint8 day_of_week;

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup bean and processor

    system_time_ptr->u8_secs      = vFn_ReadReg_i2c(ge_M41T81S_SECONDS);
    system_time_ptr->u8_mins      = vFn_ReadReg_i2c(ge_M41T81S_MINUTES);
    system_time_ptr->u8_hours     = vFn_ReadReg_i2c(ge_M41T81S_HOURS);

    day_of_week         = vFn_ReadReg_i2c(ge_M41T81S_DAY);
    --day_of_week;                                      // decrease by one to match dow()
    if(day_of_week < 0){
        day_of_week = 0;
    }
    system_time_ptr->u8_day         = day_of_week;

    system_time_ptr->u8_date        = vFn_ReadReg_i2c(ge_M41T81S_DATE);
    system_time_ptr->u8_month       = vFn_ReadReg_i2c(ge_M41T81S_MONTH);
    system_time_ptr->u8_year        = vFn_ReadReg_i2c(ge_M41T81S_YEAR);

    return;
}                                                       //end-fcn_rtcReadDateTime

/*-----------------------------------rtcWriteTime-------------------------------------------
  
   rtcWriteTime  
    Function:           Writes sequential Time to SPI RAM
    Input Parameter:    ????????????
    Return Type:        ????????????
  
   Notes:   time_mark.u8_secs; 
            time_mark.u8_mins; 
            SystemTimeTablePtr->u8_hours;
  
   
--------------------------------------------------------------------------------------------------*/
void rtcWriteTime (struct TimeDate *system_time_ptr)
{

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup bean and processor

    vFn_WriteReg_i2c
        (ge_M41T81S_SECONDS,
         system_time_ptr->u8_secs);
    vFn_WriteReg_i2c
        (ge_M41T81S_MINUTES,
         system_time_ptr->u8_mins);
    vFn_WriteReg_i2c
        (ge_M41T81S_HOURS,
         system_time_ptr->u8_hours);

    return;
}                                                       //end-fcn_rtcWriteTime

/*----------------------------------- rtcWriteDate -------------------------------------------
    Function:           Writes sequential Date to SPI RAM
    Input Parameter:    ????????????
    Return Type:        ????????????
  
   Notes:   time_mark.u8_date; 
            time_mark.u8_month;
            time_mark.u8_year; 
   
   
--------------------------------------------------------------------------------------------------*/
void rtcWriteDate (struct TimeDate *system_time_ptr)
{
uint8 day_of_week;
                                                        // Write Time 
    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup bean and processor

    vFn_WriteReg_i2c
        (ge_M41T81S_DAY,
         system_time_ptr->u8_day);
    vFn_WriteReg_i2c
        (ge_M41T81S_DATE,
         system_time_ptr->u8_date);
    vFn_WriteReg_i2c
        (ge_M41T81S_MONTH,
         system_time_ptr->u8_month);
    vFn_WriteReg_i2c
        (ge_M41T81S_YEAR,
         system_time_ptr->u8_year);

    return;
}                                                       //end-fcn_rtcWriteDate

/*---------------------------------- vFn_rtcAlarmOper ----------------------------------------
vFn_rtcAlarmOper
    Function:           Enable/Disable Alarm interrupts 
    Input Parameter:    state to set alarm to
    Return Type:        void
    
    Notes:      does not leave the rtc addr ptr pointing to flags register (after seconds)
------------------------------------------------------------------------------------------------*/
void vFn_rtcAlarmOper (uint16     u16_rtc_alarm_state)
{
uint8 u8_clock_reg;
    m41t81s_rtc_union           u_rtc_m41t81s;
    m41t81s_rtc_union_ptr       pu_rtc_m41t81s;

    pu_rtc_m41t81s = &u_rtc_m41t81s;

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup i2c and processor

    vFn_ReadBlock_i2c
        (ge_M41T81S_PTR_ADDR,                           // start address for rtc reg ptr
         sizeof(u_rtc_m41t81s.au8_rtc_buffer),
         &u_rtc_m41t81s.au8_rtc_buffer);

    if(u16_rtc_alarm_state == ge_RTC_ALARM_ENABLE_0){
        pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month |= (1 << AFE_M41T81S);     // set AFE, alarm flag enable
        pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month |= ~(1 << SQWE_M41T81S);   // clear sqwe
        vFn_WriteReg_i2c
                (ge_M41T81S_ALARM_MONTH,
                 pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month);
    }else {
    if(u16_rtc_alarm_state == ge_RTC_ALARM_DISABLE_0){
        pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month |= ~(1 << AFE_M41T81S);    // clear AFE, alarm flag disable
        vFn_WriteReg_i2c
                (ge_M41T81S_ALARM_MONTH,
                 pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month);
    }else {
    if(u16_rtc_alarm_state == ge_RTC_RESET_ALARM_FLAG){
        vFn_ReadReg_i2c(ge_M41T81S_ALARM_FLAGS);
    }}}

    return;
} 

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( vFn_rtcAlarmAction ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:           vFn_rtcAlarmAction
    Input Parameter:    s_rtcAlarmStruct *ps_rtc_alarm
    Return Type:        none
    
    Notes:             Called in events.c on trigger of alarm action
--------------------------------------------------------------------------------------------------*/
void vFn_rtcAlarmAction (s_rtcAlarmStruct *ps_rtc_alarm){
/*   u16Fn_WaveOut
       (ps_rtc_alarm->c_alarm_wav_file,                  // pass pointer to wave file to play
        ge_SD_FILE_STREAM,
        0);                                          // index disregarded by sd card functions
*/
    vFn_ScriptMaster
       ("AlrmClk1.cmd",
        ge_SD_FILE_STREAM,
        0,
        FONT_NINE_DOT);


    return;
}

/*---------------------------------- vFn_rtcReadAlarm ----------------------------------------
    Function:           Read rtc Alarm 
    Input Parameter:    pointer to rtcAlarm struct alarm channel to fill in
    Return Type:        void
    
    Notes:              rtc struct filled in on exit
  ------------------------------------------------------------------------------------------------*/
void vFn_rtcReadAlarm (s_rtcAlarmStruct *ps_rtc_alarm)
{
    m41t81s_rtc_union           u_rtc_m41t81s;
    m41t81s_rtc_union_ptr       pu_rtc_m41t81s;

    pu_rtc_m41t81s  = &u_rtc_m41t81s;                   // point to rtc reg structure

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup bean and processor

    vFn_ReadBlock_i2c
        (ge_M41T81S_PTR_ADDR,                           // start address for rtc reg ptr
         sizeof(u_rtc_m41t81s.au8_rtc_buffer),
         &u_rtc_m41t81s.au8_rtc_buffer);

    ps_rtc_alarm->u8_month = pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_month;
    ps_rtc_alarm->u8_date  = pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_date;
    ps_rtc_alarm->u8_hours = pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_hours;
    ps_rtc_alarm->u8_mins  = pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_minutes;
    ps_rtc_alarm->u8_secs  = pu_rtc_m41t81s->s_rtc.s_rtc_alarm_timedate_struct.u8_alarm_seconds;

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( vFn_rtcAlarmSet ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:               update the flags and fields for the rtc alarm
    Input Parameter:    ptr to alarm block to write
                        prt to alarm time values
                        mode of alarm, SINGLE, or REPEAT
    Return Type:        none
    
    Notes:
--------------------------------------------------------------------------------------------------*/
void    vFn_rtcAlarmTimeSet (s_rtcAlarmStruct       *ps_rtc_alarm,
                             TimeDateStruct         *system_time_table_ptr,
                             uint16                 u16_alarm_mode)
{

    ps_rtc_alarm->u8_month  = system_time_table_ptr->u8_month;
    ps_rtc_alarm->u8_date   = system_time_table_ptr->u8_date;
    ps_rtc_alarm->u8_hours  = system_time_table_ptr->u8_hours;
    ps_rtc_alarm->u8_mins   = system_time_table_ptr->u8_mins;
    ps_rtc_alarm->u8_secs   = system_time_table_ptr->u8_secs;

    if(u16_alarm_mode == ge_RTC_ALARM_ON_SECS){
        ps_rtc_alarm->u8_date  |= ALARM_RPT5_MASK;          // repeat once per second, this is a failure mode
        ps_rtc_alarm->u8_date  |= ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours |= ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  |= ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  |= ALARM_RPT1_MASK;
    }else {
    if(u16_alarm_mode == ge_RTC_ALARM_ON_MIN){
        ps_rtc_alarm->u8_date  |= ALARM_RPT5_MASK;
        ps_rtc_alarm->u8_date  |= ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours |= ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  |= ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  &= ~ALARM_RPT1_MASK;
    }else { 
    if(u16_alarm_mode == ge_RTC_ALARM_ON_HOUR){
        ps_rtc_alarm->u8_date  |= ALARM_RPT5_MASK;
        ps_rtc_alarm->u8_date  |= ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours |= ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  &= ~ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  &= ~ALARM_RPT1_MASK;
    }else {
    if(u16_alarm_mode == ge_RTC_ALARM_ON_DATE){
        ps_rtc_alarm->u8_date  |= ALARM_RPT5_MASK;
        ps_rtc_alarm->u8_date  |= ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours &= ~ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  &= ~ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  &= ~ALARM_RPT1_MASK;
    }else {
    if(u16_alarm_mode == ge_RTC_ALARM_ON_MONTH){
        ps_rtc_alarm->u8_date  |= ALARM_RPT5_MASK;
        ps_rtc_alarm->u8_date  &= ~ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours &= ~ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  &= ~ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  &= ~ALARM_RPT1_MASK;
    }else {
    if(u16_alarm_mode == ge_RTC_ALARM_ON_SINGLE){       // alarms only once
        ps_rtc_alarm->u8_date  &= ~ALARM_RPT5_MASK;
        ps_rtc_alarm->u8_date  &= ~ALARM_RPT4_MASK;
        ps_rtc_alarm->u8_hours &= ~ALARM_RPT3_MASK;
        ps_rtc_alarm->u8_mins  &= ~ALARM_RPT2_MASK;
        ps_rtc_alarm->u8_secs  &= ~ALARM_RPT1_MASK;
    }}}}}}

    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_MONTH,
             ps_rtc_alarm->u8_month);
    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_DATE,
             ps_rtc_alarm->u8_date);
    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_HOURS,
             ps_rtc_alarm->u8_hours);
    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_MINS,
             ps_rtc_alarm->u8_mins);
    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_SECS,
             ps_rtc_alarm->u8_secs);

    vFn_rtcReadAlarm (ps_rtc_alarm);

    return;
}



