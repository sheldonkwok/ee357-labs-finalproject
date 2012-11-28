/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : swPort.h
**     Project   : miscLib
**     Processor : MCF51CN128CLK
**     Component : BitsIO
**     Version   : Component 02.102, Driver 03.13, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:40 PM
**     Abstract  :
**         This component "BitsIO" implements a multi-bit input/output.
**         It uses selected pins of one 1-bit to 8-bit port.
**         Note: This component is set to work in Input direction only.
**     Settings  :
**         Port name                   : PTB
**
**         Bit mask of the port        : 0x003E
**         Number of bits/pins         : 5
**         Single bit numbers          : 0 to 4
**         Values range                : 0 to 31
**
**         Initial direction           : Input (direction cannot be changed)
**         Initial output value        : 0 = 000H
**         Initial pull option         : up
**
**         Port data register          : PTBD      [0xFFFF8010]
**         Port control register       : PTBDD     [0xFFFF8011]
**
**             ----------------------------------------------------
**                   Bit     |   Pin   |   Name
**             ----------------------------------------------------
**                    0      |    12   |   PTB1_MII_RX_ER_TMRCLK1
**                    1      |    23   |   PTB2_MII_TX_ER_SS1
**                    2      |    24   |   PTB3_MII_TX_CLK_MOSI1
**                    3      |    25   |   PTB4_MII_TX_EN_MISO1
**                    4      |    26   |   PTB5_MII_TXD0_SPSCK1
**             ----------------------------------------------------
**
**         Optimization for            : speed
**     Contents  :
**         GetDir - bool swPort_GetDir(void);
**         GetVal - byte swPort_GetVal(void);
**         GetBit - bool swPort_GetBit(byte Bit);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef swPort_H_
#define swPort_H_

/* MODULE swPort. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  swPort_GetVal (component BitsIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pins and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Input direction only.
**     Parameters  : None
**     Returns     :
**         ---        - Input value (0 to 31)
** ===================================================================
*/
byte swPort_GetVal(void);

/*
** ===================================================================
**     Method      :  swPort_GetBit (component BitsIO)
**
**     Description :
**         This method returns the specified bit of the input value.
**           a) direction = Input  : reads the input value from pins
**                                   and returns the specified bit
**           b) direction = Output : returns the specified bit
**                                   of the last written value
**         Note: This component is set to work in Input direction only.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Bit        - Number of the bit to read (0 to 4)
**     Returns     :
**         ---        - Value of the specified bit (FALSE or TRUE)
**                      FALSE = "0" or "Low", TRUE = "1" or "High"
** ===================================================================
*/
bool swPort_GetBit(byte Bit);

/*
** ===================================================================
**     Method      :  swPort_GetDir (component BitsIO)
**
**     Description :
**         This method returns direction of the component.
**     Parameters  : None
**     Returns     :
**         ---        - Direction of the component (always FALSE, Input only)
**                      FALSE = Input, TRUE = Output
** ===================================================================
*/
#define swPort_GetDir() ( \
    (bool)0                            /* Pins are fixed to GPI mode */ \
  )



/* END swPort. */
#endif /* #ifndef __swPort_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
