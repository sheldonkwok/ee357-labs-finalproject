#ifndef DATE_TIME_CONST_DEF_H
    #define DATE_TIME_CONST_DEF_H
//-------------------------------- System Clock ----------------------------------------------------
#define TIME_ENABLED                                    YES
#define TIME_DISABLED                                   NO
#define DATE_ENABLED                                    YES
#define DATE_DISABLED                                   NO

#define MAX_SECS                                        0x59  // must use hex, not decimal, or compares fail due to radix base
#define MAX_MINS                                        0x59
#define MAX_HOURS                                       0x23

//----------------------------------------------------------------------------------------------------
#define JAN                                             0x01
#define FEB                                             0x02
#define MAR                                             0x03
#define APR                                             0x04
#define MAY                                             0x05
#define JUN                                             0x06
#define JUL                                             0x07
#define AUG                                             0x08
#define SEP                                             0x09
#define OCT                                             0x0A
#define NOV                                             0x0B
#define DEC                                             0x0C

//--------------------------------------------------------------------------------------------------
#define MS_DIGIT                                        0x00 // offset into ascii digit buffers, time and date
#define LS_DIGIT                                        0x01
#define HIGH_NIBBLE_MASK                                0xF0
#define LOW_NIBBLE_MASK                                 0x0F
#define CLEAR_CHARS                                     YES
#define DONOT_CLEAR_CHARS                               NO
#define STRING_ON                                       YES
#define STRING_OFF                                      NO
#define FIRST_MONTH                                     0x01
#define LAST_MONTH                                      0x0C
#define FIRST_WEEK_DAY                                  0x01
#define LAST_WEEK_DAY                                   0x07

#endif
