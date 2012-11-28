/** ###################################################################
**     Filename  : V9_1_2_51CN128.C
**     Project   : V9_1_2_51CN128
**     Processor : MCF51CN128CLK
**     Version   : Driver 01.00
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 6/8/2010, 6:22 PM
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE V9_1_2_51CN128 */

//===============================================================================================
//      $Date: 2010-08-16 16:22:43 -0700 (Mon, 16 Aug 2010) $
//      $Revision: 65 $
//      $Author: Robert Lewis $
//      $HeadURL: file:///K:/Sync%20Repo/r05.0_09_jun_2010_RPS_V9_1.2/trunk/V9_1.2_51CN128/CODE/V9_1_2_51CN128.c $
//===============================================================================================

#include "Cpu.h"
#include "Events.h"
#include "TimerDelay.h"
#include "RxTx_spi.h"
#include "ledGreen.h"
#include "ledRed.h"
#include "lcdBKLITE.h"
#include "sdCS.h"
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
//rwl #include "SCI.h"
//rwl #include "Inhr1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"

#include "PE_Const.h"
#include "IO_Map.h"

//===============================================================================================
#include "RPS_BldOptions.h"
#include <string.h>

#include "TypeDef.h"
#include "fontConstDef.h"
#include "lcdConstDef.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "SwitchStructDef.h"

#include "lcdStructDef.h"
#include "DeviceRegStructDef.h"

#include "GlobalVarDef.h"

#include "sdEnumDef.h"

#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdFcnDef.h"
#include "FcnDef.h"
#include "sdCmd.h"

//#include "rpsWCRev.h"               // latest bld information from SVN
char *Revision      = "2";
char *Modified      = "Modified";
char *BldTime       = "2010/06/10 11:46:09";
char *CommitDate    = "2010/06/09 17:36:42";
char *Range         = "2";
char *Mixed         = "Not mixed";
char *URL           = "file:///K:/Sync Repo/r05.0_09_jun_2010_RPS_V9_1.2/trunk/common";


//----------------------------------------------------------------------------------------------
File_iMn                     File1;
FilePtr                      FilePtr1;

//----------------------------------------------------------------------------------------------
TimeDateStruct               SystemTimeTable;     

TimeDateStructPtr            SystemTimeTablePtr;
//----------------------------------------------------------------------------------------------

SwStatusStructPtr            swStatusFlagsPtr;
SwStatusStruct               swStatusFlags;
//----------------------------------------------------------------------------------------------

lcdTimeDateStaticStructPtr   lcdTimeDateStaticPtr;
lcdTimeDateDynStructPtr      lcdTimeDateDynPtr;

#include "lcdTimeDateStructInit.h"                      // positional information, line, and char

void        MainMenu                (void);

void        vFn_rtc_m41t81s_test    (void);
void        vFn_TestReadAlarm       (void);

void main(void)
{

uint8   query_value = 'N';

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

//------------------------------------------------------------------------------------------------
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{ 
//------------------------------------------------------------------------------------------------
    lcdTimeDateStaticPtr  = &lcdTimeDateStatic;         // init time struct ptrs
    lcdTimeDateDynPtr     = &lcdTimeDateDyn;

    SystemTimeTablePtr    = &SystemTimeTable;
    swStatusFlagsPtr      = &swStatusFlags;

    InitSetup();

    rtcStartClock(SystemTimeTablePtr);                  // start rtc, read date time, validate

    vFn_grphClrView(FontSizeCurr);                      // clean draw outline

    swResetAllFlags();                                  // clear all switch flags

    swInterruptEnable
      (ge_SW_LFT,
       ge_SW_RT,
       ge_SW_UP,
       ge_SW_DOWN,
       NULL_CHAR);

    while (1){
//rwl          vFn_TestCalibAccel();
#ifdef DEMO_TEST
        rpsDemo();
#endif

#ifdef LCD_MENU_ON
        MainMenu();                                     // progress through all fcns through menu
#endif

    }

//------------------------------------------------------------------------------------------------
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}       
//------------------------------------------------------------------------------------------------

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END V9_1_2_51CN128 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.06 [04.26]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
