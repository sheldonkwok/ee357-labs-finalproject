#ifndef DEF_EXTGLOBALDECLAR_H
    #define DEF_EXTGLOBALDECLAR_H
#include "mssgConstDef.h"
#include "DeviceRegStructDef.h"
#include "rtcAlarmStructDef.h"

/*============================================================================================================
                                $RCSfile: ExtGlobalVarDeclar.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.15 $
       $Date: 2007/06/08 21:54:59 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/ExtGlobalVarDeclar.h,v $

=============================================================================================================*/
extern uint8 volatile   gu8SPI1Status;                                             /** SPI Driver Status */

//-----------------------------------------------------------------------------------------------------------
extern uint8            lcdCharAddr,
                        lcdLineAddr,
                        lcdCurrLine,
                        lcdScrollCntrl,

                        OutPutMode,

                        gu8_ChannelSide,
                        gu8_rtcFlags,

                        FontSizeCurr,
                        FontSizeLast,

                        u8_rtcAlarmIntFlag;

//--------------------------------------- Volatile and Conditional compiled variables --------------------------
extern uint8 volatile   MenuFlag,

                        RxTx_spi_Flag,
                        i2cRxFlag,
                        i2cTxFlag,
                        TimerDelayFlag,
                        gu8_peTimerFlag,
                        gu8_swDebounceTimerFlag;

extern sint16 volatile  gs16_swDebounceTimerCntr;       // use signed

extern sint16 volatile  gs16_peTimerCntr;       // use signed

extern      m41t81s_rtc_union           u_rtc_m41t81s;
extern      m41t81s_rtc_union_ptr       pu_rtc_m41t81s;

extern      s_rtcAlarmStruct            s_rtc_alarm;                 // structure for rtc alarms
extern      ps_rtcAlarmStruct           ps_rtc_alarm;

#endif

