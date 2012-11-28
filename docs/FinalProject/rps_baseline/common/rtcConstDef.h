#ifndef RTC_CONST_DEF_H
    #define RTC_CONST_DEF_H

    #define ds1305_TIME_ADDR_spi                            0x80
    #define ds1305_DATE_ADDR_spi                            0x83
    #define TRICKLE_ADDR_WRITE_spi                          0x91
    #define TRICKLE_ADDR_READ_spi                           0x11
    #define TRICKLE_1D_4K_spi                               0xA6
    #define TRICKLE_OFF                                     0xFC

    #define SEC_ALARM_DIAG_MASK_spi                         0x8F    // sec, min, hours, day = 0x8F. 1sec interrupt generated
    #define ALARM_ENABLE_MASK                               0x80    // bit 7 on, set alarm cycle based on field applied to

    #define RTC_INT_ALARM0                                  0x01    // bit position for alarm 0 in control register
    #define RTC_INT_ALARM1                                  0x02    // bit position for alarm 1 in control register
    #define RTC_INT_DUAL_ALARM                              0x04    // mask for dual alarm control, 1 = individual, 0 = only alarm 0 pin

    #define CNTRL_REG_ADDR_WRITE_spi                        0x8F

    #define SEC_ALARM0_ADDR_WRITE_spi                       0x87    // write address has bit 7 = 1
    #define MIN_ALARM0_ADDR_WRITE_spi                       0x88
    #define HOUR_ALARM0_ADDR_WRITE_spi                      0x89
    #define DAY_ALARM0_ADDR_WRITE_spi                       0x8A

    #define SEC_ALARM0_ADDR_READ_spi                        0x07    // read address has bit 7 = 0
    #define MIN_ALARM0_ADDR_READ_spi                        0x08
    #define HOUR_ALARM0_ADDR_READ_spi                       0x09
    #define DAY_ALARM0_ADDR_READ_spi                        0x0A

    #define SEC_ALARM1_ADDR_WRITE_spi                       0x8B    // write address has bit 7 = 1
    #define MIN_ALARM1_ADDR_WRITE_spi                       0x8C
    #define HOUR_ALARM1_ADDR_WRITE_spi                      0x8D
    #define DAY_ALARM1_ADDR_WRITE_spi                       0x8E

    #define SEC_ALARM1_ADDR_READ_spi                        0x0B // read address has bit 7 = 0
    #define HOUR_ALARM1_ADDR_READ_spi                       0x0C
    #define MIN_ALARM1_ADDR_READ_spi                        0x0D
    #define DAY_ALARM1_ADDR_READ_spi                        0x0E

    #define CNTRL_REG_ADDR_READ_spi                         0x0F

    #define OSCIL_ENABLE_spi                                0x80
    #define WRITE_PROTECT_spi                               0x40

#endif
