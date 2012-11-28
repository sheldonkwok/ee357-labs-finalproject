#include "RPS_BldOptions.h"

#include "TypeDef.h"

#include "sdEnumDef.h"
#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "TimerDelay.h"

#include "lcdConstDef.h"
#include "rtcConstDef.h"

#include "lcdStructDef.h"
#include "DeviceRegStructDef.h"
#include "ExtTimeDeclar.h"

#include "ExtGlobalVarDeclar.h"

#include "RxTx_spi.h"

#include "sdCmd.h"
#include "sdFileSys.h"

#include "FcnDef.h"
#include "sdFcnDef.h"

#include "lcdFcnDef.h"

/*============================================================================================================
                                $RCSfile: Init.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.11 $
       $Date: 2007/06/15 18:38:30 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/Init.c,v $

=============================================================================================================*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                                Init Rtns, Modules &constants
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*-------------------------------------InitSetup---------------------------------------------------
InitSetup
    Function:           Init devices 
    Input Parameter     none
    Return Type:        none
    
    Notes:
  ------------------------------------------------------------------------------------------------*/
void InitSetup (void)
{
    vFn_lcdBkLite(ge_LED_OFF);

    ledIndicator                                        // explicitly set LED's off at start up
        (ge_LED_ON,
         ge_LED_RED,
         NULL_CHAR);

    ledIndicator
        (ge_LED_ON,
         ge_LED_GREEN,
         NULL_CHAR);

    TimeDelay(ge_T_1_SEC);                                 // wait so a spin loop is not incountered

    ledIndicator                                        // explicitly set LED's off at start up
        (ge_LED_OFF,
         ge_LED_RED,
         NULL_CHAR);

    ledIndicator
        (ge_LED_OFF,
         ge_LED_GREEN,
         NULL_CHAR);

    TimeDelay(ge_T_1_SEC);                              // wait so a spin loop is not incountered

    u8Fn_grphSetFont(FONT_NINE_DOT);                    // init a default grph font

    grphInit();                                         // initialize LCD display
    grphInit();                                         // initialize LCD display, // *rwl debug why two are required
    grphErase();                                        // clear

    vFn_rtcAlarmOper(ge_RTC_ALARM_DISABLE_0);           // disable alarm int
                                                        //  to account for startup state alarms of alarms on
    sdInitialize();

    gu8_swDebounceTimerFlag = ge_SW_DEBOUNCE_COMPLETE;  // updated in Events.c
    gs16_swDebounceTimerCntr = 0;                       // loop cntr used in Events.c, use signed

//rwl     kbiPort_EnableEvent();                              // allow port interrupts after Radio Init is done

    return;
}                                                       //end-fcn_InitSetup


