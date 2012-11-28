/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : lcdBKLITE.h
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
**                       71            |  PTE4_KBI2P4_CLKOUT_TPM1CH1
**             ----------------------------------------------------
**
**         Port name                   : PTE
**
**         Bit number (in port)        : 4
**         Bit mask of the port        : 0x0010
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTED      [0xFFFF8040]
**         Port control register       : PTEDD     [0xFFFF8041]
**
**         Optimization for            : speed
**     Contents  :
**         GetVal - bool lcdBKLITE_GetVal(void);
**         PutVal - void lcdBKLITE_PutVal(bool Val);
**         ClrVal - void lcdBKLITE_ClrVal(void);
**         SetVal - void lcdBKLITE_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef lcdBKLITE_H_
#define lcdBKLITE_H_

/* MODULE lcdBKLITE. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  lcdBKLITE_GetVal (component BitIO)
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
#define lcdBKLITE_GetVal() ( \
    (bool)((getReg8(PTED) & 0x10))     /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  lcdBKLITE_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void lcdBKLITE_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  lcdBKLITE_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define lcdBKLITE_ClrVal() ( \
    (void)clrReg8Bits(PTED, 0x10)      /* PTED4=0x00 */ \
  )

/*
** ===================================================================
**     Method      :  lcdBKLITE_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define lcdBKLITE_SetVal() ( \
    (void)setReg8Bits(PTED, 0x10)      /* PTED4=0x01 */ \
  )



/* END lcdBKLITE. */
#endif /* #ifndef __lcdBKLITE_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
