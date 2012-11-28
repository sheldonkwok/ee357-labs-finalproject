/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : grphSI.c
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.13, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/7/2010, 11:14 AM
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
**                       7             |  PTA4_MII_RXD2_RXD3
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 4
**         Bit mask of the port        : 0x0010
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 1
**         Initial pull option         : up
**
**         Port data register          : PTAD      [0xFFFF8000]
**         Port control register       : PTADD     [0xFFFF8001]
**
**         Optimization for            : speed
**     Contents  :
**         ClrVal - void grphSI_ClrVal(void);
**         SetVal - void grphSI_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE grphSI. */

#include "grphSI.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  grphSI_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void grphSI_ClrVal(void)

**  This method is implemented as a macro. See grphSI.h file.  **
*/

/*
** ===================================================================
**     Method      :  grphSI_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void grphSI_SetVal(void)

**  This method is implemented as a macro. See grphSI.h file.  **
*/


/* END grphSI. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
