#include "TypeDef.h"

#define TM101_SLAVE_ADDR            0x48u               // 7 bit addressing, ADD0=ADD1=0
#define TM101_POINTER_CONFIG_REG    0x01u               // pointer to configuration register
#define TM101_POINTER_DATA_REG_HIGH 0x03u               // pointer to high byte of data register
#define TM101_CONFIG_VALUE          0x81u               // single shot mode, 1 cnv hen suspend

void    TxByte_i2c              (uint8 tx_byte);
uint8   RxByte_i2c              (uint8 *rx_byte);
void    Config_i2c              (void);
uint8   ReadTemp_i2c            (void);



