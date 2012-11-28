#ifndef DEF_ENUMDEF_H
    #define DEF_ENUMDEF_H
    
//    #include "fonts.h"

/*============================================================================================================
                                $RCSfile: EnumDef.h,v $

       $Author: blewis $
       $Revision: 1.23 $
       $Date: 2006/06/20 03:11:52 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/EnumDef.h,v $

=============================================================================================================*/
//((((((((((((((((((((((((((((((( enum declarations ))))))))))))))))))))))))))))))))))))))))))))))))
enum MenuExecutionOrder
{
#ifdef RTC_ALARM_SET
    ge_RTC_ALARM_SET,
#endif
    ge_RTC_CLOCK,
    ge_RTC_TIME_SET,
    ge_SCRIPT_EXECUTE,
    ge_SD_CARD_WRITE,
    ge_LAST_MAIN_MENU_SEL
};

enum swPolledOper
{
    ge_WAIT_ON_SW              = 0x01,
    ge_READ_ONCE
};

enum
{
    ge_i2c_ON_RECEIVE_DATA     = 1,
    ge_i2c_ON_TRANSMIT_DATA,
    ge_i2c_ON_BYTE_TRANSFER,
    ge_i2c_ON_ARBIT_LOST,
    ge_i2c_ON_NACK,
    ge_i2c_NO_INTERRUPT,
    ge_i2c_ON_RX,
    ge_i2c_ON_TX,
    ge_i2c_ON_READ,
    ge_i2c_ON_WRITE,
    ge_i2c_MSSG_RX_SUCCESS,
    ge_i2c_MSSG_TX_SUCCESS,
    ge_i2c_MASTER_CMD_SUCCESS,
    ge_i2c_MASTER_CMD_FAIL,
    ge_i2c_TX_FAIL
};

enum
{
    ge_I2C_MASTER,
    ge_I2C_SLAVE
};

enum
{
    ge_MSSG_RX_SUCCESS      = 0x1,
    ge_MSSG_RX_FAIL,
    ge_FAIL_NOTIFY_COUNT,
    ge_MSSG_TX_SUCCESS,
    ge_MSSG_TX_FAIL,
    ge_MASTER_CMD_SUCCESS,
    ge_MASTER_CMD_FAIL,
    ge_SLAVE_CMD_SUCCESS,
    ge_PE_TIMER_TIMED_OUT
};

enum i2c_device_addr_list                               // must be unique or will cause problems
{
    ge_TEMP_LM75_ADDR                   = 0x49,         // Real address is 0x92, but PE shifts it left 1 place before use ???!!!
    ge_M41T81S_ADDR                     = 0x68,         // Real address is 0xD0, but PE shifts it left 1 place
    ge_MMA7455L_ADDR                    = 0x1D
//rwl      ge_BQ32000_ADDR                     = 0x68,         // Real address is 0xD0, but PE shifts it left 1 place
//rwl      ge_ISL1218_ADDR                     = 0x6F,         // Intersil real time clock
};

typedef enum i2c_TS_LM75_reg_values
{
    ge_TS_LM75_TEMP_ADDR                = 0x00,
    ge_TS_LM75_CONFIG_ADDR              = 0x01,
    ge_TS_LM75_THYST_ADDR               = 0x02,
    ge_TS_LM75_TOS_ADDR                 = 0x03
};


enum
{
    ge_CAMERA_ON                = 0x31,
    ge_CAMERA_OFF,

    ge_SET_CAPTURE_FRAMES_30,
    ge_SET_CAPTURE_FRAMES_60,
    ge_SELECT_CAMERA_1,
    ge_SELECT_CAMERA_2,

    ge_SET_BASE_NAME_CF,
    ge_SET_BASE_INDEX_CF,
    ge_OPEN_CF,
    ge_CLOSE_CF,
    ge_DELETE_CF,

    ge_SET_BASE_NAME_DS,
    ge_SET_BASE_INDEX_DS,
    ge_OPEN_DS,
    ge_CLOSE_DS,
    ge_DELETE_DS,

    ge_SET_TIME_DC,
    ge_SET_DATE_DC,

    ge_SEND_STATUS_DC,
    ge_DISP_STRING_DC,
    ge_RESET_DC,
    ge_SHUTDOWN_DC,
    ge_STORE_DATA_DS,
    ge_RECORD_ON,
    ge_RECORD_OFF,
    ge_LCD_ON,
    ge_LCD_OFF
};                      

typedef enum debug_list
{   
    ge_DEBUG_RS232_RTC                    = 0x01,
    ge_DEBUG_BQ32000,
    ge_DEBUG_M41T81S,
    ge_DEBUG_LBT_MASTER,
    ge_DEBUG_LBT_SLAVE,
    ge_DEBUG_MMA7455L,
    ge_DEBUG_SWITCH_KBI,
    ge_DEBUG_ACCEPTANCE_TEST_MASTER,
    ge_DEBUG_ACCEPTANCE_TEST_SLAVE
};

enum m41t81s_ptr_addr
{
    ge_M41T81S_PTR_ADDR                 = 0x00
};

typedef enum i2c_ISL1208_reg_values
{
    ge_ISL1208_RESET_RTCF               = 0x00
};

enum rtc_fcns
{
    ge_RTC_ENABLE_OSCILL,
    ge_RTC_DISABLE_OSCILL,
    ge_RTC_CLEAR_OSCILL_FAIL,
    ge_RTC_ALARM_ENABLE_0,                              // constant is overloaded, 
    ge_RTC_ALARM_DISABLE_0
};

typedef enum i2c_rtc_reg_addr_m41t81s
{
    ge_M41T81S_SUB_SECONDS              = 0x00,         // register address offsets, do not change
    ge_M41T81S_SECONDS                  = 0x01,
    ge_M41T81S_MINUTES                  = 0x02,
    ge_M41T81S_HOURS                    = 0x03,
    ge_M41T81S_DAY                      = 0x04,
    ge_M41T81S_DATE                     = 0x05,
    ge_M41T81S_MONTH                    = 0x06,
    ge_M41T81S_YEAR                     = 0x07,
    ge_M41T81S_CALIBRATION              = 0x08,
    ge_M41T81S_WATCHDOG                 = 0x09,
    ge_M41T81S_ALARM_MONTH              = 0x0A,
    ge_M41T81S_ALARM_DATE               = 0x0B,
    ge_M41T81S_ALARM_HOURS              = 0x0C,
    ge_M41T81S_ALARM_MINS               = 0x0D,
    ge_M41T81S_ALARM_SECS               = 0x0E,
    ge_M41T81S_ALARM_FLAGS              = 0x0F,
    ge_M41T81S_ALARM_RESERVED1          = 0x10,
    ge_M41T81S_ALARM_RESERVED2          = 0x11,
    ge_M41T81S_ALARM_RESERVED3          = 0x12,
    ge_M41T81S_ALARM_SQUARE_WAVE        = 0x13
};

typedef enum i2c_BQ32000_reg_address
{
    ge_BQ32000_SECONDS                  = 0x00,
    ge_BQ32000_MINUTES                  = 0x01,
    ge_BQ32000_CENT_HOURS               = 0x02,
    ge_BQ32000_DAY                      = 0x03,
    ge_BQ32000_DATE                     = 0x04,
    ge_BQ32000_MONTH                    = 0x05,
    ge_BQ32000_YEAR                     = 0x06
};

typedef enum i2c_MMA7455_reg_address           
{                                      
    ge_MMA7455_XOUTL                   = 0x00,
    ge_MMA7455_XOUTH                   = 0x01,
    ge_MMA7455_YOUTL                   = 0x02,
    ge_MMA7455_YOUTH                   = 0x03,
    ge_MMA7455_ZOUTL                   = 0x04,
    ge_MMA7455_ZOUTH                   = 0x05,
    ge_MMA7455_XOUT8                   = 0x06,
    ge_MMA7455_YOUT8                   = 0x07,
    ge_MMA7455_ZOUT8                   = 0x08,
    ge_MMA7455_STATUS                  = 0x09,
    ge_MMA7455_DETSRC                  = 0x0A,
    ge_MMA7455_TOUT                    = 0x0B,
    ge_MMA7455_RESERVE1                = 0x0C,
    ge_MMA7455_I2CAD                   = 0x0D,
    ge_MMA7455_USRINF                  = 0x0E,
    ge_MMA7455_WHOAMI                  = 0x0F,
    ge_MMA7455_XOFFL                   = 0x10,
    ge_MMA7455_XOFFH                   = 0x11,
    ge_MMA7455_YOFFL                   = 0x12,
    ge_MMA7455_YOFFH                   = 0x13,
    ge_MMA7455_ZOFFL                   = 0x14,
    ge_MMA7455_ZOFFH                   = 0x15,
    ge_MMA7455_MCTL                    = 0x16,                  // mode control reg 
    ge_MMA7455_INTRST                  = 0x17,
    ge_MMA7455_CTL1                    = 0x18,
    ge_MMA7455_CTL2                    = 0x19,
    ge_MMA7455_LDTH                    = 0x1A,
    ge_MMA7455_PDTH                    = 0x1B,
    ge_MMA7455_PW                      = 0x1C,
    ge_MMA7455_LT                      = 0x1D,
    ge_MMA7455_TW                      = 0x1E,
    ge_MMA7455_RESERVE2                = 0x1F
};

typedef enum MMA_7455_value_list
{
    ge_MMA7455_GFORCE_2G                = 0x05,                 // for SENS_2g
    ge_MMA7455_GFORCE_4G                = 0x09,                 // for SENS_4g
    ge_MMA7455_GFORCE_8G                = 0x01,                 // for SENS_8g      // uses 10 bit data
    ge_MMA7455L_X_AXIS                  = 0x02,                 // these are not reg addr, arbitrary but unique enum
    ge_MMA7455L_Y_AXIS                  = 0x03,
    ge_MMA7455L_Z_AXIS                  = 0x04,
    ge_ACCEL_DATA_SIZE                  = 0x06,                 // size of accel data x,y,z
    ge_ACCEL_BLOCK_SIZE                 = 0x20                  // size of accel full data block
};

enum MMA_7455_addr_list
{
    ge_MMA7455_MTCL_ADDR                = 0x01,                 // control reg addr
    ge_MMA7455L_X_CAL_LOW               = 0X10,
    ge_MMA7455L_X_CAL_HIGH              = 0X11,
    ge_MMA7455L_Y_CAL_LOW               = 0X12,
    ge_MMA7455L_Y_CAL_HIGH              = 0X13,
    ge_MMA7455L_Z_CAL_LOW               = 0X14,
    ge_MMA7455L_Z_CAL_HIGH              = 0X15,
    ge_MMA7455L_i2c_ADDR                = 0x1D                  // MMA7455L address
};

enum debug_fcn_list
{
    ge_SPI_SIGNAL_TEST,
    ge_GRPH_LINES_TEST,
    ge_LED_TEST,
    ge_DISP_BKLITE,
    ge_DISP_TEST,
    ge_FLASH_ID_TEST,
    ge_FLASH_STATUS_TEST,
    ge_FLASH_ERASE_TEST,
    ge_FLASH_RW_TEST,
    ge_FLASH_SD_WRITE_TEST,
    ge_FLASH_SD_VERIFY_TEST,
    ge_SD_WAVE_TEST,
    ge_SD_FLASH_TEST,
    ge_SCRIPT_TEST,
    ge_DEF_DISPINFORM,
    ge_LOGO_ON,
    ge_RTC_TEST,
    ge_RTC_ALARM_TEST,
    ge_FILE_SPEED_TEST,
    ge_ZIGBEE_RX_TX_TEST,
    ge_DEBUG_NUM_ENTRIES                                // number of entries in list
};

enum debug_loop_cntrl
{
    ge_INFINITE_LOOP,
    ge_1_TIME,
    ge_10_TIMES,
    ge_100_TIMES,
    ge_1000_TIMES
};

enum MenuSelectionStart
{   ge_FIRST_MENU_SEL = 0                              // specify starting point of menu list, list below defaults to 0 as first entry
};

enum ScriptExecuteList
{
    ge_SCRIPT_MS_DIGIT,
    ge_SCRIPT_LS_DIGIT,
    ge_SCRIPT_SAVE,
    ge_LAST_SCRIPT_MENU_SEL
};

enum TimeFcnsList
{
    ge_TIME_HOURS_MS_DIGIT,
    ge_TIME_HOURS_LS_DIGIT,
    ge_TIME_MINUTES_MS_DIGIT,
    ge_TIME_MINUTES_LS_DIGIT,
    ge_TIME_SECONDS_MS_DIGIT,
    ge_TIME_SECONDS_LS_DIGIT,
    ge_DATE_MONTH,
    ge_DATE_DATE_MS_DIGIT,
    ge_DATE_DATE_LS_DIGIT,
    ge_DATE_YEAR_MS_DIGIT,
    ge_DATE_YEAR_LS_DIGIT,
    ge_TIME_DATE_SAVE,
    ge_LAST_TIME_MENU_SEL
};

enum TimeDispOptions
{
    ge_ALARM_DISPLAY,
    ge_TIME_DATE_DISPLAY,
    ge_VALID,
    ge_INVALID,
    ge_TIME_ENABLED,
    ge_DATE_ENABLED
};

enum rtcClkFcns
{
    ge_RTC_CLOCK_DISP,
    ge_LAST_RTCCLK_MENU_SEL
};

enum MssgProtocol
{
    ge_MSSG_HANDSHAKE_RESET = 0x01,                         // start at 1
    ge_SLAVE_WAKEUP,            // 2
    ge_SLAVE_WAKEUP_ACK,        // 3
    ge_SLAVE_DATA,              // 4
    ge_SLAVE_DATA_ACK,          // 5
    ge_LINE_FINISHED,           // 6
    ge_LINE_FINISHED_ACK,       // 7
    ge_TX_FINISHED,             // 8
    ge_TX_FINISHED_ACK,         // 9
    ge_SLAVE_TERM,              // 10
    ge_RX_TIME_OUT,             // 11
    ge_NO_RX_STATUS,            // 12
    ge_UNSOLICITED_FRAME_RCVD,  // 13
    ge_RX_FRAME,                // 14
    ge_USER_ABORT               // 15
};

enum MssgProtocolByteSeq
{
    ge_MSSG_CNTRL_INDEX = 0
};

enum sdWriteFcns
{
    ge_SD_WRITE_TEST,
    ge_LAST_SDWRITE_MENU_SEL
};

enum sdReadFcns
{
    ge_SD_READ_TEST,
    ge_LAST_SDREAD_MENU_SEL
};

enum
{
    ge_CHAR_ADDR_OK,                // 0
    ge_CHAR_ADDR_EXCEEDED,          // 1
    ge_IGNORE_CHAR,                 // 2
    ge_CR_FOUND,                    // 3
    ge_YES,                         // 4
    ge_NO,                          // 5
    ge_SUCCESS,                     // 6
    ge_FAIL,                        // 7
    ge_TRUE,
    ge_FALSE,
    ge_FAIL_FILE_OPEN,              // 8
    ge_FAIL_FILE_SUCCESS            // 9
};

enum OutPutType
{
    ge_FILE_PRT,
    ge_GRPH_LCD,
    ge_STDIO
};

enum SampleType
{
    ge_MONO_8_12K   = 0,
    ge_MONO_8_16K   = 1,
    ge_MONO_8_22K   = 2
};

enum ledStateList
{
    ge_LED_ON,
    ge_LED_OFF,
    ge_LED_BOTH,
    ge_LED_BOTH_ON,
    ge_LED_BOTH_OFF,
    ge_LED_TOGGLE,
    ge_LED_GREEN,
    ge_LED_RED
};

enum TimeDelayScriptList
{
    ge_ONE_SEC      =  1,
    ge_TWO_SEC      =  2,
    ge_THREE_SEC    =  3,
    ge_FOUR_SEC     =  4,
    ge_FIVE_SEC     =  5,
    ge_SIX_SEC      =  6,
    ge_SEVEN_SEC    =  7,
    ge_EIGHT_SEC    =  8,
    ge_NINE_SEC     =  9
};

enum
{
    ge_SD_SPI_DEV   = 1, 
    ge_RTC_SPI_DEV  = 2
}; 

enum FcnCallList
{
    ge_LCD_CLEAR,
    ge_FLASH_FILE,
    ge_TURN_PLL_ON,
    ge_TURN_PLL_OFF,
    ge_PRINT_AUDIT_ON,
    ge_PRINT_AUDIT_OFF,
    ge_PRINT_SERIAL,
    ge_PRINT_LCD,
    ge_LEDGREEN_ON,
    ge_LEDGREEN_OFF,
    ge_LEDRED_ON,
    ge_LEDRED_OFF,
    ge_TIME_DELAY,
    ge_LCDBKLITE_OFF,
    ge_LCDBKLITE_ON,
    ge_SOUND_FILE,
    ge_COMMENT,
    ge_WAIT_ON_KEY,
    ge_DISP_CLOCK,
    ge_DISP_TIME_AND_DATE,
    ge_SET_RTC_ALARM,
    ge_RTC_INT_ALARM0,
    ge_RTC_INT_ALARM1,
    ge_RTC_ALARM_BOTH,
    ge_RTC_ALARM_OFF,
    ge_RTC_ALARM_ON,
    ge_RTC_ALARM_ON_SECS,
    ge_RTC_ALARM_ON_MIN,
    ge_RTC_ALARM_ON_HOUR,
    ge_RTC_ALARM_ON_DATE,
    ge_RTC_ALARM_ON_MONTH,
    ge_RTC_ALARM_ON_YEAR,
    ge_RTC_ALARM_ON_SINGLE,
    ge_RTC_ALARM_ENABLE,
    ge_RTC_ALARM_DISABLE,
    ge_RTC_ALARM_SINGLE,
    ge_RTC_ALARM_REPEAT,
    ge_READ_RTC_ALARM,
    ge_RTC_RESET_ALARM_FLAG,
    ge_RTC_ALARM_SCRIPT_ERROR,
    ge_DO_NOTHING 
};

enum WaveInputStreamList
{
    ge_SD_FILE_STREAM,
    ge_FLASH_MEMORY_STREAM
};

enum
{
    ge_WAV_FILE,
    ge_CMD_FILE,
    ge_DAT_FILE
};

enum
{
    ge_I2C_RX_IN_PROGRESS,
    ge_I2C_RX_COMPLETE,
    ge_I2C_TX_IN_PROGRESS,
    ge_I2C_TX_COMPLETE
};

enum
{
    ge_TX_INPROGRESS_SPI,
    ge_TX_COMPLETE_SPI
};

enum
{
    ge_RTC_ALARM_INT_RESET,
    ge_RTC_ALARM_INT_SET
};

enum
{
    ge_TMR_NO_TIMEOUT            = 0,
    ge_TMR_TIMEOUT_10_MS         = 1,
    ge_TMR_TIMEOUT_20_MS         = 2,
    ge_TMR_TIMEOUT_30_MS         = 3,
    ge_TMR_TIMEOUT_40_MS         = 4,
    ge_TMR_TIMEOUT_50_MS         = 5,
    ge_TMR_TIMEOUT_60_MS         = 6,
    ge_TMR_TIMEOUT_70_MS         = 7,
    ge_TMR_TIMEOUT_80_MS         = 8,
    ge_TMR_TIMEOUT_90_MS         = 9,
    ge_TMR_TIMEOUT_100_MS        = 10,
    ge_TMR_TIMEOUT_120_MS        = 12,
    ge_TMR_TIMEOUT_140_MS        = 14,
    ge_TMR_TIMEOUT_160_MS        = 16,
    ge_TMR_TIMEOUT_180_MS        = 18,
    ge_TMR_TIMEOUT_200_MS        = 20,
    ge_TMR_TIMEOUT_300_MS        = 30,
    ge_TMR_TIMEOUT_400_MS        = 40,
    ge_TMR_TIMEOUT_500_MS        = 50,
    ge_TMR_TIMEOUT_600_MS        = 60,
    ge_TMR_TIMEOUT_1_SEC         = 100,
    ge_TMR_TIMEOUT_2_SEC         = 200,
    ge_TMR_TIMEOUT_3_SEC         = 300,
    ge_TMR_TIMEOUT_10_SEC        = 1000,
    ge_TMR_TIMEOUT_20_SEC        = 2000,
    ge_TMR_TIMEOUT_30_SEC        = 3000
};

enum
{
    ge_T_NO_TIMEOUT    = 0,
    ge_T_10_MS         = 1,
    ge_T_50_MS         = 5,
    ge_T_100_MS        = 10,
    ge_T_150_MS        = 15,
    ge_T_200_MS        = 20,
    ge_T_250_MS        = 25,
    ge_T_300_MS        = 30,
    ge_T_350_MS        = 35,
    ge_T_400_MS        = 40,
    ge_T_450_MS        = 45,
    ge_T_500_MS        = 50,
    ge_T_600_MS        = 60,
    ge_T_1_SEC         = 100,
    ge_T_2_SEC         = 200,
    ge_T_3_SEC         = 300,
    ge_T_4_SEC         = 400,
    ge_T_5_SEC         = 500
};

enum
{
    ge_SW_DEBOUNCE_NO_TIMEOUT    = 0,
    ge_SW_DEBOUNCE_50_MS         = 50,
    ge_SW_DEBOUNCE_100_MS        = 10,
    ge_SW_DEBOUNCE_150_MS        = 15,
    ge_SW_DEBOUNCE_200_MS        = 20,
    ge_SW_DEBOUNCE_250_MS        = 25,
    ge_SW_DEBOUNCE_300_MS        = 30,
    ge_SW_DEBOUNCE_350_MS        = 35,
    ge_SW_DEBOUNCE_400_MS        = 40,
    ge_SW_DEBOUNCE_450_MS        = 45,
    ge_SW_DEBOUNCE_500_MS        = 50,
    ge_SW_DEBOUNCE_600_MS        = 60,
    ge_SW_DEBOUNCE_1_SEC         = 100,
    ge_SW_DEBOUNCE_2_SEC         = 200
};

enum
{
    ge_NULL = 0,
    ge_BASE_ASCII,
    ge_BASE_BINARY
};

enum swList
{
    ge_SW_LFT,
    ge_SW_RT,
    ge_SW_UP,
    ge_SW_DOWN,
    ge_SW_SEL,
    ge_SW_INT_ENABLE,
    ge_SW_INT_DISABLE,
    ge_SW_FLAG_SET,
    ge_SW_FLAG_RESET,
    ge_SW_DEBOUNCE_ACTIVE,
    ge_SW_DEBOUNCE_COMPLETE,
    ge_PE_TIMER_ACTIVE,
    ge_PE_TIMER_COMPLETE
};

enum swValue                                            // manually defined, align to port definitions
{
    ge_SWLFT                = 0xE1,
    ge_SWRT                 = 0xE4,
    ge_SWUP                 = 0xE2,
    ge_SWDWN                = 0xE8,
    ge_SWSEL                = 0xF0
};

enum lcdDispList
{
    ge_CONT_MENU,
    ge_EXIT_MENU,
    ge_BYPASS_FIELD,
    ge_NORMAL_FIELD,
    ge_LCD_LINE_SCROLL_ON,
    ge_LCD_LINE_SCROLL_OFF,
    ge_CLEAR_LINE,
    ge_DONOT_CLEAR_LINE,
    ge_CLEAR_SCREEN,
    ge_DONOT_CLEAR_SCREEN,
    ge_ON,
    ge_OFF
};

#endif

