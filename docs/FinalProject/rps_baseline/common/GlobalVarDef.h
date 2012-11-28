#ifndef DEF_GLOBALVAR_H
    #define DEF_GLOBALVAR_H

/*============================================================================================================
                                $RCSfile: GlobalVarDef.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.16 $
       $Date: 2007/06/08 21:54:59 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/GlobalVarDef.h,v $

=============================================================================================================*/
uint8 volatile          gu8SPI1Status;                                             /** SPI Driver Status */

uint8                   OutPutMode = 2,
                        lcdCharAddr,
                        lcdLineAddr,
                        lcdCurrLine,
                        lcdScrollCntrl,

                        gu8_ChannelSide,
                        gu8_peTimerFlag,
                        gu8_rtcFlags,
                        u8_rtcAlarmIntFlag;


uint8                   FontSizeCurr = FONT_NINE_DOT;                       //rwl put in RPS_BldOptions

uint8 volatile          MenuFlag;

sint16 volatile         gs16_swDebounceTimerCntr;       // loop cntr used in Events.c, use signed
uint8 volatile          gu8_swDebounceTimerFlag;        // updated in Events.c

uint8 volatile          RxTx_spi_Flag;
uint8 volatile          TimerDelayFlag;

sint16 volatile         gs16_peTimerCntr;       // use signed

#endif                      //end-def_DEF_GLOBALVAR_H
