/*============================================================================================================
                                $RCSfile: SwitchHandlerFcns.c,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.4 $
       $Date: 2007/06/20 03:11:52 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/SwitchHandlerFcns.c,v $

=============================================================================================================*/
#include "TypeDef.h"
#include "RPS_BldOptions.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#ifndef NAV_SW_POLLED
    #include "kbiPort.h"
#endif
#ifdef NAV_SW_INTERRUPT
    #include "swPort.h"
#endif

#include "lcdConstDef.h"

#include "SwitchStructDef.h"
#include "ExtSwitchDeclar.h"

#include "ExtGlobalVarDeclar.h"

#include "swDebounceTimer.h"

#include "FcnDef.h"

void    u8Fn_swDeriveFlags                  (uint8              sw_value_read);

#define DEBOUNCE_TIME               ge_SW_DEBOUNCE_200_MS

#ifdef NAV_SW_POLLED
/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( vFn_swPoll ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

vFn_swPoll
    Function:   vFn_swPoll
    
    Input:      pointer to struct for switch or switches to read and check on
                ge_WAIT, ge_READ_ONCE
    
    Return:     void
    
    Notes:      wait until not active
                read port looking for switch press
                checks switch against switch passed
                if equal; set debounce timer and exit
                if not equal set debouce timer and restart loop
--------------------------------------------------------------------------------------------------*/
void vFn_swPoll (uint8     *pu8_switch_value,
                 uint16     u16_sw_poll_mode)
{
uint8   static sw_value_read;
uint16  u16_switch_match_flag;

    u16_switch_match_flag = ge_FAIL;                    // init start requirements

    if(u16_sw_poll_mode == ge_WAIT_ON_SW){
        do{
            sw_value_read = ~(swPort_GetVal());         // read port, complement all the port pins

            u8Fn_swDeriveFlags(sw_value_read);

            if(*pu8_switch_value == ge_SW_FLAG_SET){    // must match with passed flag or fail
                u16_switch_match_flag = ge_SUCCESS;
            }else {
            if(*pu8_switch_value != ge_SW_FLAG_SET){
                u16_switch_match_flag = ge_FAIL;
            }}
        }while(u16_switch_match_flag != ge_SUCCESS);    // wait until correct switch is read
    }else {
    if(u16_sw_poll_mode == ge_READ_ONCE){
        sw_value_read = ~(swPort_GetVal());             // read port, complement all the port pins
        u8Fn_swDeriveFlags(sw_value_read);              // only read once
    }}

    vFn_swSetDebounceTimer(DEBOUNCE_TIME);              // wait for settle
    while(gu8_swDebounceTimerFlag  == ge_SW_DEBOUNCE_ACTIVE){}; // wait if swDebounce active

    return;
}

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( u8Fn_swDeriveFlags ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

u8Fn_swDeriveFlags
    Function:   u8Fn_swDeriveFlags
    
    Input:      switch detected
    
    Return:     void
    
    Notes:      sets the struct entries based on value passed
--------------------------------------------------------------------------------------------------*/
void u8Fn_swDeriveFlags (uint8 sw_value_read)
{
uint8 static invalid_interrupt_cntr;

    swResetAllFlags();

    switch(sw_value_read){

        case    ge_SWLFT:
            if( swStatusFlagsPtr->Lft.Enable == ge_SW_INT_ENABLE ){             // now check if interrupt to be acknowledged

               swStatusFlagsPtr->Lft.Status                           = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->LftOrRt.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection;       // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_LFT;                             // save new/current direction
            }
        break;

        case    ge_SWRT:
            if(swStatusFlagsPtr->Rt.Enable == ge_SW_INT_ENABLE){             // now check if interrupt to be acknowledged

               swStatusFlagsPtr->Rt.Status                            = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->LftOrRt.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection;       // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_RT;                              // save new/current direction
            }
        break;

        case    ge_SWUP:
            if(swStatusFlagsPtr->Up.Enable == ge_SW_INT_ENABLE){             // now check if interrupt to be acknowledged

               swStatusFlagsPtr->Up.Status                            = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->UpOrDwn.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection; // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_UP;          // save new/current direction
            }
        break;

        case    ge_SWDWN:
            if(swStatusFlagsPtr->Dwn.Enable == ge_SW_INT_ENABLE){            // now check if interrupt to be acknowledged

               swStatusFlagsPtr->Dwn.Status                           = ge_SW_FLAG_SET; // indicate switch is pushed
               swStatusFlagsPtr->UpOrDwn.Status                       = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET; // indicate either switch was pushed
               swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status              = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;

               swStatusFlagsPtr->LastDirection = swStatusFlagsPtr->CurrDirection; // save last direction
               swStatusFlagsPtr->CurrDirection = ge_SW_DOWN;        // save new/current direction
            }
        break;

        case    ge_SWSEL:
            if(swStatusFlagsPtr->Sel.Enable == ge_SW_INT_ENABLE){            // now check if interrupt to be acknowledged

               swStatusFlagsPtr->Sel.Status                           = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrSel.Status                  = ge_SW_FLAG_SET;
               swStatusFlagsPtr->UpOrDwnOrSel.Status                  = ge_SW_FLAG_SET;
               swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status         = ge_SW_FLAG_SET;
            }
        break;

        default:
                invalid_interrupt_cntr++;               // trap, shouldn't happen
        break;
    }

    return;
}
#endif

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( vFn_swSetDebounceTimer ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

vFn_swSetDebounceTimer
    Function:   vFn_swSetDebounceTimer
    
    Input:      uint16 ge_timeout_in_ms, this is enumerated count of timer intervals to wait before setting flag
    
    Return:     termination status
    
    Notes:      Caller uses enum count of increments to set global variable used by events bean
                Inits flag, cntr and enables bean and events
                Caller waits on gu8_swDebounceTimerFlag to set False to continue
--------------------------------------------------------------------------------------------------*/
void vFn_swSetDebounceTimer (uint16 ge_timeout_in_ms)
{
    gs16_swDebounceTimerCntr = ge_timeout_in_ms;
    gu8_swDebounceTimerFlag = ge_SW_DEBOUNCE_ACTIVE;
    swDebounceTimer_Enable();
    swDebounceTimer_EnableEvent();

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( swInterruptEnable )))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Enable software flags for switches in interrupt handler 
    
    Input:          None

    Return:         None
    
    Notes:          
----------------------------------------------------------------------------------------------------*/
void swInterruptEnable (uint8 lft,
                        uint8 rt,
                        uint8 up,
                        uint8 dwn,
                        uint8 sel)
{
    if(lft == ge_SW_LFT){
        swStatusFlagsPtr->Lft.Enable = ge_SW_INT_ENABLE;   // enable the switch, doesn't matter if already enabled
    }

    if(rt == ge_SW_RT){
        swStatusFlagsPtr->Rt.Enable = ge_SW_INT_ENABLE;    // enable the switch, doesn't matter if already enabled
    }

    if(up == ge_SW_UP){
        swStatusFlagsPtr->Up.Enable = ge_SW_INT_ENABLE;    // enable the switch, doesn't matter if already enabled
    }
    
    if(dwn == ge_SW_DOWN){
        swStatusFlagsPtr->Dwn.Enable = ge_SW_INT_ENABLE;   // enable the switch, doesn't matter if already enabled
    }

    if(sel == ge_SW_SEL){
        swStatusFlagsPtr->Sel.Enable = ge_SW_INT_ENABLE;   // enable the switch, doesn't matter if already enabled
    }

    return;                                             //end-fcn_swInterruptEnable
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( swInterruptDisable )))))))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Disable software flags for switches in interrupt handler 
    
    Input:          None

    Return:         None
    
    Notes:          
----------------------------------------------------------------------------------------------------*/
void swInterruptDisable (uint8 lft,
                         uint8 rt,
                         uint8 up,
                         uint8 dwn,
                         uint8 sel)
{
    if(lft == ge_SW_LFT){
        swStatusFlagsPtr->Lft.Enable = ge_SW_INT_DISABLE;   // enable the switch, doesn't matter if already enabled
    }
    
    if(rt == ge_SW_RT){
        swStatusFlagsPtr->Rt.Enable = ge_SW_INT_DISABLE;    // enable the switch, doesn't matter if already enabled
    }
    
    if(up == ge_SW_UP){
        swStatusFlagsPtr->Up.Enable = ge_SW_INT_DISABLE;    // enable the switch, doesn't matter if already enabled
    }
    
    if(dwn == ge_SW_DOWN){
        swStatusFlagsPtr->Dwn.Enable = ge_SW_INT_DISABLE;   // enable the switch, doesn't matter if already enabled
    }
    
    if(sel == ge_SW_SEL){
        swStatusFlagsPtr->Sel.Enable = ge_SW_INT_DISABLE;   // enable the switch, doesn't matter if already enabled
    }

    return;                                             //end-fcn_swInterruptDisable
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( swResetFlags )))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Reset all flags 
    
    Input:          none 

    Return:         None
    
    Notes:          Reset all flags including and the combination flags
----------------------------------------------------------------------------------------------------*/
void swResetAllFlags (void)
{
    swResetFlags
       (ge_SW_LFT,
        ge_SW_RT,
        ge_SW_UP,
        ge_SW_DOWN,
        ge_SW_SEL);                                 // use switch to exit, therefore init flags

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( swResetFlags )))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Reset Reset flags 
    
    Input:          uint16 lft, uint16 rt, uint16 up, uint16 dwn, uint16 sel, 

    Return:         None
    
    Notes:          Reset all flags including and the combination flags
----------------------------------------------------------------------------------------------------*/
void swResetFlags (uint8 lft,
                   uint8 rt,
                   uint8 up,
                   uint8 dwn,
                   uint8 sel)
{
    if(lft == ge_SW_LFT)
    {
       swStatusFlagsPtr->Lft.Status                   = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRt.Status               = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrSel.Status          = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status      = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status = ge_SW_FLAG_RESET;
    }
    
    if(rt == ge_SW_RT)
    {
       swStatusFlagsPtr->Rt.Status                    = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRt.Status               = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrSel.Status          = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status      = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status = ge_SW_FLAG_RESET;
    }
    
    if(up == ge_SW_UP)
    {
       swStatusFlagsPtr->Up.Status                     = ge_SW_FLAG_RESET;  // init flags that could be used
       swStatusFlagsPtr->UpOrDwn.Status                = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->UpOrDwnOrSel.Status           = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status       = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status  = ge_SW_FLAG_RESET;
    }
    
    if(dwn == ge_SW_DOWN)
    {
       swStatusFlagsPtr->Dwn.Status                    = ge_SW_FLAG_RESET;  //    do loops could fail if not reset
       swStatusFlagsPtr->UpOrDwn.Status                = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->UpOrDwnOrSel.Status           = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwn.Status       = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status  = ge_SW_FLAG_RESET;
    }
    
    if(sel == ge_SW_SEL)
    {
       swStatusFlagsPtr->Sel.Status                    = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->UpOrDwnOrSel.Status           = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrSel.Status           = ge_SW_FLAG_RESET;
       swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status  = ge_SW_FLAG_RESET;
    }

    return;
}                                                       //end-fcn_swResetFlags

/*--------------------------------------------------------------------------------------------------    
(((((((((((((((((((((((((((((((((( WaitOnAnySwitch ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Waits for the any flag to be set    

    Input:          None

    Return:         None

    Notes:          swStatusFlagsPtr->LftOrRtOrSel.Status is set in the interrupt routine
--------------------------------------------------------------------------------------------------*/
void WaitOnAnySwitch (void)
{
    swResetFlags
       (ge_SW_LFT,
        ge_SW_RT,
        ge_SW_UP,
        ge_SW_DOWN,
        ge_SW_SEL);                                      // use switch to exit, therefore init flags

    swInterruptEnable                                   // enable 5 basic sw's needed for menu fcns
        (ge_SW_LFT,    
         ge_SW_RT,     
         ge_SW_UP,     
         ge_SW_DOWN,    
         ge_SW_SEL);                                    // enable 5 basic sw's needed for menu fcns

    #ifdef NAV_SW_POLLED
        vFn_swPoll
            ( &(swStatusFlags.LftOrRtOrUpOrDwnOrSel.Status),
             ge_WAIT_ON_SW );                           // wait on one of 5 sw's
    #endif
    while (swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status != ge_SW_FLAG_SET){}; // this is a wait on the last trans before while loop exited

/*    swInterruptDisable
        (ge_SW_LFT,    
         ge_SW_RT,     
         ge_SW_UP,     
         ge_SW_DOWN,    
         ge_SW_SEL);                                    // switchs implicitly enabled, explicitly disable
*/
    return;
}                                                       // end-fcn

/*--------------------------------------------------------------------------------------------------    
(((((((((((((((((((((((((((((((((( vFn_swResetAndEnable ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Waits for the any flag to be set    

    Input:          None

    Return:         None

    Notes:          clears all flags and allows any switch to be recognized
--------------------------------------------------------------------------------------------------*/

void vFn_swResetAndEnable (void)
{

    swResetFlags
       (ge_SW_LFT,
        ge_SW_RT,
        ge_SW_UP,
        ge_SW_DOWN,
        ge_SW_SEL);                                      // use switch to exit, therefore init flags

    swInterruptEnable
      (ge_SW_LFT,                                        // enable 5 basic sw's needed for menu fcns   
       ge_SW_RT,
       ge_SW_UP,
       ge_SW_DOWN,
       ge_SW_SEL);

    return;
}


