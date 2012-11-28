/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : grphRS.c
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
**                       8             |  PTA5_MII_RXD1_SPSCK2
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 5
**         Bit mask of the port        : 0x0020
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTAD      [0xFFFF8000]
**         Port control register       : PTADD     [0xFFFF8001]
**
**         Optimization for            : speed
**     Contents  :
**         ClrVal - void grphRS_ClrVal(void);
**         SetVal - void grphRS_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE grphRS. */

#include "grphRS.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  grphRS_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void grphRS_ClrVal(void)

**  This method is implemented as a macro. See grphRS.h file.  **
*/

/*
** ===================================================================
**     Method      :  grphRS_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void grphRS_SetVal(void)

**  This method is implemented as a macro. See grphRS.h file.  **
*/


/* END grphRS. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/