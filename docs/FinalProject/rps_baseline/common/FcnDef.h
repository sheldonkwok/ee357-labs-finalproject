#include "PE_Types.h"
#ifndef DEF_FIOFCNDEF_H
    #define DEF_FIOFCNDEF_H

/*===========================================================================

       $Author: r w lewis $

============================================================================*/

#include "DeviceRegStructDef.h"
#include "lcdStructDef.h"
#include "rtcAlarmStructDef.h"
#include "SwitchStructDef.h"

/*-----------------------------FcnDef.h -------------------------------------
//
//  Function:           function prototypes
//  Notes: 
//
//--------------------------------------------------------------------------*/
void    Debug                       (uint16    test_fcn);
void    rpsDemo                     (void);
//--------------------------------------------------------------------------*/

void    vFn_i2cDeviceSetup          (uint16    ge_device_addr);
//--------------------------------------------------------------------------*/

void    vFn_swPoll                  (uint8     *pu8_switch_value,
                                     uint16     u16_sw_poll_mode);
//--------------------------------------------------------------------------*/

void    vFn_ResetMssgOpStatus       (int8       *gua8_zgbBuffer_ptr);

uint16  u16Fn_UpdateMssgOpStatus    (int8       *gua8_zgbBuffer_ptr);
//-----------------------------------------------------------------------
uint16  vFn_ScriptMaster            (char      *cstr_file_ptr,
                                     uint16     u16_input_stream,
                                     uint16     u16_file_index,
                                     uint8      font_size);

//-----------------------------------------------------------------------
void    vFn_flshDeepPwr             (uint16     deep_power_mode);
uint8   u8Fn_flshGetStatusReg       (void);
uint32  u32Fn_flshGetID             (void);
void    vFn_flshTestRead            (void);
void    vFn_flshTestWrite           (void);
uint8   u8Fn_flshStatusRegEnable    (void);
void    vFn_flshChipErase           (void);

void    vFn_flshFileWrite           (uint32      u32_addr, 
                                     uint32      u32_page_size, 
                                     char       *cstr_data_ptr);

uint16  u16Fn_flshFileVerify        (uint32      u32_addr,
                                     uint32      u32_num_bytes,
                                     char       *cstr_data_ptr);

uint16  u16Fn_flshSdFileVerify      (int8       *cstr_file_ptr,
                                     uint16      u16_file_type,
                                     uint16      u16_file_index);

uint32  u32Fn_flshFileRead          (uint32      u32_addr,
                                     uint32      u32_num_bytes,
                                     char       *cstr_data_ptr);

uint16  u16Fn_flshFileEraseVerify   (uint32      u32_addr,
                                     uint32      u32_number_erase_blocks);

uint16  u16Fn_flshFileErase         (uint32      u32_addr,
                                     uint16      u16_file_type);

uint16  u16Fn_flshWriteToFlash      (int8       *cstr_src_ptr,
                                     uint16      u16_file_type,
                                     uint16      u16_file_index);

uint32  u32Fn_flshFileOpen          (uint16      u16_file_index,
                                     uint16      u16_file_type);

void    vFn_flshDeepPwr             (uint16      deep_power_mode);

//-----------------------------------------------------------------------
void    vFn_ScriptExecute           (uint8       font_size)  ;

void    vFn_ScriptExecuteMenu       (struct      lcdMenuCntrl           *lcd_menu_cntrl_ptr,
                                     uint8       font_size);

void    vFn_AlarmFcns               (uint8       font_size);

void    TimeFcns                    (uint8      font_size);

void    TimeMenu                    (struct     lcdTimeDateStatic  *lcd_time_date_static_ptr,
                                     struct     lcdTimeDateDyn     *lcd_time_date_dyn_ptr,
                                     struct     TimeDate           *system_time_ptr,
                                     struct     lcdMenuCntrl       *lcd_menu_cntrl_ptr,
                                     uint16     ge_alarm_dislay_type,
                                     uint8      font_size);

uint8   UpdateWeekDay               (struct     TimeDate           *system_time_ptr,
                                     struct     lcdTimeDateDyn     *lcd_time_date_dyn_ptr,
                                     uint8      font_size);

uint8   MenuNextSelection           (struct     lcdMenuCntrl       *lcd_menu_cntrl_ptrs,
                                     uint16     selection_type);

void    ExitOperationPrompt         (uint8      font_size);

//-----------------------------------------------------------------------
void    rtcClk                      (uint8      font_size);
void    rtcClockDisp                (uint8      font_size);
void    rtcStartClock               (struct     TimeDate           *system_time_ptr);

//-----------------------------------------------------------------------
uint8   u8Fn_RxTx_spi               (uint8      write_byte);
void    rtcWriteTime                (struct     TimeDate           *system_time_ptr);
void    rtcWriteDate                (struct     TimeDate           *system_time_ptr);
void    rtcReadDateTime             (struct     TimeDate            *system_time_ptr);

void    vFn_rtcAlarmTimeSet         (s_rtcAlarmStruct               *ps_rtc_alarm,
                                     struct TimeDate                *lcd_system_time_table_ptr,
                                     uint16                         u16_alarm_mode);

void    vFn_rtcAlarmOper            (uint16                 u16_rtc_alarm_state);
void    vFn_rtcAlarmAction          (s_rtcAlarmStruct               *ps_rtc_alarm);
void    vFn_rtcReadAlarm            (s_rtcAlarmStruct               *ps_rtc_alarm);
//-----------------------------------------------------------------------
void    InitSetup                   (void);
void    Init_sci                    (void);
void    Init_tmr                    (void);
void    spiInit                     (sint16);
void    spiClkPhase                 (sint16);

//-----------------------------------------------------------------------
void    vFn_MssgWrite               (uint8      font_size);
void    vFn_MssgRead                (uint8      font_size);

//-----------------------------------------------------------------------
void    ledIndicator                (uint16     led_state,
                                     uint16     led_color,
                                     uint8      blink_num);
                                                
void    SetOutPut                   (uint8      output_mode,
                                     uint8      lcd_scroll_cntrl);
                                                
void    rtcInterrupt                (uint8      set_mode);
                                                
uint16  u16Fn_WaveOut               (char       *cstr_SoundFile_ptr,
                                     uint16     input_stream,
                                     uint16     flash_index);

//-----------------------------------------------------------------------
uint8   BCDDouble2Hex               (uint8      bcd_num);
uint8   Hex2BCDDouble               (uint8      hex_num);
sint16  ValidateTimeDate            (struct     TimeDate    *system_time_ptr);
                                               
void    Char2AsciiDigits            (uint8      char_2_cnv,
                                     sint8     *destptr,
                                     sint8      cnv_size);
char*   xstrcat                     (char      *des,
                                     char      *src,
                                     ...);
//-----------------------------------------------------------------------
void    swInterruptDisable          (uint8      lft,
                                     uint8      rt,
                                     uint8      up,
                                     uint8      dwn,
                                     uint8      sel);
                                                
void    swInterruptEnable           (uint8      lft,
                                     uint8      rt,
                                     uint8      up,
                                     uint8      dwn,
                                     uint8      sel);
                                                
void    swEnableInterruptInternal   (sint8      switch_name);
void    swDisableInterruptInternal  (sint8      switch_name);
                                                
void    swResetAllFlags             (void);     
                                                
void    swResetFlags                (uint8      lft,
                                     uint8      rt,
                                     uint8      up,
                                     uint8      dwn,
                                     uint8      sel);

void    vFn_swResetAndEnable        (void);
void    WaitOnAnySwitch             (void);
void    swDebounce                  (void);
void    vFn_swSetDebounceTimer      (uint16     ge_timeout_in_ms);

//-------------------------------------------------------------------------------------------
uint8   IncrIndex                   (uint8      incr_index,
                                     uint8      lowest_value,
                                     uint8      highest_value);

uint8   IncrDispOneDigit            (uint8      line_num,
                                     uint8      digit_addr,
                                     uint8      highest_value,
                                     uint8      lowest_value,
                                     uint8      digit_value,
                                     uint16     digit_base,
                                     uint8      font_size);

void    IncrDispDigit               (uint8     *digit_ptr,                  // pointer to hex digit pair
                                     uint8      line_addr,
                                     uint8      char_addr,
                                     uint8      lowest_value,
                                     uint8      highest_value,
                                     uint8      digit_position,
                                     uint8      font_size);

void    IncrDispHoursLSDigit        (uint8     *digit_ptr,                  // pointer to hex digit pair
                                     uint8      line_addr,
                                     uint8      char_addr,
                                     uint8      lowest_value,
                                     uint8      highest_value,
                                     uint8      font_size);

void    IncrDispMonth               (struct     TimeDate        *system_time_ptr, 
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);

void    IncrDispDateMSDigit         (struct     TimeDate        *system_time_ptr,
                                     uint8                   lcd_clk_date_line,
                                     uint8                   lcd_clk_date_addr,
                                     uint8      font_size);

void    IncrDispDateLSDigit         (struct     TimeDate        *system_time_ptr,
                                     uint8                   lcd_clk_date_line,
                                     uint8                   lcd_clk_date_addr,
                                     uint8      font_size); 

uint8   IncrDateCheck               (struct     TimeDate        *system_time_ptr);

//------------------------------------------------------------------------------------------------------
void    lcdDispUpdate               (uint8      curr_ascii_char,
                                     uint8      font_size);
        
void    DispLogo                    (void);
        
void    DispClockStatic             (struct     lcdTimeDateStatic  *lcd_time_date_static_ptr,
                                     uint8                  time_enabled,             
                                     uint8                  date_enabled,
                                     uint8      font_size);

void    vFn_rtcClockDispUpdate      (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);

void    DispClockDyn                (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8                       time_enabled,             
                                     uint8                       date_enabled,
                                     uint8      font_size);
                                                
void    DispTimeStringDyn           (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispDateStringDyn           (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispTimeHoursDyn            (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
        
void    DispTimeMinutesDyn          (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispTimeSecondsDyn          (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispDateYearDyn             (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispDateMonthDyn            (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      cursor_status,
                                     uint8      font_size);
                                                
void    DispDateWeekDayDyn          (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispDateDateDyn             (struct     TimeDate        *system_time_ptr,
                                     struct     lcdTimeDateDyn  *lcd_time_date_dyn_ptr,
                                     uint8      font_size);
                                                
void    DispTimeStatic              (struct     lcdTimeDateStatic  *lcd_time_date_static_ptr,
                                     uint8      font_size);
                                                
void    DispDateStatic              (struct     lcdTimeDateStatic  *lcd_time_date_static_ptr,
                                     uint8      font_size);
//----------------------------------------------------------------------
void    DispPromptExit              (uint8      lft, 
                                     uint8      rt,   
                                     uint8      up,   
                                     uint8      dwn, 
                                     uint8      sel, 
                                     uint8      font_size); 
//----------------------------------------------------------------------
void    DispChar                    (uint8      disp_char);
        
void    DispCursorOff               (void);
void    DispCursor                  (uint8      lcd_line_num,
                                     uint8      lcd_char_addr,
                                     uint8      cursor_cntrl,
                                     uint8      font_size,
                                     uint8      string_length);
                                              
void    DispString                  (uint8      lcd_line_num,
                                     uint8      lcd_char_addr,
                                     uint8     *buffer,
                                     uint8      clear_line,
                                     uint8      cursor_cntrl,
                                     uint8      cursor_line_num,
                                     uint8      cursor_addr,
                                     uint8      font_size);
                                              
void    DispStringNoCursor          (uint8      lcd_line_num,
                                     uint8      lcd_char_addr,
                                     uint8     *buffer,
                                     uint8      clear_line,
                                     uint8       font_size);
                                              
void    grphClrLine                 (uint8      lcd_line_num,
                                     uint8      font_size);
                                              
void    DispBuffLine                (uint8      lcd_line_num,
                                     uint8      lcd_char_addr,
                                     uint8     *buffer,
                                     uint8      font_size);
                                              
void    DispAndIncrAddr             (char       curr_ascii_char,
                                     uint8      scroll_mode,
                                     uint8      font_size,
                                     uint16     pause_time,
                                     uint16     time_delay_EOL,
                                     uint8      new_page);
                                               
uint8   DispPrompt                  (uint8      lft, 
                                     uint8      rt,  
                                     uint8      up,  
                                     uint8      dwn, 
                                     uint8      sel, 
                                     uint8      line_addr,
                                     uint8      char_addr,
                                     uint8      ascii_char1,
                                     uint8      ascii_char2,
                                     uint8     *current_char_ptr,
                                     uint8      font_size); 
        
void    DispSetCharAddr             (uint8      lcd_line_num,
                                     uint8      lcd_char_addr);
                                                
uint8   lcdIncrLineAddr             (uint8      scroll_mode,
                                     uint8      font_size);
                                                
void    DispInform                  (uint8      font_size);
                                              
void    DispInformPage              (uint8     *message_ptr,
                                     uint8      font_size);
                                              
void    vFn_DispLineSet             (uint8     *mssg_set_ptr,
                                     uint8      line_length,
                                     uint8      font_size,
                                     uint8      clear_screen);

//----------------------------------------- --------------------------------
sint8   lcdBusyCheck                (void); 
void    lcdWriteCmdWithBusyCheck    (uint8      lcd_cmnd);
void    lcdWriteCmd                 (uint8      lcd_cmnd);
void    lcdWriteCmdWithDelay        (uint8      lcd_cmnd,
                                     uint8      cmnd_delay);
//-------------------------------------------------------------------------
void    TimeDelay                   (uint16     timer_delay);
void    TimerDelay                  (void);    
void    TimeDelayScript             (uint16     time_delay_EOL);

//-------------------------------------------------------------------------
void    vFn_grphClrView             (uint8      font_size);
void    grphOutline                 (void);    
                                               
void    grphUpdate                  (uint8      top,
                                     uint8      bottom);
                                               
uint8   u8Fn_grphSetFont            (uint8      font_size);
        
//-------------------------------------------------------------------------
void    Print_iMn                   (char       *print_char);
void    DispTest                    (uint8      loop_mode);

void    vFn_StringToUpperCase       (char       *input_string,
                                     uint16     string_size);
//-------------------------------------------------------------------------
void    vFn_TempSensor              (ts_lm75_union              *pu_ts_lm75);
void    vFn_InitAccel               (uint16     u16_accel_scale);

void    vFn_ReadAccelData           (uint8                      u8_reg_addr,
                                     uint16                     u16_accel_block_size,
                                     uint8                      *pu8_accel_block);

uint16  u16Fn_CalibAccel            (uint16     u16_accel_gforce,
                                     uint16     u16_accel_axis);

#endif
