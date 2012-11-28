/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : rtcINT.h
**     Project   : flashLib
**     Processor : MCF51CN128CLK
**     Component : ExtInt
**     Version   : Component 02.094, Driver 01.22, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 3:47 PM
**     Abstract  :
**         This component "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The bean uses one pin which generates interrupt on 
**         selected edge.
**     Settings  :
**         Interrupt name              : Virq
**         User handling procedure     : rtcINT_OnInterrupt
**
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       45            |  PTC4_IRQ_SS1_ADP11
**             ----------------------------------------------------
**
**         Port name                   : PTC
**
**         Bit number (in port)        : 4
**         Bit mask of the port        : 0x0010
**
**         Signal edge/level           : falling
**         Priority                    : 735
**         Pull option                 : off
**         Initial state               : Enabled
**
**         Edge register               : IRQSC     [0xFFFF80E0]
**         Enable register             : IRQSC     [0xFFFF80E0]
**         Request register            : IRQSC     [0xFFFF80E0]
**
**         Port data register          : PTCD      [0xFFFF8020]
**         Port control register       : PTCDD     [0xFFFF8021]
**     Contents  :
**         Enable  - void rtcINT_Enable(void);
**         Disable - void rtcINT_Disable(void);
**         GetVal  - bool rtcINT_GetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __rtcINT_H
#define __rtcINT_H

/* MODULE rtcINT. */



/*Including shared modules, which are used in the whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Events.h"
#include "Cpu.h"


#define rtcINT_Enable() \
  (IRQSC_IRQACK = 0x01,IRQSC_IRQIE = 0x01)
/*
** ===================================================================
**     Method      :  rtcINT_Enable (component ExtInt)
**
**     Description :
**         Enable the component - the external events are accepted.
**         This method is available only if HW module allows
**         enable/disable of the interrupt.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define rtcINT_Disable() \
  (IRQSC_IRQIE = 0x00)

/*
** ===================================================================
**     Method      :  rtcINT_Disable (component ExtInt)
**
**     Description :
**         Disable the component - the external events are not accepted.
**         This method is available only if HW module allows
**         enable/disable of the interrupt.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

__interrupt void rtcINT_Interrupt(void);
/*
** ===================================================================
**     Method      :  rtcINT_Interrupt (component ExtInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the components event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#define rtcINT_GetVal() \
  ((bool)((PTCD) & 0x10))

/*
** ===================================================================
**     Method      :  rtcINT_GetVal (component ExtInt)
**
**     Description :
**         Returns the actual value of the input pin of the component.
**     Parameters  : None
**     Returns     :
**         ---             - Returned input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
** ===================================================================
*/
#endif /* __rtcINT_H*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/