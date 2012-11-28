#include "PE_Const.h"
#include "IO_Map.h"

//===============================================================================================
#include "RPS_BldOptions.h"
#include <string.h>

#include "TypeDef.h"
#include "EnumDef.h"
#include "FcnDef.h"

#include "i2c.h"
#include "peTimer.h"

#include "DeviceRegStructDef.h"
#include "ExtGlobalVarDeclar.h"

uint16                      u16_interrupt_status;

uint8 static                read_char;
uint8 static                u8_read_byte;
uint8                      *pu8_data_block;                 // remove and use au8_master below

//----------------------------------------------------------------------------------------------
extern uint16               u16_i2cOnReceiveDataFlag,
                            u16_i2cOnTransmitDataFlag,
                            u16_i2cOnWriteDataFlag,
                            u16_i2cOnByteTransferFlag,
                            u16_i2cOnArbitLostFlag,
                            u16_i2cOnNACKFlag,
                            u16_i2cOnRxCharFlag,
                            u16_i2cOnTxCharFlag,
                            u16_i2cOnReadReqFlag,
                            u16_i2cOnWriteReqFlag;
//----------------------------------------------------------------------------------------------

uint16               u16_block_size, u16_data_bytes_snt, *pu16_data_bytes_snt;
//----------------------------------------------------------------------------------------------

void    vFn_ResetI2CInterruptFlags          (void);
uint8   vFn_ReadReg_i2c                     (uint8      u8_reg_ptr);
uint8   u8Fn_ReadByte_i2c                   (uint16     ge_i2c_mode);

void    vFn_ReadBlock_i2c                   (uint8      u8_reg_ptr,
                                             uint8      u8_byte_cnt,
                                             uint8     *pu8_buffer);

void    vFn_WriteReg_i2c                    (uint8      u8_reg_ptr,
                                             uint8      u8_write_byte);

uint8   u8Fn_WriteBlock_i2c                 (uint8     *pu8_data_block,
                                             uint16     u16_block_size,
                                             uint16    *pu16_data_bytes_snt);

void    vFn_peSetTimeOutTimer               (uint16     ge_timeout_in_ms);
//----------------------------------------------------------------------------------------------

void    vFn_rtc_m41t81s_test                (void);
//----------------------------------------------------------------------------------------------

#ifdef RPS_ACCELEROMETER_FCNS_OPT_LIB
    #include "accelFcns_opt.c"
#endif

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_TempSensor )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_TempSensor
    Function:       read all registers of temperature sensor
    
    Input:          void

    Return:         void

    Notes:          struct has all values filled in on return
----------------------------------------------------------------------------------------------------*/
void vFn_TempSensor (ts_lm75_union      *pu_ts_lm75)
{

    vFn_i2cDeviceSetup(ge_TEMP_LM75_ADDR);              // setup bean and processor

    vFn_ReadBlock_i2c
            ( ge_TS_LM75_TEMP_ADDR,
             0x05,                                      //iMn               sizeof(u_ts_lm75.au8_ts_buffer),
             &(pu_ts_lm75->au8_ts_buffer[0]) );         // read block, all registers

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_rtc_m4t81s_test )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_rtc_m41t81s_test
    Function:       test read of M41T81S clock
    
    Input:          void

    Return:         void

    Notes:          cannot use WriteBlock using au8_rtc_buffer for rtc because of subsec data element.
----------------------------------------------------------------------------------------------------*/
void    vFn_rtc_m41t81s_test (void)
{
    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup i2c and processor

    vFn_ReadBlock_i2c
        (ge_M41T81S_PTR_ADDR,                           // start address for rtc reg ptr
         sizeof(u_rtc_m41t81s.au8_rtc_buffer),
         &u_rtc_m41t81s.au8_rtc_buffer[0]);

    vFn_rtcCntrl(ge_RTC_ENABLE_OSCILL);
    vFn_rtcCntrl(ge_RTC_CLEAR_OSCILL_FAIL);

    vFn_ReadBlock_i2c
        (ge_M41T81S_PTR_ADDR,                           // start address for rtc reg ptr
         sizeof(u_rtc_m41t81s.au8_rtc_buffer),
         &u_rtc_m41t81s.au8_rtc_buffer[0]);

    vFn_i2cDeviceSetup(ge_M41T81S_ADDR);                // setup bean and processor

    pu_rtc_m41t81s      = &u_rtc_m41t81s;
    pu16_data_bytes_snt = &u16_data_bytes_snt;

    memset(u_rtc_m41t81s.au8_rtc_buffer, 0x00, sizeof(u_rtc_m41t81s.au8_rtc_buffer[0]) );

#ifdef RTC_ERASE
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_mins        = 0x02;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_seconds     = 0x01;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_hours       = 0x03;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_day         = 0x04;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_date        = 0x05;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_month       = 0x06;
    pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_year        = 0x07;

    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_HOURS,
             0x00);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_SECONDS,
             0x01);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_MINUTES,
             0x02);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_HOURS,
             0x03);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_DAY,
             0x04);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_DATE,
             0x05);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_ALARM_MONTH,
             0x06);                                                 // clear HT bit to turn clock on
    vFn_WriteReg_i2c
            (ge_M41T81S_YEAR,
             0x07);                                                 // clear HT bit to turn clock on
#endif

    vFn_rtcCntrl(ge_RTC_ENABLE_OSCILL);

    for(;;){
        memset(u_rtc_m41t81s.au8_rtc_buffer, 0x55, sizeof(u_rtc_m41t81s.au8_rtc_buffer) );
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_seconds    = vFn_ReadReg_i2c(ge_M41T81S_SECONDS);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_mins       = vFn_ReadReg_i2c(ge_M41T81S_MINUTES);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_hours      = vFn_ReadReg_i2c(ge_M41T81S_HOURS);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_day        = vFn_ReadReg_i2c(ge_M41T81S_DAY);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_date       = vFn_ReadReg_i2c(ge_M41T81S_DATE);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_month      = vFn_ReadReg_i2c(ge_M41T81S_MONTH);
        pu_rtc_m41t81s->s_rtc.s_rtc_timedate_struct.u8_year       = vFn_ReadReg_i2c(ge_M41T81S_YEAR);
    }

    return;
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( vFn_peSetTimeOutTimer ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

vFn_peSetTimeOutTimer
    Function:   vFn_peSetTimeOutTimer
    
    Input:      uint16 ge_timeout_in_ms, this is enumerated count of timer intervals to wait before setting flag
    
    Return:     termination status
    
    Notes:      Caller uses enum count of increments to set global variable used by events bean
                Inits flag, cntr and enables bean and events
                Caller waits on gu8_peTimerFlag to set False to continue
--------------------------------------------------------------------------------------------------*/
void vFn_peSetTimeOutTimer (uint16 ge_timeout_in_ms)
{
    gs16_peTimerCntr = ge_timeout_in_ms;
    gu8_peTimerFlag = ge_PE_TIMER_ACTIVE;
#ifdef TIMEOUT_TIMER_ENABLE
    peTimer_Enable();
    peTimer_EnableEvent();
#endif

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_WriteReg_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_WriteReg_i2c
    Function:       Write rtc register
    
    Input:          uint8 register address, uint8 u8_write_byte

    Return:         void
    
    Notes:          this is a call to WriteBlock with count of two
----------------------------------------------------------------------------------------------------*/
void vFn_WriteReg_i2c (uint8      u8_reg_ptr,
                       uint8      u8_write_byte)
{
uint8   tx_block[2];
uint16  u16_data_bytes_snt;

    tx_block[0] = u8_reg_ptr;
    tx_block[1] = u8_write_byte;

    u8Fn_WriteBlock_i2c
        (tx_block,
         sizeof(tx_block),
         &u16_data_bytes_snt);           // write control register

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( u8Fn_WriteBlock_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       writes block with reg address as first byte
    
    Input:          uint8 u8_write_byte, byte to write

    Return:         void
    
    Notes:          The address pointer must be set in the first byte or a single byte write done first

    do{ 
        u8_status = i2c_SendBlock
                         (pu8_data_block,
                          u16_block_size, 
                          pu16_data_bytes_snt);      // send out the rtc address ptr as first tx
    }while(IIC1S_BUSY);                           // I2C BUSY FLAG, WILL NOT WORK WITH PE FLAGS ONLY
----------------------------------------------------------------------------------------------------*/
uint8    u8Fn_WriteBlock_i2c (uint8   *pu8_data_block,
                              uint16   u16_block_size,
                              uint16  *pu16_data_bytes_snt)
{
uint8   u8_status;
uint16  u16_i;

    u16_i = 0;
    vFn_peSetTimeOutTimer(ge_TMR_TIMEOUT_10_SEC);       // sets flag ge_PE_TIMER_ACTIVE until time out of TL DC interface
    u16_i2cOnTransmitDataFlag = ge_FALSE;
    u8_status = ERR_FAILED;                             // init status
    do{
        if(u16_i > 0x20    &&
           u8_status == ERR_BUSOFF){
            i2c_Init();                                 // if locked up, reset
        }else {

            u8_status = i2c_SendBlock
                             (pu8_data_block,
                              u16_block_size, 
                              pu16_data_bytes_snt);     // reg address ptr is first byte
        }
        ++u16_i;                                        // loop counter, if stuck on ERR_nnn
    }while(u8_status == ERR_BUSY     ||                 // have to use do loop to get status rtn from driver
           u8_status == ERR_BUSOFF   ||
           IIC1S_BUSY                &&
           u8_status == ERR_FAILED   &&
           gu8_peTimerFlag == ge_PE_TIMER_ACTIVE);      // wait if busoff and busy and not timed out, only do once

    while(u16_i2cOnTransmitDataFlag != ge_TRUE  &&
          gu8_peTimerFlag           == ge_PE_TIMER_ACTIVE){};

    if(gu8_peTimerFlag == ge_PE_TIMER_COMPLETE){
        u8_status = ge_PE_TIMER_COMPLETE;               // timer timed out
    }else {
        u8_status = ge_i2c_MSSG_TX_SUCCESS;
    }

    return(u8_status);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_ReadBlock_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_ReadBlock_i2c
    Function:       Read a block of registers, starting at the address passed
    
    Input:          uint8 start register address, uint8 bytes to read, *uint8 buffer to fill

    Return:         void
    
    Notes:          
----------------------------------------------------------------------------------------------------*/
void vFn_ReadBlock_i2c (uint8      u8_reg_ptr,
                        uint8      u8_byte_cnt,
                        uint8     *pu8_buffer)
{
uint8   u8_read_byte, u8_status;
uint16  u16_rcv_bytes;

    u16_i2cOnTransmitDataFlag = ge_FALSE;
    u8_status = ERR_FAILED;                             // init status
    do{
        u8_status = i2c_SendChar(u8_reg_ptr);          // send out the reg address ptr, this is not the slave addr
    }while(u8_status == ERR_BUSY     ||                 // have to use do loop to get status rtn from driver
           u8_status == ERR_BUSOFF   ||
           IIC1S_BUSY                &&
           u8_status == ERR_FAILED);                    // wait for bus and busy, only do once

    while(u16_i2cOnTransmitDataFlag != ge_TRUE){};

    u16_i2cOnReceiveDataFlag = ge_FALSE;                // wait for send of device addr reg to complete
    u8_status = ERR_FAILED;                             // init status
    do{
        u8_status = i2c_RecvBlock
                        (pu8_buffer,
                         u8_byte_cnt,
                         &u16_rcv_bytes);                   // bytes read
    }while(u8_status == ERR_BUSY     ||                 // have to use do loop to get status rtn from driver
           u8_status == ERR_BUSOFF   ||
           IIC1S_BUSY                &&
           u8_status == ERR_FAILED);                    // wait for bus and busy, only do once

    while(u16_i2cOnReceiveDataFlag != ge_TRUE){};

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_ReadReg_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_ReadReg_i2c
    Function:       Read rtc register
    
    Input:          uint8 register address

    Return:         uint8 read_byte
    
    Notes:          only for Master mode
                    slave address sent, 
                    register pointer address sent, 
                    slave address sent, 
                    data read
----------------------------------------------------------------------------------------------------*/
uint8 vFn_ReadReg_i2c (uint8      u8_reg_ptr)
{
uint8 u8_read_byte, u8_status;

    u16_i2cOnTransmitDataFlag = ge_FALSE;
    u8_status = ERR_FAILED;                             // init status
    do{
        u8_status = i2c_SendChar(u8_reg_ptr);          // send out the address ptr
    }while(u8_status == ERR_BUSY     ||                 // have to use do loop to get status, or status rtn from driver
           u8_status == ERR_BUSOFF   ||
           IIC1S_BUSY                &&
           u8_status == ERR_FAILED);                    // wait for bus and busy, only do once

    while(u16_i2cOnTransmitDataFlag != ge_TRUE){};

    u16_i2cOnReceiveDataFlag = ge_FALSE;
    u8_status = ERR_FAILED;                             // init status
    do{
        u8_status = i2c_RecvChar(&u8_read_byte);        // read the response
    }while(u8_status == ERR_BUSY     ||                 // have to use do loop to get status, or status rtn from driver
           u8_status == ERR_BUSOFF   ||
           IIC1S_BUSY                &&
           u8_status == ERR_FAILED);                    // wait for bus and busy, only do once

    while(u16_i2cOnReceiveDataFlag != ge_TRUE){};

    return(u8_read_byte);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( u8Fn_ReadByte_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
u8Fn_ReadByte_i2c
    Function:       Read rtc register
    
    Input:          uint16 ge_i2c_mode, ge_I2C_SLAVE or ge_I2C_MASTER read

    Return:         uint8 read_byte
    
    Notes:          when as master, address of slave sent then one char of data read
                    when as slave, one char data read - no address sent
----------------------------------------------------------------------------------------------------*/
uint8 u8Fn_ReadByte_i2c (uint16      ge_i2c_mode)
{
uint8 static u8_read_byte, u8_status;
uint16  u16_i;

    u16_i = 0;

    if(ge_i2c_mode == ge_I2C_MASTER){
        u16_i2cOnReceiveDataFlag = ge_FALSE;            // this flag only exists for Master mode
        u8_status = ERR_FAILED;                         // init status
        do{
            if(u16_i > 0x20    &&
               u8_status == ERR_BUSOFF){
                i2c_Init();                             // if locked up, reset
            }else {
                u8_status = i2c_RecvChar(&u8_read_byte);
            }
            ++u16_i;                                    // loop counter, if stuck on ERR_nnn
        }while(u8_status == ERR_BUSY     ||
               u8_status == ERR_BUSOFF   ||
               IIC1S_BUSY                &&
               u8_status == ERR_FAILED);                // wait for bus and busy, only do once
        while(u16_i2cOnReceiveDataFlag  != ge_TRUE){};  // wait for read response, only for Master mode
    }else{
    if(ge_i2c_mode == ge_I2C_SLAVE){
        u16_i2cOnRxCharFlag = ge_FALSE;
        while(u16_i2cOnRxCharFlag   != ge_TRUE){};      // this is toggled when the char is read, wait for read response

        u8_status = ERR_FAILED;                         // init status
        do{
            u8_status = i2c_RecvChar(&u8_read_byte);    // After we have received a read response, now enter the RecvChar function
        }while(u8_status == ERR_BUSY     ||
               u8_status == ERR_BUSOFF   ||
               IIC1S_BUSY                &&
               u8_status == ERR_FAILED);                // wait for bus and busy, only do once
    }}

    return(u8_read_byte);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_StatusCheck )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       debug check to check PE status
    
    Input:          uint8 u8_status, use status returned from PE

    Return:         u8_status, status found
    
    Notes:          used at present for tracing debug, not intended for implementation
----------------------------------------------------------------------------------------------------*/
uint8   vFn_StatusCheck (uint8 u8_status)
{
    if(u8_status != ERR_OK){
        if(u8_status == ERR_SPEED){
            u8_status = ge_FAIL;
        }else {
        if(u8_status == ERR_BUSY){
            u8_status = ge_FAIL;
        }else {
        if(u8_status == ERR_BUSOFF){
            u8_status = ge_FAIL;
        }else {
        if(u8_status == ERR_DISABLED){
            u8_status = ge_FAIL;
        }}}}
    }

    return(u8_status);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_ResetI2CInterruptFlags )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_ResetI2CInterruptFlags
    Function:       clear flags
    
    Input:          null

    Return:         null
    
    Notes:          used primarily for debug
----------------------------------------------------------------------------------------------------*/
void    vFn_ResetI2CInterruptFlags (void)
{
    u16_i2cOnReceiveDataFlag    = ge_FALSE;
    u16_i2cOnTransmitDataFlag   = ge_FALSE;
    u16_i2cOnByteTransferFlag   = ge_FALSE;
    u16_i2cOnArbitLostFlag      = ge_FALSE;
    u16_i2cOnNACKFlag           = ge_FALSE;
    u16_i2cOnRxCharFlag         = ge_FALSE;
    u16_i2cOnTxCharFlag         = ge_FALSE;
    u16_i2cOnReadReqFlag        = ge_FALSE;
    u16_i2cOnWriteReqFlag       = ge_FALSE;
    u16_i2cOnReadReqFlag        = ge_FALSE;
    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_EvalTrap_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       debug check to see which interrupt occurred
    
    Input:          null

    Return:         ge_FLAG of interrupt
    
    Notes:          Can only set 3 usuable interrupts, therefore this fcn tests which if any flag was set
                        and returns the interrupt state
----------------------------------------------------------------------------------------------------*/
uint16   vFn_EvalTrap_i2c(void)
{

    if(u16_i2cOnReceiveDataFlag     == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_RECEIVE_DATA;
    }else {
    if(u16_i2cOnTransmitDataFlag    == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_TRANSMIT_DATA;
    }else {
    if(u16_i2cOnByteTransferFlag    == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_BYTE_TRANSFER;
    }else {
    if(u16_i2cOnArbitLostFlag       == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_ARBIT_LOST;
    }else {
    if(u16_i2cOnNACKFlag            == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_NACK;
    }else {
        u16_interrupt_status = ge_i2c_NO_INTERRUPT;     // no interrupt detected
    if(u16_i2cOnRxCharFlag            == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_RX;
    }else {
        u16_interrupt_status = ge_i2c_NO_INTERRUPT;     // no interrupt detected
    if(u16_i2cOnTxCharFlag            == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_TX;
    }else {
        u16_interrupt_status = ge_i2c_NO_INTERRUPT;     // no interrupt detected
    if(u16_i2cOnReadReqFlag            == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_READ;
    }else {
        u16_interrupt_status = ge_i2c_NO_INTERRUPT;     // no interrupt detected
    if(u16_i2cOnWriteReqFlag        == ge_TRUE){
        u16_interrupt_status = ge_i2c_ON_WRITE;
    }else {
        u16_interrupt_status = ge_i2c_NO_INTERRUPT;     // no interrupt detected
    }}}}}}}}}

    return(u16_interrupt_status);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_ReadByteReg_i2c )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       read one byte from address
    
    Input:          address to read from

    Return:         uint8
    
    Notes:          pe sends device address before each command
----------------------------------------------------------------------------------------------------*/
uint8    vFn_ReadByteReg_i2c(uint8 u8_address){

uint8   read_char, u8_status;

    u16_i2cOnReceiveDataFlag = ge_FALSE;
    do{
        u8_status = i2c_RecvChar(&read_char);           // read the response
    }while(u8_status == ERR_BUSY ||
           u8_status == ERR_BUSOFF);
    while(u16_i2cOnReceiveDataFlag     != ge_TRUE){}

    return(read_char);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_i2cDeviceSetup )))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_i2cDeviceSetup
    Function:       Sets the uC into Slave mode and enables PE Beans and events
    
    Input:          uint16    ge_device_addr, when Master, sets device address ID of slave
    Return:         void
----------------------------------------------------------------------------------------------------*/
void vFn_i2cDeviceSetup (uint16    ge_device_addr)
{
    vFn_ResetI2CInterruptFlags();                       // specify the slave address, in PE bean                                            // specify the slave address, in PE bean
    i2c_Enable();                                       // enable bean
    i2c_SelectSlave(ge_device_addr);                    // specify the address
    i2c_EnableEvent();                                  // enable interrupts

    return;
}


