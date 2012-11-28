/** ###################################################################
**     Filename  : Events.H
**     Project   : sndLib
**     Processor : MCF51CN128CLK
**     Beantype  : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 8/11/2010, 9:33 PM
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "TimerDelay.h"
#include "RxTx_spi.h"
#include "sdCS.h"
#include "ledGreen.h"
#include "ledRed.h"
#include "lcdBKLITE.h"
#include "sdDETECT.h"
#include "grphRESET.h"
#include "grphSCL.h"
#include "grphRS.h"
#include "grphSI.h"
#include "ModulusTimer.h"
#include "flashCS.h"
#include "swDebounceTimer.h"
#include "audEnable.h"
#include "i2c.h"
#include "peTimer.h"
#include "swPort.h"
#include "rtcINT.h"
#include "sndPWM.h"


void sndPWM_OnEnd(void);
/*
** ===================================================================
**     Event       :  sndPWM_OnEnd (module Events)
**
**     From bean   :  sndPWM [PWM]
**     Description :
**         This event is called when the specified number of cycles has
**         been generated. (Only when the bean is enabled - <Enable>
**         and the events are enabled - <EnableEvent>). The event is
**         available only when the <Interrupt service/event> property
**         is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void rtcINT_OnInterrupt(void);
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

void peTimer_OnInterrupt(void);
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

void i2c_OnReceiveData(void);
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

void i2c_OnTransmitData(void);
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

void i2c_OnByteTransfer(void);
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

void i2c_OnArbitLost(void);
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

void i2c_OnNACK(void);
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

void swDebounceTimer_OnInterrupt(void);
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

void ModulusTimer_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  ModulusTimer_OnInterrupt (module Events)
**
**     From bean   :  ModulusTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RxTx_spi_OnTxChar(void);
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

void TimerDelay_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TimerDelay_OnInterrupt (module Events)
**
**     From bean   :  TimerDelay [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.06 [04.26]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
