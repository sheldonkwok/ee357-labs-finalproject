/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : kbiPort.C
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Beantype  : KBI
**     Version   : Bean 01.089, Driver 01.19, CPU db: 3.00.014
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 6/9/2010, 3:22 PM
**     Abstract  :
**         This bean "KBI" implements the Freescale Keyboard 
**         Interrupt Module (KBI/KBD) which allows to catch events 
**         on choiced external pins. These pins share one KBI/KBD 
**         interrupt which can be caused by an events on the pins.
**     Settings  :
**         Keyboard                    : KBI1 
**         Used pins           
**         Pin 0                       : PTG7_KBI1P7_FB_D1
**         Pin 1                       : PTG3_KBI1P3_FB_A5_FB_AD5_SDA1
**         Pin 2                       : PTG2_KBI1P2_FB_A6_FB_AD6_SCL1
**         Pull resistor               : up
**         Generate interrupt on       : falling
**         Interrupt service           : Enabled
**         Interrupt                   : Vkeyboard
**         Interrupt Priority          : 120
**         Enable in init. code        : No
**         Events enabled in init.     : No
**     Contents  :
**         Enable       - void kbiPort_Enable(void);
**         Disable      - void kbiPort_Disable(void);
**         EnableEvent  - void kbiPort_EnableEvent(void);
**         DisableEvent - void kbiPort_DisableEvent(void);
**         GetVal       - byte kbiPort_GetVal(void);
**         SetEdge      - byte kbiPort_SetEdge(byte edge);
**         GetStatus    - bool kbiPort_GetStatus(void);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE kbiPort. */

#include "Events.h"
#include "kbiPort.h"


volatile bool kbiPort_EnEvent;         /* Enable/Disable events */

/*
** ===================================================================
**     Method      :  kbiPort_Enable (bean KBI)
**
**     Description :
**         Enables the bean - the external events are accepted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void kbiPort_Enable(void)

**      This method is implemented as macro. See kbiPort.h file.      **
*/

/*
** ===================================================================
**     Method      :  kbiPort_Disable (bean KBI)
**
**     Description :
**         Disables the bean - the external events are not accepted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void kbiPort_Disable(void)

**      This method is implemented as macro. See kbiPort.h file.      **
*/

/*
** ===================================================================
**     Event       :  kbiPort_OnInterrupt (module Events)
**
**     From bean   :  kbiPort [KBI]
**     Description :
**         This event is called when the active signal edge/level
**         occurs. This event is enabled only if Interrupt
**         service/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
ISR(kbiPort_Interrupt)
{
  KBI1SC_KBACK = 0x01;                 /* Clear the interrupt flag */
  if(kbiPort_EnEvent) {
    kbiPort_OnInterrupt();             /* Invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  kbiPort_GetVal (bean KBI)
**
**     Description :
**         Returns the value of pins
**     Parameters  : None
**     Returns     :
**         ---             - The value of associated pins (bits ordered
**                           according to the bean list of pins)
** ===================================================================
*/
byte kbiPort_GetVal(void)
{
  byte value = 0x00;
  value |= ((PTGD & 0x80) >> 7);
  value |= ((PTGD & 0x08) >> 2);
  value |= PTGD & 0x04;
  return value;
}

/*
** ===================================================================
**     Method      :  kbiPort_GetStatus (bean KBI)
**
**     Description :
**         Gets and clears the interrupt flag. If the <Interrupt
**         service/event> property is set to "Enabled", interrupt flag
**         is cleared in the generated interrupt service routine, but
**         this method can be used for example to clear ongoing input
**         signal glitches.
**     Parameters  : None
**     Returns     :
**         ---             - Returns true if the interrupt flag is set,
**                           otherwise returns false
** ===================================================================
*/
bool kbiPort_GetStatus(void)
{
  byte flag = (byte)(KBI1SC & 0x08);
  if (flag) {
    KBI1SC_KBACK = 0x01;               /* If the flag is set then clear it */
  }
  return flag;
}

/*
** ===================================================================
**     Method      :  kbiPort_EnableEvent (bean KBI)
**
**     Description :
**         Enables on interrupts event. This method is available only
**         if Interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void kbiPort_EnableEvent(void)

**      This method is implemented as macro. See kbiPort.h file.      **
*/

/*
** ===================================================================
**     Method      :  kbiPort_DisableEvent (bean KBI)
**
**     Description :
**         Disables on interrupts event. This method is available only
**         if Interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void kbiPort_DisableEvent(void)

**      This method is implemented as macro. See kbiPort.h file.      **
*/

/*
** ===================================================================
**     Method      :  kbiPort_SetEdge (bean KBI)
**
**     Description :
**         Sets the sensitive edge. If all selected pins don't have the
**         same edge setting possibility, the method allows to set only
**         those edges that are common (possible to set for all
**         selected pins).
**     Parameters  :
**         NAME            - DESCRIPTION
**         edge            - Edge type:
**                           0 - falling edge
**                           1 - rising edge
**                           2 - both edges
**                           3 - low level
**                           4 - high level
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_RANGE - Value is out of range
** ===================================================================
*/
byte kbiPort_SetEdge(byte edge)
{
  byte EnDi;

  if ((edge > 4) || (edge == 2)) {     /* If parameter is out of range */
    return ERR_RANGE;                  /* ....then return error */
  }
  EnDi = KBI1SC_KBIE;
  KBI1SC_KBIE = 0;                     /* Disable device */
  if ((edge == 3) || (edge == 4)) {    /* Level selected */
    KBI1SC_KBIMOD = 0x01;              /* Set the level */
    if (edge == 3) {
    /* KBI1ES: KBEDG7=0,KBEDG3=0,KBEDG2=0 */
      KBI1ES &= ~0x8C;                 /* The low level */
    }
    else {
    /* KBI1ES: KBEDG7=1,KBEDG3=1,KBEDG2=1 */
      KBI1ES |= 0x8C;                  /* The high level */
    }
  }
  else {                               /* Edge selected */
    KBI1SC_KBIMOD = 0;                 /* Set the edge */
    if (edge == 0) {
    /* KBI1ES: KBEDG7=0,KBEDG3=0,KBEDG2=0 */
      KBI1ES &= ~0x8C;                 /* The falling edge */
    }
    else {
    /* KBI1ES: KBEDG7=1,KBEDG3=1,KBEDG2=1 */
      KBI1ES |= 0x8C;                  /* The rising edge */
    }
  }
  if (EnDi) {                          /* Has to be enabled ? */
    KBI1SC_KBACK = 0x01;
    KBI1SC_KBIE = 0x01;                /* Enable device */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  kbiPort_Init (bean KBI)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void kbiPort_Init(void)
{
  kbiPort_EnEvent = FALSE;             /* Disable events */
  /* KBI1SC: ??=0,??=0,??=0,??=0,KBF=0,KBACK=0,KBIE=0,KBIMOD=0 */
  setReg8(KBI1SC, 0x00);                
  /* KBI1PE: KBIPE7=0,KBIPE3=0,KBIPE2=0 */
  KBI1PE &= ~0x8C;
  KBI1SC_KBACK = 0x01;
  KBI1SC_KBIE = 0;
}

/* END kbiPort. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.06 [04.26]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
