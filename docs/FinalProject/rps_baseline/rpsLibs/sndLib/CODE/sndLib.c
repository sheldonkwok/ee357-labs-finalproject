/** ###################################################################
**     Filename  : sndLib.C
**     Project   : sndLib
**     Processor : MCF51CN128CLK
**     Version   : Driver 01.00
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 8/11/2010, 9:33 PM
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE sndLib */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TimerDelay.h"
#include "RxTx_spi.h"
#include "sdCS.h"
#include "ledGreen.h"
#include "ledRed.h"
#include "lcdBKLITE.h"
#include "sdDETECT.h"
#include "grphRESET.h"
#include "grphSCL.h"
#include "grphRS.h"
#include "grphSI.h"
#include "ModulusTimer.h"
#include "flashCS.h"
#include "swDebounceTimer.h"
#include "audEnable.h"
#include "i2c.h"
#include "peTimer.h"
#include "swPort.h"
#include "rtcINT.h"
#include "sndPWM.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END sndLib */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.06 [04.26]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
