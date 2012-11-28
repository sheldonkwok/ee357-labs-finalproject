/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : sndPWM.c
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Component : PWM
**     Version   : Component 02.224, Driver 01.26, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/7/2010, 11:14 AM
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


/* MODULE sndPWM. */

#include "Events.h"
#include "PE_Error.h"
#include "sndPWM.h"


bool sndPWM_EnEvent;                   /* Enable/Disable events */
static word RatioStore;                /* Ratio of L-level to H-level */

/* Internal method prototypes */

/*
** ===================================================================
**     Method      :  SetRatio (component PWM)
**
**     Description :
**         The method stores duty value to compare register(s) and sets 
**         necessary bits or (in List mode) call SetReg method for duty 
**         value storing.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void SetRatio(void);

/* End of Internal methods declarations */

/*
** ===================================================================
**     Method      :  SetRatio (component PWM)
**
**     Description :
**         The method stores duty value to compare register(s) and sets 
**         necessary bits or (in List mode) call SetReg method for duty 
**         value storing.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void SetRatio(void)
{
  if (RatioStore == 0xFFFFU) {         /* Duty = 100%? */
    TPM2C2V = 0xFFFFU;                 /* Store new value to the compare reg. */
  } else {
    TPM2C2V = (word)(((0x0171UL * (dword)RatioStore)  + 0x8000) >> 0x10); /* Calculate new compare value according to the given ratio */
  }
}

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
byte sndPWM_Enable(void)
{
  /* TPM2SC: TOF=0,TOIE=1,CPWMS=0,CLKSB=0,CLKSA=1,PS2=0,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x48);               /* Run the counter (set CLKSB:CLKSA) */ 
  /* PTHPF1: H7=3 */
  setReg8Bits(PTHPF1, 0xC0);            
  return ERR_OK;                       /* OK */
}

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
byte sndPWM_Disable(void)
{
  /* TPM2SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x00);               /* Stop counter (CLKSB:CLKSA = 00) */ 
  /* TPM2CNTH: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0 */
  setReg8(TPM2CNTH, 0x00);             /* Reset HW Counter */ 
  /* PTHPF1: H7=0 */
  clrReg8Bits(PTHPF1, 0xC0);            
  return ERR_OK;                       /* OK */
}

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
/*
byte sndPWM_EnableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

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
/*
byte sndPWM_DisableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

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
byte sndPWM_SetRatio8(byte Ratio)
{
  ((byte*)&RatioStore)[0] = Ratio;     /* Store new value of the ratio */
  ((byte*)&RatioStore)[1] = Ratio;
  SetRatio();                          /* Calculate and set up new appropriate values of the compare and modulo registers */
  return ERR_OK;                       /* OK */
}

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
byte sndPWM_SetRatio16(word Ratio)
{
  RatioStore = Ratio;                  /* Store new value of the ratio */
  SetRatio();                          /* Calculate and set up new appropriate values of the compare and modulo registers */
  return ERR_OK;                       /* OK */
}

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
byte sndPWM_SetDutyUS(word Time)
{
  dlong rtval;                         /* Result of two 32-bit numbers multiplication */
  if (Time > 0x0FU) {                  /* Is the given value out of range? */
    return ERR_RANGE;                  /* If yes then error */
  }
  PE_Timer_LngMul((dword)Time, 0x10A6810ALU, &rtval); /* Multiply given value and High speed CPU mode coefficient */
  if (PE_Timer_LngHi2(rtval[0], rtval[1], &RatioStore)) { /* Is the result greater or equal than 65536 ? */
    RatioStore = 0xFFFFU;              /* If yes then use maximal possible value */
  }
  SetRatio();                          /* Calculate and set up new appropriate values of the compare and modulo registers */
  return ERR_OK;                       /* OK */
}

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
byte sndPWM_SetDutyMS(word Time)
{
    #pragma unused (Time)
  /* Period is too short. The method SetDutyMS will return an error code only in High speed mode. */
  return ERR_MATH;                     /* Calculation error */
}

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
void sndPWM_Init(void)
{
  /* TPM2SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x00);               /* Disable device */ 
  /* TPM2C2SC: CH2F=0,CH2IE=0,MS2B=1,MS2A=1,ELS2B=1,ELS2A=1,??=0,??=0 */
  setReg8(TPM2C2SC, 0x3C);             /* Set up PWM mode with output signal level low */ 
  RatioStore = 0xF9C2U;                /* Store initial value of the ratio */
  sndPWM_EnEvent = FALSE;              /* Disable events */
  /* TPM2MOD: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=1,BIT7=0,BIT6=1,BIT5=1,BIT4=1,BIT3=0,BIT2=0,BIT1=0,BIT0=0 */
  setReg16(TPM2MOD, 0x0170U);          /* Set modulo register */ 
  SetRatio();                          /* Calculate and set up new values of the compare according to the selected speed CPU mode */
  /* TPM2SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
  setReg8(TPM2SC, 0x00);               /* Stop counter (CLKSB:CLKSA = 00) */ 
  /* TPM2CNTH: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0 */
  setReg8(TPM2CNTH, 0x00);             /* Reset HW Counter */ 
  /* PTHPF1: H7=0 */
  clrReg8Bits(PTHPF1, 0xC0);            
}


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
ISR(sndPWM_Interrupt)
{
  (void)getReg8(TPM2SC);               /* Dummy read of the TPM2SC register to reset flag */
  /* TPM2SC: TOF=0 */
  clrReg8Bits(TPM2SC, 0x80);           /*  Reset interrupt request flag */ 
  if (sndPWM_EnEvent) {                /* Are the events enabled? */
    sndPWM_OnEnd();                    /* Invoke user event */
  }
}


/* END sndPWM. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
