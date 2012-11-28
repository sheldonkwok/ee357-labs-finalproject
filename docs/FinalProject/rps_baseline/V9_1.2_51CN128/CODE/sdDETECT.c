/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : sdDETECT.c
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.13, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/7/2010, 11:14 AM
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Input direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       69            |  PTE2_KBI2P2_SS2_ADP0
**             ----------------------------------------------------
**
**         Port name                   : PTE
**
**         Bit number (in port)        : 2
**         Bit mask of the port        : 0x0004
**
**         Initial direction           : Input (direction cannot be changed)
**         Initial output value        : 1
**         Initial pull option         : up
**
**         Port data register          : PTED      [0xFFFF8040]
**         Port control register       : PTEDD     [0xFFFF8041]
**
**         Optimization for            : speed
**     Contents  :
**         GetVal - bool sdDETECT_GetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE sdDETECT. */

#include "sdDETECT.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  sdDETECT_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Input direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool sdDETECT_GetVal(void)

**  This method is implemented as a macro. See sdDETECT.h file.  **
*/


/* END sdDETECT. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
