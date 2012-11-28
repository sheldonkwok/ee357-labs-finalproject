#include "TypeDef.h"
#include "lcdConstDef.h"

#include "RxTx_spi.h"            // pulls in cpu.h for port level defines

#include "Events.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "ExtGlobalVarDeclar.h"

#include "FcnDef.h"

/*============================================================================================================
                                $RCSfile: OutputFcns.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.8 $
       $Date: 2007/06/05 20:22:39 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/OutputFcns.c,v $

=============================================================================================================*/
/*--------------------------------------------------------------------------------------------------
                                  ledIndicator                                                      
----------------------------------------------------------------------------------------------------

    Function:   LedIndic - Set an LED on, off, blink
  
    Input:      uint16 led_state, uint16 led_color, uint8 blink_num
  
    Return:     none
  
    Notes:      port pin is set to sink current, therefore must clr bit to turn on LED
--------------------------------------------------------------------------------------------------*/

void ledIndicator (uint16 led_state,
                   uint16 led_color,
                   uint8 blink_num)
{
unsigned int i;
bool led_value;

    switch (led_state){

       case ge_LED_ON:
            if(led_color == ge_LED_GREEN){
                ledGreen_ClrVal();                      // LED ON
            }else {
                ledRed_ClrVal();                        // LED ON
            }
       break;

       case ge_LED_OFF:
            if(led_color == ge_LED_GREEN){
                ledGreen_SetVal();                      // LED ON
            }else {
                ledRed_SetVal();                      // LED OFF
            }
       break;

       case ge_LED_TOGGLE:
            if(led_color == ge_LED_GREEN){
                for(i = 0; i < (blink_num * 2); i++){
                    ledGreen_NegVal();
                    TimeDelay(ge_T_500_MS);
                }
            }else {
            if(led_color == ge_LED_RED){
                for(i = 0; i < (blink_num * 2); i++){
                    ledRed_NegVal();
                    TimeDelay(ge_T_500_MS);
                }
            }else {
            if(led_color == ge_LED_BOTH){
                for(i = 0; i < (blink_num * 2); i++){
                    ledGreen_NegVal();
                    ledRed_NegVal();
                    TimeDelay(ge_T_500_MS);
                }
            }}}
       break;
    }

    return;
}

/*--------------------------------------------------------------------------------------------------
                                  SetOutPut                                                      
----------------------------------------------------------------------------------------------------

    Function:   SetOutPut - Set a global variable indicating output type
  
    Input:      uint8 for device driver switch statement, uint8 related only to lcd functions
                    indicating, ge_LCD_LINE_SCROLL_ON or ge_LCD_LINE_SCROLL_ON
  
    Return:     none
  
    Notes:      used by putchar to decide which IO specific output fcn to call

--------------------------------------------------------------------------------------------------*/

void SetOutPut (uint8 output_mode,
                uint8 lcd_scroll_cntrl)
{

    OutPutMode = output_mode;
    lcdScrollCntrl = lcd_scroll_cntrl;

    return;

}

/*-----------------------------------u8Fn_RxTx_spi.c-----------------------------------------------
   u8Fn_RxTx_spi
    Function:           Writes byte to the SPI
    Input Parameter:    uint8 data
    Return Type:        None
  
   Notes: 
      bit(3) spi_CE    O/P    SS or CE bit for SPI slave
      bit(2) rtc_DDR_SLAVE_CLK_SCLK_spi  O/P    SCK to clock data
      bit(1) rtc_DDR_MASTER_OUT_SLAVE_IN_MOSI_spi  O/P    Master Output Slave Input data stream
      bit(0) rtc_DDR_MASTER_IN_SLAVE_OUT_MISO_spi  I/P    Master Input Slave Output data stream
                                  data is being clocked out MS-->LS bit
                                  
       Set up the Baud Rate for SPI port:                                   
      ** Baud Rate = CPU speed (24 Mhz) / (Input divisor)                   
      ** Input divisor = (High Byte + 1)* 2 ^(LowByte + 1)                  
      ** For 0x21, Input Divisor = 12(decimal), therefore Baud Rate = 2 Mhz       
      ** 2 MHz is current value designed for with DEV1.                     

 *  The function waits for the flag to be cleared; since the SPI is using an interrupt, the only
 * way to clear the flag is by reaching the event OnTxChar. In this function a byte will be sent
 * only when the previous byte is fuly transmited 

--------------------------------------------------------------------------------------------------*/
uint8 u8Fn_RxTx_spi (uint8 bTxData)
{
uint8 read_byte;
uint8  error_status;

    RxTx_spi_Flag = ge_TX_INPROGRESS_SPI;                  // set Tx
    RxTx_spi_SendChar(bTxData);

    while (RxTx_spi_Flag == ge_TX_INPROGRESS_SPI){};       // wait for Tx to complete

    error_status = RxTx_spi_RecvChar(&read_byte);       //Here a full byte was clocked out, it is time to read data

    return(read_byte);
}


