/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : ModulusTimer.c
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Component : TimerInt
**     Version   : Component 02.158, Driver 01.20, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/7/2010, 11:14 AM
**     Abstract  :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : TPM1 (16-bit)
**         Compare name                : TPM12
**         Counter shared              : Yes
**
**         High speed mode
**             Prescaler               : divide-by-4
**             Clock                   : 6000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 4
**             microseconds            : 125
**             seconds (real)          : 0.000125
**             Hz                      : 8000
**             kHz                     : 8
**
**         Runtime setting             : period/frequency interval (continual setting)
**             ticks                   : 1 to 3 ticks
**             microseconds            : 23 to 125 microseconds
**             seconds (real)          : 0.000022166667 to 0.000125 seconds
**             Hz                      : 8000 to 45112 Hz
**             kHz                     : 8 to 45 kHz
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
**              Compare                : TPM1C2V   [0xFFFF826C]
**
**         Flip-flop registers
**              Mode                   : TPM1C2SC  [0xFFFF826B]
**     Contents  :
**         Enable       - byte ModulusTimer_Enable(void);
**         Disable      - byte ModulusTimer_Disable(void);
**         EnableEvent  - byte ModulusTimer_EnableEvent(void);
**         DisableEvent - byte ModulusTimer_DisableEvent(void);
**         SetFreqHz    - byte ModulusTimer_SetFreqHz(word Freq);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE ModulusTimer. */

#include "Events.h"
#include "PE_Error.h"
#include "ModulusTimer.h"

static bool EnUser;                    /* Enable device by user */
bool ModulusTimer_EnEvent;             /* Enable/Disable events */
static word CmpVal;                    /* Value added to compare register in ISR */
static word CmpHighVal;                /* Compare register value for high speed CPU mode */
/*** Internal macros and method prototypes ***/

/*
** ===================================================================
**     Method      :  SetCV (component TimerInt)
**
**     Description :
**         The method computes and sets compare eventually modulo value 
**         for time measuring.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ModulusTimer_SetCV(word Val);

/*
** ===================================================================
**     Method      :  HWEnDi (component TimerInt)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void);


/*** End of internal method prototypes ***/

/*
** ===================================================================
**     Method      :  HWEnDi (component TimerInt)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  word TmpCmpVal;                      /* Temporary variable for compare value */

  if (EnUser) {
    TmpCmpVal = (word)(TPM1CNT + CmpVal); /* Count current value for the compare register */
    TPM1C2V = TmpCmpVal;               /* Set compare register */
    while (TPM1C2V != TmpCmpVal) {}    /* Wait for register update (because of Latching mechanism) */
    /* TPM1C2SC: CH2F=0 */
    clrReg8Bits(TPM1C2SC, 0x80);       /* Reset request flag */ 
    /* TPM1C2SC: CH2IE=1 */
    setReg8Bits(TPM1C2SC, 0x40);       /* Enable compare interrupt */ 
  } else {
    /* TPM1C2SC: CH2IE=0 */
    clrReg8Bits(TPM1C2SC, 0x40);       /* Disable Compare interrupt */ 
  }
}

/*
** ===================================================================
**     Method      :  ModulusTimer_Enable (component TimerInt)
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
byte ModulusTimer_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    HWEnDi();                          /* Enable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ModulusTimer_Disable (component TimerInt)
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
byte ModulusTimer_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    HWEnDi();                          /* Disable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ModulusTimer_EnableEvent (component TimerInt)
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
/*
byte ModulusTimer_EnableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  ModulusTimer_DisableEvent (component TimerInt)
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
/*
byte ModulusTimer_DisableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  ModulusTimer_SetFreqHz (component TimerInt)
**
**     Description :
**         This method sets the new frequency of the generated
**         events. The frequency is expressed in Hz as a 16-bit
**         unsigned integer number.
**         This method is available only if the runtime setting type
**         'from interval' is selected in the Timing dialog box in
**         the Runtime setting area.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Freq            - Frequency to set [in Hz]
**                      (8000 to 45112 Hz)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte ModulusTimer_SetFreqHz(word Freq)
{
  dlong rtval;                         /* Result of two 32-bit numbers division */
  word rtword;                         /* Result of 64-bit number division */

  if ((Freq > 0xB038) || (Freq < 0x1F40)) { /* Is the given value out of range? */
    return ERR_RANGE;                  /* If yes then error */
  }
  rtval[1] = 0x5B8D8000 / (dword)Freq; /* Divide high speed CPU mode coefficient by the given value */
  rtval[0] = 0x00;                     /* Convert result to the type dlong */
  if (PE_Timer_LngHi1(rtval[0], rtval[1], &rtword)){ /* Is the result greater or equal than 65536 ? */
    rtword = 0xFFFF;                   /* If yes then use maximal possible value */
  }
  CmpHighVal = rtword;                 /* Store result (compare register value for high speed CPU mode) to the variable CmpHighVal */
  ModulusTimer_SetCV((word)CmpHighVal); /* Store appropriate value to the compare register according to the selected high speed CPU mode */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  ModulusTimer_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void ModulusTimer_Init(void)
{
  CmpHighVal = 0x02EE;                 /* Remember appropriate value of compare register for high speed CPU mode */
  /* TPM1C2SC: CH2F=0,CH2IE=0,MS2B=0,MS2A=1,ELS2B=0,ELS2A=0,??=0,??=0 */
  setReg8(TPM1C2SC, 0x10);             /* Set output compare mode and disable compare interrupt */ 
  EnUser = FALSE;                      /* Disable device */
  ModulusTimer_EnEvent = FALSE;        /* Disable event */
  ModulusTimer_SetCV(0x02EEU);         /* Inicialize appropriate value to the compare/modulo/reload register */
  ModulusTimer_SetCV((word)CmpHighVal); /* Store appropriate value to the compare/modulo/reload register according to High speed CPU mode */
  HWEnDi();
}


/*
** ===================================================================
**     Method      :  ModulusTimer_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(ModulusTimer_Interrupt)
{
  /* TPM1C2SC: CH2F=0 */
  clrReg8Bits(TPM1C2SC, 0x80);         /* Reset compare interrupt request flag */ 
  TPM1C2V += CmpVal;                   /* Set new value to the compare register */
  if (ModulusTimer_EnEvent) {          /* Are the events enabled? */
    ModulusTimer_OnInterrupt();        /* Invoke user event */
  }
}


/*
** ===================================================================
**     Method      :  SetCV (component TimerInt)
**
**     Description :
**         The method computes and sets compare eventually modulo value 
**         for time measuring.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void ModulusTimer_SetCV(word Val)
{
  TPM1C2V = (word)(TPM1CNT + Val);     /* Count and save new value to the compare register */
  CmpVal = Val;                        /* Store result to the variable CmpVal */
}


/* END ModulusTimer. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
