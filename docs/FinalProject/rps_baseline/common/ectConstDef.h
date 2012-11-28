#ifndef ECT_CONST_DEF_H
    #define ECT_CONST_DEF_H

//--------------------------------------------------------------------------------------------------
//((((((((((((((((((((((((((((((((((ectConstDef.h )))))))))))))))))))))))))))))))))))))))))))))))
//----------------------------------------------------------------------------------------------------
    #define TIMER_PRESCALE_16                       0x04    // divide by 16
    #define PRESCALE_CNT                            TIMER_PRESCALE_16
    #define TIMER_PRESCALE                          TIMER_PRESCALE_16    // select prescale

    #define PWMEN_pwm                               PWME            // pulse width Module Enable
    #define PWMPOL_pwm                              PWMPOL          // pulse width clock select and polarity
    #define PWMCLK_pwm                              PWMCLK          // Pulse Width Module Clk and Concatenate
    #define PWMPRCLK_pwm                            PWMPRCLK
    #define PWMCAE_pwm                              PWMCAE          // Pulse Width Module Center
    #define PWMCTL_pwm                              PWMCTL          // Pulse Width Module Register Control
    #define PWMDTY4_pwm                             PWMDTY4         // duty cycle channel 4
    #define PWMPER4_pwm                             PWMPER4         // period channel 4

//==================================================================================================================
    #define PWMCTL_pwm                              PWMCTL          // Pulse Width Module Control
    #define PWMCAE_pwm                              PWMCAE          // Pulse Width Module Center
    #define PWMSCLB_pwm                             PWMSCLB         // pwm scale B register
    #define PWMPRCLK_pwm                            PWMPRCLK        // pwm scale B register


    #define modulus_CNTR_CNTRL_REG_MCCTL            MCCTL           // modulus down counter control register
    #define modulus_DOWN_CNT_CNTR_REG_MCCNT         MCCNT           // modulus down count counter register
    #define modulus_DOWN_CNT_CNTR_FLAG_REG_MCFLG    MCFLG           // modulus down count counter register
    #define modulus_CNTR_UNDERFLOW_INT_ENABLE_MCZI  MCZI            // modulus down counter interrupt enable
    #define modulus_MODE_ENABLE_MODMC               MODMC           // enables modulus counter
    #define modulus_READ_DOWN_CNTR_LOAD_RDMCL       RDMCL           // enables read of modulus down counter
    #define modulus_DOWN_CNTR_ENABLE_MCEN           MCEN            // enables modulus down counter
    #define modulus_DOWN_CNTR_INT_FLAG_MCZF         MCZF            // flag set when count reachs 0



//---------------------- Modulus Down-Counter Control Register ---(MCCTL)-----
    #define MCZI                            bit(7)                      // Modulus Counter Underflow interupt enable
    #define MODMC                           bit(6)                      // Modulus Mode Enable
    #define RDMCL                           bit(5)                      // Read Modulus Down Count Load
    #define ICLAT                           bit(4)                      // Input Capture Force Latch Action
    #define FLMC                            bit(3)                      // Force Load Register-->Modulus Counter Count Reg
    #define MCEN                            bit(2)                      // MCEN-Modulus Down-Counter Enable
    #define MCPR1                           bit(1)                      // MCPR1-Modulus Counter Prescale 1
    #define MCPR0                           bit(0)                      // MCPR0-Modulus Counter Prescale 0
//---------------------- Modulus Down Counter Flag Register ------(MCFLG)-----
    #define MCZF                            bit(7)                      // Modulus Counter Underflow Interrupt Flag

#define MODULUS_PRESCALE                                0x02  // no divide
#define MODULUS_PRESCALE_MASK                           0xFC  
#define MODULUS_DOWN_CNT                                0x3E8 // -->1ms interrupt


#endif
