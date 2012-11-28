/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : peTimer.h
**     Project   : fileLib
**     Processor : MCF51CN128CLK
**     Component : TimerInt
**     Version   : Component 02.158, Driver 01.20, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 3:09 PM
**     Abstract  :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : RTC (8-bit)
**         Compare name                : RTCmod
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 31250 Hz
**           Initial period/frequency
**             Xtal ticks              : 3
**             microseconds            : 96
**             seconds (real)          : 0.000096
**             Hz                      : 10417
**             kHz                     : 10
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Disabled
**              Events                 : Disabled
**
**         Timer registers
**              Counter                : RTCCNT    [0xFFFF82C1]
**              Mode                   : RTCSC     [0xFFFF82C0]
**              Run                    : RTCSC     [0xFFFF82C0]
**              Prescaler              : RTCSC     [0xFFFF82C0]
**
**         Compare registers
**              Compare                : RTCMOD    [0xFFFF82C2]
**
**         Flip-flop registers
**     Contents  :
**         Enable       - byte peTimer_Enable(void);
**         Disable      - byte peTimer_Disable(void);
**         EnableEvent  - byte peTimer_EnableEvent(void);
**         DisableEvent - byte peTimer_DisableEvent(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __peTimer
#define __peTimer

/* MODULE peTimer. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"
extern bool peTimer_EnEvent;           /* Enable/Disable events */


byte peTimer_Enable(void);
/*
** ===================================================================
**     Method      :  peTimer_Enable (component TimerInt)
**
**     Description :
**         This method enables the component - it starts the timer.
**         Events may be generated (<DisableEvent>/<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte peTimer_Disable(void);
/*
** ===================================================================
**     Method      :  peTimer_Disable (component TimerInt)
**
**     Description :
**         This method disables the component - it stops the timer. No
**         events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define peTimer_EnableEvent() (peTimer_EnEvent = TRUE, (byte)ERR_OK) /* Set the flag "events enabled" */
/*
** ===================================================================
**     Method      :  peTimer_EnableEvent (component TimerInt)
**
**     Description :
**         This method enables the events.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define peTimer_DisableEvent() (peTimer_EnEvent = FALSE, (byte)ERR_OK) /* Set the flag "events disabled" */
/*
** ===================================================================
**     Method      :  peTimer_DisableEvent (component TimerInt)
**
**     Description :
**         This method disables the events.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

__interrupt void peTimer_Interrupt(void);
/*
** ===================================================================
**     Method      :  peTimer_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void peTimer_Init(void);
/*
** ===================================================================
**     Method      :  peTimer_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END peTimer. */

#endif /* ifndef __peTimer */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
