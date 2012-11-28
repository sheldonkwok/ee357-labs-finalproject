/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : TimerDelay.h
**     Project   : scriptLib
**     Processor : MCF51CN128CLK
**     Component : TimerInt
**     Version   : Component 02.158, Driver 01.20, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:23 PM
**     Abstract  :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM10
**         Counter shared              : Yes
**
**         High speed mode
**             Prescaler               : divide-by-4
**             Clock                   : 6000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 312
**             microseconds            : 10000
**             milliseconds            : 10
**             seconds (real)          : 0.01
**             Hz                      : 100
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Disabled
**              Events                 : Disabled
**
**         Timer registers
**              Counter                : TPM1CNT   [0xFFFF8261]
**              Mode                   : TPM1SC    [0xFFFF8260]
**              Run                    : TPM1SC    [0xFFFF8260]
**              Prescaler              : TPM1SC    [0xFFFF8260]
**
**         Compare registers
**              Compare                : TPM1C0V   [0xFFFF8266]
**
**         Flip-flop registers
**              Mode                   : TPM1C0SC  [0xFFFF8265]
**     Contents  :
**         Enable       - byte TimerDelay_Enable(void);
**         Disable      - byte TimerDelay_Disable(void);
**         EnableEvent  - byte TimerDelay_EnableEvent(void);
**         DisableEvent - byte TimerDelay_DisableEvent(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __TimerDelay
#define __TimerDelay

/* MODULE TimerDelay. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"
extern bool TimerDelay_EnEvent;        /* Enable/Disable events */


byte TimerDelay_Enable(void);
/*
** ===================================================================
**     Method      :  TimerDelay_Enable (component TimerInt)
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

byte TimerDelay_Disable(void);
/*
** ===================================================================
**     Method      :  TimerDelay_Disable (component TimerInt)
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

#define TimerDelay_EnableEvent() (TimerDelay_EnEvent = TRUE, (byte)ERR_OK) /* Set the flag "events enabled" */
/*
** ===================================================================
**     Method      :  TimerDelay_EnableEvent (component TimerInt)
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

#define TimerDelay_DisableEvent() (TimerDelay_EnEvent = FALSE, (byte)ERR_OK) /* Set the flag "events disabled" */
/*
** ===================================================================
**     Method      :  TimerDelay_DisableEvent (component TimerInt)
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

__interrupt void TimerDelay_Interrupt(void);
/*
** ===================================================================
**     Method      :  TimerDelay_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void TimerDelay_Init(void);
/*
** ===================================================================
**     Method      :  TimerDelay_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END TimerDelay. */

#endif /* ifndef __TimerDelay */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
