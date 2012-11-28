/*============================================================================================================
                                $RCSfile: Events.c,v $

       $Author: blewis $
        $Revision: 1.1 $
       $Date: 2006/06/22 19:02:37 $
       $Source: G:/cvsrepo-may-30-2005/FS-DP512-jun-22-06/CODE/Events.c,v $

=============================================================================================================*/
/** ###################################################################
**     Filename  : Events.C
**     Project   : fio_rc2
**     Processor : MC9S12DG128BCPV
**     Beantype  : Events
**     Version   : Driver 01.03
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 8/20/2005, 4:07 PM
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         Timer4_OnInterrupt - void Timer4_OnInterrupt(void);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/
/* MODULE Events */
#include "RPS_BldOptions.h"

#ifndef NAV_SW_POLLED
    #include "kbiPort.h"
#endif

#include "Cpu.h"
#include "Events.h"

#include "DateTimeConstDef.h"
#include "mssgConstDef.h"

#include "TypeDef.h"

#include "EnumDef.h"

#include "SwitchStructDef.h"
#include "ExtSwitchDeclar.h"

#include "ExtGlobalVarDeclar.h"

#include "sdEnumDef.h"

#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdFcnDef.h"

#include "FcnDef.h"

#include "sdExtFileDeclar.h"
#include "sdWaveConstDef.h"
#include "WaveStructDeclar.h"
#include "rtcConstDef.h"

extern uint8  volatile  gu8_FileFinishedFlag, gu8_BlockFinishedFlag;
extern char             c_SampleIndex;
extern uint16 volatile  gu16_LastElement, gu16_DataIndex, u16_BitsPerSample;
extern uint32 volatile  gu32_ByteCntr, gu32_DataChunkSize, gu32_BlockCntr;
extern uint8  volatile  WaveData[READ_BUFF_SIZE], *gc_WaveDataPtr;
extern                  u_chunk                 gs_Chunk;

extern uint16 LoopCntr;

extern s_rtcAlarmStruct             *ps_rtc_alarm;

uint32 invalid_interrupt_cntr;

uint16                       u16_i2cOnReceiveDataFlag,
                             u16_i2cOnTransmitDataFlag,
                             u16_i2cOnWriteDataFlag,
                             u16_i2cOnByteTransferFlag,
                             u16_i2cOnArbitLostFlag,
                             u16_i2cOnNACKFlag,
                             u16_i2cOnRxCharFlag,
                             u16_i2cOnTxCharFlag,
                             u16_i2cOnReadReqFlag,
                             u16_i2cOnWriteReqFlag,
                             u16_i2cOnReadReqFlag;


#define DEBOUNCE_TIME               ge_SW_DEBOUNCE_300_MS
/*
** ===================================================================
**     Event       :  kbiPort_OnInterrupt (module Events)
**
**     From bean   :  kbiPort [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
**
** MODULE kbiPort
** ===================================================================
*/
#ifdef NAV_SW_INTERRUPT
void kbiPort_OnInterrupt (void)
{
uint8        switch_value;

    switch_value = ~(kbiPort_GetVal());                         // complement all the port pins

    switch(switch_value){

        case    ge_SWLFT:
            if( (swStatusFlagsPtr->Lft.Enable == ge_SW_INT_ENABLE) &&                   // now check if interrupt to be acknowledged 
                (gu8_swDebounceTimerFlag      == ge_SW_DEBOUNCE_COMPLETE) ){            // don't acknowledge if debounce active

               swStatusFlagsPtr->Lft.Status                           = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->LftOrRt.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection;       // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_LFT;                             // save new/current direction

               vFn_swSetDebounceTimer(DEBOUNCE_TIME);
            }
        break;

        case    ge_SWRT:
            if( (swStatusFlagsPtr->Rt.Enable == ge_SW_INT_ENABLE) &&                    // now check if interrupt to be acknowledged
                (gu8_swDebounceTimerFlag     == ge_SW_DEBOUNCE_COMPLETE) ){             // don't acknowledge if debounce active

               swStatusFlagsPtr->Rt.Status                            = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->LftOrRt.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection;       // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_RT;                              // save new/current direction

               vFn_swSetDebounceTimer(DEBOUNCE_TIME);
            }
        break;

        case    ge_SWUP:
            if( (swStatusFlagsPtr->Up.Enable == ge_SW_INT_ENABLE) &&                    // now check if interrupt to be acknowledged
                (gu8_swDebounceTimerFlag     == ge_SW_DEBOUNCE_COMPLETE) ){             // don't acknowledge if debounce active

               swStatusFlagsPtr->Up.Status                            = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->UpOrDwn.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection; // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_UP;          // save new/current direction

               vFn_swSetDebounceTimer(DEBOUNCE_TIME);
            }
        break;

        case    ge_SWDWN:
            if( (swStatusFlagsPtr->Dwn.Enable == ge_SW_INT_ENABLE) &&                   // now check if interrupt to be acknowledged
                (gu8_swDebounceTimerFlag      == ge_SW_DEBOUNCE_COMPLETE) ){            // don't acknowledge if debounce active

               swStatusFlagsPtr->Dwn.Status                           = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->UpOrDwn.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection; // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_DOWN;        // save new/current direction

               vFn_swSetDebounceTimer(DEBOUNCE_TIME);
            }
        break;

        case    ge_SWSEL:
            if( (swStatusFlagsPtr->Sel.Enable == ge_SW_INT_ENABLE) &&                   // now check if interrupt to be acknowledged
                (gu8_swDebounceTimerFlag      == ge_SW_DEBOUNCE_COMPLETE) ){            // don't acknowledge if debounce active

               swStatusFlagsPtr->Sel.Status                           = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET;
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               vFn_swSetDebounceTimer(DEBOUNCE_TIME);
            }
        break;

        default:
                invalid_interrupt_cntr++;               // trap, shouldn't happen
        break;
    }

    kbiPort_GetStatus();                                // get and clear interrupt flag     

    return;
}
#endif

/*
** ===================================================================
**     Event       :  RxTx_spi_OnTxChar (module Events)
**
**     From bean   :  RxTx_spi [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RxTx_spi_OnTxChar(void)
{
    RxTx_spi_Flag = ge_TX_COMPLETE_SPI;

    return;
}

/*
** ===================================================================
**     Event       :  RxTx_spi_OnRxChar (module Events)
**
**     From bean   :  RxTx_spi [SynchroMaster]
**     Description :
**         This event is called after a correct character is
**         received.
**         DMA mode:
**         If DMA controller is available on the selected CPU and
**         the receiver is configured to use DMA controller then
**         this event is disabled. Only OnFullRxBuf method can be
**         used in DMA mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RxTx_spi_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  RxTx_spi_OnError (module Events)
**
**     From bean   :  RxTx_spi [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be
**         read using <GetError> method.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RxTx_spi_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TimerDelay_OnInterrupt (module Events)
**
**     From bean   :  TimerDelay [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TimerDelay_OnInterrupt(void)
{
    TimerDelayFlag = TRUE;                // set the interrupt flag
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  INTERRUPT ROUTINES  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
** ===================================================================
**     Event       :  ModulusTimer_OnInterrupt (module Events)
**
**     From bean   :  ModulusTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
            if(u16_BitsPerSample == 8){
                sndPWM_SetRatio8(*(gc_WaveDataPtr + ((gu16_DataIndex * c_SampleIndex) + gu8_ChannelSide)));
            }else {
            if(u16_BitsPerSample == 16){
                sndPWM_SetRatio16(*(gc_WaveDataPtr + ((gu16_DataIndex * c_SampleIndex) + gu8_ChannelSide)));
            }}
must change all buffer pointers and calculations, use a union for generic buffer access for u8, u16
** ===================================================================
*/
void ModulusTimer_OnInterrupt(void)
{
    if(gu32_ByteCntr < gu32_DataChunkSize){
        if((gu16_DataIndex * c_SampleIndex) < gu16_LastElement){
            sndPWM_SetRatio8(*(gc_WaveDataPtr + ((gu16_DataIndex * c_SampleIndex) + gu8_ChannelSide)));
            gu16_DataIndex++;
            gu32_ByteCntr = gu32_ByteCntr + c_SampleIndex;  // mono = 1, stereo = 2
        }else {
            gu8_BlockFinishedFlag = ge_YES;                 // indicates block finished processed
            gu16_DataIndex = 0;                             // new block, start DataIndex at 0
            gu32_BlockCntr++;                               // need to find last block, to set gu16_LastElement
        }
    }else {
        gu8_FileFinishedFlag = ge_YES;                      // indicates finished processed
    }
}

/*
** ===================================================================
**     Event       :  sndPWM_OnEnd (module Events)
**
**     From bean   :  sndPWM [PWM]
**     Description :
**         This event is called when the specified number of cycles
**         has been generated. (Only when the bean is enabled -
**         <Enable> and the events are enabled - <EnableEvent>). The
**         event is available only when the peripheral supports an
**         interrupt, that is generated at the end of the PWM period.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void sndPWM_OnEnd(void)
{
  /* Write your code here ... */
}


/*
** ===================================================================
**     Event       :  peTimer_OnInterrupt (module Events)
**
**     From bean   :  peTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void peTimer_OnInterrupt(void)
{
    if(gs16_peTimerCntr > 0){
        gs16_peTimerCntr--;                             // loop cnt of bean time increments, set by vFn_zgbRxSetTimeOut()
    } else {
        gu8_peTimerFlag = ge_PE_TIMER_COMPLETE;         // cntr time out is done
        peTimer_DisableEvent();                         // turn off events
        peTimer_Disable();                              // disable bean
    }


}

/*
** ===================================================================
**     Event       :  i2c_OnReceiveData (module Events)
**
**     From bean   :  i2c [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of
**         the data successfully. This event is not available for
**         the SLAVE mode and if both RecvChar and RecvBlock are
**         disabled. This event is enabled only if interrupts/events
**         are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void i2c_OnReceiveData(void)
{
    u16_i2cOnReceiveDataFlag = ge_TRUE;
}

/*
** ===================================================================
**     Event       :  i2c_OnTransmitData (module Events)
**
**     From bean   :  i2c [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission
**         of the data successfully. This event is not available for
**         the SLAVE mode and if both SendChar and SendBlock are
**         disabled. This event is enabled only if interrupts/events
**         are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void i2c_OnTransmitData(void)
{
    u16_i2cOnTransmitDataFlag = ge_TRUE;
}

/*
** ===================================================================
**     Event       :  i2c_OnByteTransfer (module Events)
**
**     From bean   :  i2c [InternalI2C]
**     Description :
**         This event is called when one-byte transfer (including
**         the acknowledge bit) is successfully finished (slave
**         address or one data byte is transmitted or received).
**         This event is not available for the SLAVE mode and if all
**         RecvChar, RecvBlock, SendChar and SendBlock are disabled.
**         Note: It is possible to use the event for slowing down
**         communication, when slower slave needs some time for data
**         processing.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void i2c_OnByteTransfer(void)
{
    u16_i2cOnByteTransferFlag = ge_TRUE;
}

/*
** ===================================================================
**     Event       :  i2c_OnArbitLost (module Events)
**
**     From bean   :  i2c [InternalI2C]
**     Description :
**         This event is called when the master lost the bus
**         arbitration or the device detects an error on the bus.
**         This event is enabled only if interrupts/events are
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void i2c_OnArbitLost(void)
{
    u16_i2cOnArbitLostFlag = ge_TRUE;
}

/*
** ===================================================================
**     Event       :  i2c_OnNACK (module Events)
**
**     From bean   :  i2c [InternalI2C]
**     Description :
**         Called when a no slave acknowledge (NAK) occurs during
**         communication. This event is not available for the SLAVE
**         mode. This event is enabled only if interrupts/events are
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void i2c_OnNACK(void)
{
    u16_i2cOnNACKFlag = ge_TRUE;
}

/*
** ===================================================================
**     Event       :  swDebounceTimer_OnInterrupt (module Events)
**
**     From bean   :  swDebounceTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void swDebounceTimer_OnInterrupt(void)
{
    if(gs16_swDebounceTimerCntr > 0){
        gs16_swDebounceTimerCntr--;                     // loop cnt of bean time increments
    } else {
        gu8_swDebounceTimerFlag = ge_SW_DEBOUNCE_COMPLETE;  // cntr time out is done
        swDebounceTimer_DisableEvent();              // turn off events
        swDebounceTimer_Disable();                   // disable bean
    }

    return;

}

/*
** ===================================================================
**     Event       :  rtcINT_OnInterrupt (module Events)
**
**     From bean   :  rtcINT [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void rtcINT_OnInterrupt(void)
{
    u8_rtcAlarmIntFlag = ge_RTC_ALARM_INT_SET;      // mark that interrupt occurred
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.97 [03.83]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
