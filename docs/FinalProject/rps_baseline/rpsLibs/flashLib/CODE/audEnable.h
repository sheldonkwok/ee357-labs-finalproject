/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : audEnable.h
**     Project   : flashLib
**     Processor : MCF51CN128CLK
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.13, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 3:47 PM
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       74            |  PTE7_KBI2P7_FB_CS0_RXD3
**             ----------------------------------------------------
**
**         Port name                   : PTE
**
**         Bit number (in port)        : 7
**         Bit mask of the port        : 0x0080
**
**         Initial direction           : Output (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTED      [0xFFFF8040]
**         Port control register       : PTEDD     [0xFFFF8041]
**
**         Optimization for            : speed
**     Contents  :
**         SetDir - void audEnable_SetDir(bool Dir);
**         GetVal - bool audEnable_GetVal(void);
**         PutVal - void audEnable_PutVal(bool Val);
**         ClrVal - void audEnable_ClrVal(void);
**         SetVal - void audEnable_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef audEnable_H_
#define audEnable_H_

/* MODULE audEnable. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  audEnable_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define audEnable_GetVal() ( \
    (bool)((getReg8(PTED) & 0x80))     /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  audEnable_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void audEnable_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  audEnable_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define audEnable_ClrVal() ( \
    (void)clrReg8Bits(PTED, 0x80)      /* PTED7=0x00 */, \
    Shadow_PTE &= (byte)~0x80          /* Set appropriate bit in shadow variable */ \
  )

/*
** ===================================================================
**     Method      :  audEnable_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define audEnable_SetVal() ( \
    (void)setReg8Bits(PTED, 0x80)      /* PTED7=0x01 */, \
    Shadow_PTE |= (byte)0x80           /* Set appropriate bit in shadow variable */ \
  )

/*
** ===================================================================
**     Method      :  audEnable_SetDir (component BitIO)
**
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void audEnable_SetDir(bool Dir);



/* END audEnable. */
#endif /* #ifndef __audEnable_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
