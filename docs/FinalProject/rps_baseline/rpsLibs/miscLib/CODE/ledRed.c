/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : ledRed.c
**     Project   : miscLib
**     Processor : MCF51CN128CLK
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.13, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:40 PM
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       78            |  PTJ3_FB_A2_FB_AD2
**             ----------------------------------------------------
**
**         Port name                   : PTJ
**
**         Bit number (in port)        : 3
**         Bit mask of the port        : 0x0008
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTJD      [0xFFFF8080]
**         Port control register       : PTJDD     [0xFFFF8081]
**
**         Optimization for            : speed
**     Contents  :
**         GetVal - bool ledRed_GetVal(void);
**         ClrVal - void ledRed_ClrVal(void);
**         SetVal - void ledRed_SetVal(void);
**         NegVal - void ledRed_NegVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE ledRed. */

#include "ledRed.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  ledRed_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool ledRed_GetVal(void)

**  This method is implemented as a macro. See ledRed.h file.  **
*/

/*
** ===================================================================
**     Method      :  ledRed_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void ledRed_ClrVal(void)

**  This method is implemented as a macro. See ledRed.h file.  **
*/

/*
** ===================================================================
**     Method      :  ledRed_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void ledRed_SetVal(void)

**  This method is implemented as a macro. See ledRed.h file.  **
*/

/*
** ===================================================================
**     Method      :  ledRed_NegVal (component BitIO)
**
**     Description :
**         This method negates (inverts) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void ledRed_NegVal(void)

**  This method is implemented as a macro. See ledRed.h file.  **
*/


/* END ledRed. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
