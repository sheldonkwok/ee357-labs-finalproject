/** ###################################################################
**     Filename  : Events.C
**     Project   : miscLib
**     Processor : MCF51CN128CLK
**     Beantype  : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 8/11/2010, 9:20 PM
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

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
void sndPWM_OnEnd(void)
{
  /* Write your code here ... */
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
  /* place your rtcINT interrupt procedure body here*/
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
  /* Write your code here ... */

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
  /* Write your code here ... */
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
  /* Write your code here ... */
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
  /* Write your code here ... */
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
  /* Write your code here ... */
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
  /* Write your code here ... */
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
  /* Write your code here ... */

}

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
void ModulusTimer_OnInterrupt(void)
{
  /* Write your code here ... */

}

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
  /* Write your code here ... */
}

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
void TimerDelay_OnInterrupt(void)
{
  /* Write your code here ... */

}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.06 [04.26]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/