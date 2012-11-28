/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : sndPWM.h
**     Project   : sndLib
**     Processor : MCF51CN128CLK
**     Component : PWM
**     Version   : Component 02.224, Driver 01.26, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:09 PM
**     Abstract  :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings  :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       40            |  PTH7_FB_A9_FB_AD9_TPM2CH2
**             ----------------------------------------------------
**
**         Timer name                  : TPM2 [16-bit]
**         Counter                     : TPM2CNT   [0xFFFF8281]
**         Mode register               : TPM2SC    [0xFFFF8280]
**         Run register                : TPM2SC    [0xFFFF8280]
**         Prescaler                   : TPM2SC    [0xFFFF8280]
**         Compare register            : TPM2C2V   [0xFFFF828C]
**         Flip-flop register          : TPM2C2SC  [0xFFFF828B]
**
**         Priority                    : 540
**         User handling procedure     : sndPWM_OnEnd
**         This event is called when the 1 of cycles is generated
**
**         Port name                   : PTH
**         Bit number (in port)        : 7
**         Bit mask of the port        : 0x0080
**         Port data register          : PTHD      [0xFFFF8070]
**         Port control register       : PTHDD     [0xFFFF8071]
**
**         Initialization:
**              Output level           : low
**              Timer                  : Disabled
**              Event                  : Disabled
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 24000000 Hz
**           Initial value of            period     pulse width
**             microseconds            : 15         15
**             seconds (real)          : 0.000015375 0.000015
**
**     Contents  :
**         Enable       - byte sndPWM_Enable(void);
**         Disable      - byte sndPWM_Disable(void);
**         EnableEvent  - byte sndPWM_EnableEvent(void);
**         DisableEvent - byte sndPWM_DisableEvent(void);
**         SetRatio8    - byte sndPWM_SetRatio8(byte Ratio);
**         SetRatio16   - byte sndPWM_SetRatio16(word Ratio);
**         SetDutyUS    - byte sndPWM_SetDutyUS(word Time);
**         SetDutyMS    - byte sndPWM_SetDutyMS(word Time);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __sndPWM
#define __sndPWM

/* MODULE sndPWM. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"

#define sndPWM_PERIOD_VALUE              0x0170 /* Initial period value in ticks of the timer in high speed mode */
#define sndPWM_PERIOD_VALUE_HIGH         0x0170 /* Period value in ticks of the timer in high speed mode */


extern bool sndPWM_EnEvent;            /* Enable/Disable events */

byte sndPWM_Enable(void);
/*
** ===================================================================
**     Method      :  sndPWM_Enable (component PWM)
**
**     Description :
**         This method enables the component - it starts the signal
**         generation. Events may be generated (<DisableEvent>
**         /<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte sndPWM_Disable(void);
/*
** ===================================================================
**     Method      :  sndPWM_Disable (component PWM)
**
**     Description :
**         This method disables the component - it stops the signal
**         generation and events calling. When the timer is disabled,
**         it is possible to call <ClrValue> and <SetValue> methods.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define sndPWM_EnableEvent() (sndPWM_EnEvent = TRUE, (byte)ERR_OK) /* Set the flag "events enabled" */
/*
** ===================================================================
**     Method      :  sndPWM_EnableEvent (component PWM)
**
**     Description :
**         This method enables the events. The method is available only
**         if any event is selected.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

#define sndPWM_DisableEvent() (sndPWM_EnEvent = FALSE, (byte)ERR_OK) /* Set the flag "events disabled" */
/*
** ===================================================================
**     Method      :  sndPWM_DisableEvent (component PWM)
**
**     Description :
**         This method disables the events. The method is available
**         only if any event is selected.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte sndPWM_SetRatio8(byte Ratio);
/*
** ===================================================================
**     Method      :  sndPWM_SetRatio8 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as an 8-bit unsigned integer number. 0 - FF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         <Starting pulse width> property. 
**         Note: Calculated duty depends on the timer capabilities and
**         on the selected period.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 255 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte sndPWM_SetRatio16(word Ratio);
/*
** ===================================================================
**     Method      :  sndPWM_SetRatio16 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. 0 - FFFF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         <Starting pulse width> property. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte sndPWM_SetDutyUS(word Time);
/*
** ===================================================================
**     Method      :  sndPWM_SetDutyUS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal.
**         The duty is expressed in microseconds as a 16-bit
**         unsigned integer number.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Time            - Duty to set [in microseconds]
**                      (0 to 15 us in high speed mode)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

byte sndPWM_SetDutyMS(word Time);
/*
** ===================================================================
**     Method      :  sndPWM_SetDutyMS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal.
**         The duty is expressed in milliseconds as a 16-bit
**         unsigned integer number.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Time            - Duty to set [in milliseconds]
**         Note: The period is too short. The method will return
**               just the error code in high speed mode.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

void sndPWM_Init(void);
/*
** ===================================================================
**     Method      :  sndPWM_Init (component PWM)
**
**     Description :
**         Initializes the associated peripheral(s) and the components 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

__interrupt void sndPWM_Interrupt(void);
/*
** ===================================================================
**     Method      :  sndPWM_Interrupt (component PWM)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

/* END sndPWM. */

#endif /* ifndef __sndPWM */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
