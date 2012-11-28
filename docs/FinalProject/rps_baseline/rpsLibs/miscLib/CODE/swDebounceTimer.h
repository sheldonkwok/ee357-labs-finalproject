/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : swDebounceTimer.h
**     Project   : miscLib
**     Processor : MCF51CN128CLK
**     Component : TimerInt
**     Version   : Component 02.158, Driver 01.20, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:40 PM
**     Abstract  :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM11
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
**              Compare                : TPM1C1V   [0xFFFF8269]
**
**         Flip-flop registers
**              Mode                   : TPM1C1SC  [0xFFFF8268]
**     Contents  :
**         Enable       - byte swDebounceTimer_Enable(void);
**         Disable      - byte swDebounceTimer_Disable(void);
**         EnableEvent  - byte swDebounceTimer_EnableEvent(void);
**         DisableEvent - byte swDebounceTimer_DisableEvent(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __swDebounceTimer
#define __swDebounceTimer

/* MODULE swDebounceTimer. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"
extern bool swDebounceTimer_EnEvent;   /* Enable/Disable events */


byte swDebounceTimer_Enable(void);
/*
** ===================================================================
**     Method      :  swDebounceTimer_Enable (component TimerInt)
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

byte swDebounceTimer_Disable(void);
/*
** ===================================================================
**     Method      :  swDebounceTimer_Disable (component TimerInt)
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

#define swDebounceTimer_EnableEvent() (swDebounceTimer_EnEvent = TRUE, (byte)ERR_OK) /* Set the flag "events enabled" */
/*
** ===================================================================
**     Method      :  swDebounceTimer_EnableEvent (component TimerInt)
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

#define swDebounceTimer_DisableEvent() (swDebounceTimer_EnEvent = FALSE, (byte)ERR_OK) /* Set the flag "events disabled" */
/*
** ===================================================================
**     Method      :  swDebounceTimer_DisableEvent (component TimerInt)
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

__interrupt void swDebounceTimer_Interrupt(void);
/*
** ===================================================================
**     Method      :  swDebounceTimer_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void swDebounceTimer_Init(void);
/*
** ===================================================================
**     Method      :  swDebounceTimer_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END swDebounceTimer. */

#endif /* ifndef __swDebounceTimer */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
