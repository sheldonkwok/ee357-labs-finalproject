#define DEMO_TEST                               1             // enable test functions,
//#define RPS_SPIFLASH_OPT_LIB                    1             // enable optional functions,
//#define RPS_LCDDISPFCNS_OPT_LIB                 1             // enable optional functions,
//#define RPS_ACCELEROMETER_FCNS_OPT_LIB          1             // enable optional functions,
//#define LCD_MENU_ON                             1             // define if LCD menu with nav switch desired, requires optional functions
//#define RTC_ALARM_SET                           1             // requires optional functions
//#define DISP_RTC_ALARM_INSTRUCTIONS         1
//#define ge_FLASH_FILE_ERASE_VERIFY          1                 // verify's erase of flash file
//#define ge_FLASH_FILE_VERIFY                1                 // verify file data
//#define DISP_LCD_BYPASS                 1                     // requires optional functions, bypass LCD DispString, DispStringNoCursor
#define DISP_PAGE_SET                   0x40                  // indicator of block size to display when flashing
//iMn  #define NAV_SW_INTERRUPT                    1            // define if nav switch uses kbi interrupts, else use polled
#define NAV_SW_POLLED                    1                    // define if nav switch polled

//==================================================================================================
//          Test Fcns in rpsDemo.c
//==================================================================================================
//#define BYPASS_QUERY_PROMPT                     1             // display prompt will always return a 'Y' if defined
#define RPS_FM1_9_1_2_TEST_A                  1
#define DEBUG_LOOP_CNTRL                        ge_INFINITE_LOOP  // must define one or the other
//#define DEBUG_LOOP_CNTRL                        ge_1_TIME
#define	HW_i2c_RTC_M41T81S                      1
//#define ADDR_i2c_RTC_BQ32000                   1
#define ADDR_i2c_RTC_M41T81S                   1
//#define TIMEOUT_TIMER_ENABLE                1

// Lower level tests; falls through or loops depending on DEBUG_LOOP_CNTRL

//#define GRPH_LINES_TEST                     1
//#define DISP_TEST                           1
//#define LED_TEST                            1
//#define DISP_BKLITE                         1
//#define SPI_SIGNAL_TEST                     1
//#define FLASH_ID_TEST                       1
//#define FLASH_STATUS_TEST                   1
//#define FLASH_ERASE_TEST                    1
//#define FLASH_RW_TEST                       1
//#define FLASH_SD_WRITE_TEST                 1
//#define FLASH_SD_VERIFY_TEST                1
//#define SD_WAVE_TEST                        1
//#define SD_FLASH_TEST                       1
//#define SCRIPT_TEST                         1
//#define RTC_TEST                            1
//#define RTC_ALARM_TEST                      1
//#define FILE_SPEED_TEST                     1

