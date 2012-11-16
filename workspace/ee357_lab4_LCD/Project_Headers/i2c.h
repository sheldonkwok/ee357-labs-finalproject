/*
 * i2c.h
 *
 *  Created on: Apr 4, 2011
 *      Author: Mark
 */

#ifndef I2C_H_
#define I2C_H_

typedef enum i2c_TS_LM75_reg_values
{
    ge_TS_LM75_TEMP_ADDR                = 0x00,
    ge_TS_LM75_CONFIG_ADDR              = 0x01,
    ge_TS_LM75_THYST_ADDR               = 0x02,
    ge_TS_LM75_TOS_ADDR                 = 0x03
} TS_LM75_reg_addr;

typedef struct sts {
        uint16      u16_temp;
        uint8       u8_config;
        uint8       u8_tos;
        uint8       u8_hyst;
 
} ts_lm75;


void i2c_init();
void i2c_write(unsigned char dev_addr, unsigned char *dat, int n);
void i2c_read(unsigned char dev_addr, unsigned char *dat, int n);
void i2c_read_w_ptr(unsigned char dev_addr, unsigned char pointer, unsigned char *dat, int n);

#endif /* I2C_H_ */
