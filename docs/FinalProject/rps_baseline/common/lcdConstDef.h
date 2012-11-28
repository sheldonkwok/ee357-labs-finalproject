#ifndef LCD_TIME_DATE_CONST_DEF_H
    #define LCD_TIME_DATE_CONST_DEF_H

//--------------------------------------------------------------------------------------------------
    #define STATUS_LINE                                     grphLINE_TWO // can redefine for other positons if desired
    #define TITLE_LINE                                      grphLINE_ONE // can redefine for other positons if desired
    #define TITLE_ADDR                                      START_OF_LINE
    #define CURSOR_ON                                       0x0E // Display: on. Cursor: on. Blink: on
    #define CURSOR_OFF                                      0x0C // Display: on. Cursor: off, Blink: off
//----------------------------------------------0123456789ABCDEF0123--------------------------------
//                                              HH:MM.51
//--------------------------------------------------------------------------------------------------
    #define HOURS_STATIC_LINE                               grphLINE_TWO
    #define HOURS_STATIC_ADDR                               0x00
    #define MS_HOURS_DYN_ADDR                               0x00
    #define LS_HOURS_DYN_ADDR                               0x01

    #define MINS_STATIC_LINE                                grphLINE_TWO
    #define MINS_STATIC_ADDR                                0x03
    #define MS_MINS_DYN_ADDR                                0x03
    #define LS_MINS_DYN_ADDR                                0x04

    #define SECS_STATIC_LINE                                grphLINE_TWO
    #define SECS_STATIC_ADDR                                0x05
    #define MS_SECS_DYN_ADDR                                0x06
    #define LS_SECS_DYN_ADDR                                0x07

//----------------------------------------------0123456789ABCDEF0123--------------------------------
//                                              (Thur)Sept/30/2002  ";
//--------------------------------------------------------------------------------------------------
    #define DATE_START_BASE_LINE                            grphLINE_THREE
    #define DATE_START_BASE_ADDR                            0x00

    #define WEEKDAY_STATIC_LINE                             grphLINE_FOUR
    #define WEEKDAY_STATIC_ADDR                             0x00
    #define WEEKDAY_DYN_ADDR                                0x01

    #define MONTH_STATIC_LINE                               grphLINE_THREE
    #define MONTH_STATIC_ADDR                               0x00
    #define MONTH_DYN_ADDR                                  0x00

    #define DATE_STATIC_LINE                                grphLINE_THREE
    #define DATE_STATIC_ADDR                                0x04
    #define MS_DATE_DYN_ADDR                                0x05
    #define LS_DATE_DYN_ADDR                                0x06

    #define YEAR_STATIC_LINE                                grphLINE_THREE
    #define YEAR_STATIC_ADDR                                0x08
    #define MS_YEAR_DYN_ADDR                                0x0A
    #define LS_YEAR_DYN_ADDR                                0x0B

    #define MS_HIGHEST_YEAR                                 0x02 // these are the last two digits of year
    #define MS_LOWEST_YEAR                                  0x00
    #define LS_HIGHEST_YEAR                                 0x09
    #define LS_LOWEST_YEAR                                  0x00

//--------------------------------------------------------------------------------------------------
//                                  Save:Y
    #define ge_TIME_DATE_SAVE_STATIC_LINE                      grphLINE_FOUR // these are the last two digits of year
    #define ge_TIME_DATE_SAVE_STATIC_ADDR                      0x08 // these are the last two digits of year
    #define ge_TIME_DATE_SAVE_DYN_ADDR                         0x0D // these are the last two digits of year
    
//--------------------------------------------------------------------------------------------------
    #define MS_HOURS_MIN_VALUE                              0x00
    #define MS_HOURS_MAX_VALUE                              0x02
    #define LS_HOURS_MIN_VALUE                              0x00
    #define LS_HOURS_MAX_VALUE                              0x03

    #define MS_MINS_MIN_VALUE                               0x00
    #define MS_MINS_MAX_VALUE                               0x05
    #define LS_MINS_MIN_VALUE                               0x00
    #define LS_MINS_MAX_VALUE                               0x09

    #define MS_SECS_MIN_VALUE                               0x00
    #define MS_SECS_MAX_VALUE                               0x05
    #define LS_SECS_MIN_VALUE                               0x00
    #define LS_SECS_MAX_VALUE                               0x09

//--------------------------------------------------------------------------------------------------
    #define MS_DATE_MIN_VALUE                               0x00
    #define MS_DATE_MAX_VALUE                               0x03
    #define LS_DATE_MIN_VALUE                               0x00
    #define LS_DATE_MAX_VALUE                               0x09

    #define MS_YEAR_MIN_VALUE                               0x00
    #define MS_YEAR_MAX_VALUE                               0x02
    #define LS_YEAR_MIN_VALUE                               0x00
    #define LS_YEAR_MAX_VALUE                               0x09

    #define START_OF_LINE                                   0x00 // start of line address
    #define MID_LINE                                        0x09 // after time
    #define CLEAR_LINE                                      YES

    #define HOURS_STATIC_SIZE                               0x04  // allow for NULL terminator in all fields of size below
    #define MINS_STATIC_SIZE                                0x03
    #define SECS_STATIC_SIZE                                0x04

    #define WEEKDAY_STATIC_SIZE                             0x07
    #define MONTH_STATIC_SIZE                               0x05
    #define DATE_STATIC_SIZE                                0x05
    #define YEAR_STATIC_SIZE                                0x05

    #define NUM_DAYS_WEEK                                   0x08
    #define NUM_CHARS_DAY                                   0x05

    #define NUM_DAYS_WEEK                                   0x08
    #define NUM_CHARS_DAY                                   0x05
    
    #define PROMPT_LINE                                     grphLINE_FOUR
    #define PROMPT_ADDR                                     0x0D
    #define GRPH_SINGLE_LINE                                0x01 // use line one for large fonts
    #define EXIT_LCD_LINE                                   grphLINE_FOUR
    #define EXIT_QUERY_LCD_ADDR                             0x0D    // 16 Char per line

    #define MAX_MESSAGE_LINES                               12 // number of lines for display

    #define grphMARGIN_TOP                                  4
    #define grphMARGIN_BOTTOM                               4
    #define grphMARGIN_LEFT                                 4
    #define grphMARGIN_RIGHT                                4
    #define grphROW_SPACES                                  2

    #define grphMAX_LINES_IN_WINDOW                         4
    #define grphMAX_CHARS_PER_LINE_IN_WINDOW                30
                                                            
    #define grphMAX_LINES                                   4
    #define grphMAX_CHARS_PER_LINE                          15


#endif
