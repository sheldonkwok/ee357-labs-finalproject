/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : RxTx_spi.c
**     Project   : flashLib
**     Processor : MCF51CN128CLK
**     Component : SynchroMaster
**     Version   : Component 02.335, Driver 01.29, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 3:47 PM
**     Abstract  :
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings  :
**         Synchro type                : MASTER
**
**         Serial channel              : SPI2
**
**         Protocol
**             Init baud rate          : 2_097MHz
**             Clock edge              : falling
**             Width                   : 8 bits (always)
**             Empty character         : 0
**             Empty char. on input    : RECEIVED
**
**         Registers
**             Input buffer            : SPI2D     [0xFFFF81E5]
**             Output buffer           : SPI2D     [0xFFFF81E5]
**             Control register        : SPI2C1    [0xFFFF81E0]
**             Mode register           : SPI2C2    [0xFFFF81E1]
**             Baud setting reg.       : SPI2BR    [0xFFFF81E2]
**
**             Priority                : 520
**
**             Priority                : 520
**
**         Used pins                   :
**         ----------------------------------------------------------
**              Function    | On package |    Name
**         ----------------------------------------------------------
**               Input      |     67     |  PTE0_KBI2P0_MISO2_ADP2
**               Output     |     68     |  PTE1_KBI2P1_MOSI2_ADP1
**               Clock      |     66     |  PTD7_RGPIO7_SPSCK2_ADP3
**         ----------------------------------------------------------
**
**     Contents  :
**         Enable                - byte RxTx_spi_Enable(void);
**         Disable               - byte RxTx_spi_Disable(void);
**         EnableEvent           - byte RxTx_spi_EnableEvent(void);
**         DisableEvent          - byte RxTx_spi_DisableEvent(void);
**         RecvChar              - byte RxTx_spi_RecvChar(RxTx_spi_TComData *Chr);
**         SendChar              - byte RxTx_spi_SendChar(RxTx_spi_TComData Chr);
**         SetShiftClockPolarity - byte RxTx_spi_SetShiftClockPolarity(byte Edge);
**         SetIdleClockPolarity  - byte RxTx_spi_SetIdleClockPolarity(byte Level);
**         GetError              - byte RxTx_spi_GetError(RxTx_spi_TError *Err);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE RxTx_spi. */

#include "RxTx_spi.h"
#include "TimerDelay.h"
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
#include "sndPWM.h"
#include "flashCS.h"
#include "swDebounceTimer.h"
#include "audEnable.h"
#include "i2c.h"
#include "peTimer.h"
#include "swPort.h"
#include "rtcINT.h"
#include "Events.h"


/* Internal method prototypes */
static void HWEnDi(void);

#define OVERRUN_ERR      0x01          /* Overrun error flag bit   */
#define CHAR_IN_RX       0x08          /* Char is in RX buffer     */
#define FULL_TX          0x10          /* Full transmit buffer     */
#define RUNINT_FROM_TX   0x20          /* Interrupt is in progress */
#define FULL_RX          0x40          /* Full receive buffer      */

static bool EnUser;                    /* Enable/Disable SPI */
volatile bool RxTx_spi_EnEvent;        /* Enable/Disable events */
static byte SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Unused */
                                       /*       2 - Unused */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Unused */
static byte ErrFlag;                   /* Error flags mirror of SerFlag */
static RxTx_spi_TComData BufferRead;   /* Input char SPI commmunication */
static RxTx_spi_TComData BufferWrite;  /* Output char SPI commmunication */

/*
** ===================================================================
**     Method      :  HWEnDi (component SynchroMaster)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  if (EnUser) {                        /* Enable device? */
    SPI2C1_SPE = 1;                    /* Enable device */
    SPI2C1_SPIE = 1;                   /* Enable interrupts */
    if (SerFlag & FULL_TX) {           /* Is any char in transmit buffer? */
      (void)SPI2S;                     /* Read the status register */
      SPI2D = BufferWrite;             /* Store char to transmitter register */
    }
  }
  else {
    SPI2C1_SPE = 0;                    /* Disable device */
    SPI2C1_SPIE = 0;                   /* Disable interrupts */
  }
}

/*
** ===================================================================
**     Method      :  RxTx_spi_Enable (component SynchroMaster)
**
**     Description :
**         Enable the component - it starts send and receive functions.
**         Events may be generated ("DisableEvent"/"EnableEvent"). This
**         method cannot be disabled if the Fault mode is enabled. It's
**         intended for re-enabling the module if fault occurred. For
**         more information please see the <"Fault mode" >property .
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte RxTx_spi_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    HWEnDi();                          /* Enable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  RxTx_spi_Disable (component SynchroMaster)
**
**     Description :
**         Disable the component - it stops the send and receive
**         functions. No events will be generated. Note: When this
**         method is called while a transmission is in progress, the
**         data being transmitted/received may be lost.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte RxTx_spi_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    HWEnDi();                          /* Disable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  RxTx_spi_EnableEvent (component SynchroMaster)
**
**     Description :
**         Enable the events. This method is available only if the
**         "Interrupt service/event" property is enabled and at least
**         the one of events is set to generate code.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte RxTx_spi_EnableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  RxTx_spi_DisableEvent (component SynchroMaster)
**
**     Description :
**         Disable the events. This method is available only if the
**         "Interrupt service/event" property is enabled and at least
**         the one of events is set to generate code.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte RxTx_spi_DisableEvent(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  RxTx_spi_RecvChar (component SynchroMaster)
**
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). 
**         For information about SW overrun behavior please see
**         <General info page>.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - A pointer to the received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK - The valid data is received.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode.
**                           ERR_RXEMPTY - No data in receiver.
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last char or block received. In
**                           polling mode, this error code is returned
**                           only when the hardware supports detection
**                           of the overrun error. The valid data is
**                           also received.
**                           ERR_FAULT - Fault error was detected from
**                           the last char or block received. In the
**                           polling mode the ERR_FAULT is return until
**                           the user clear the fault flag bit, but in
**                           the interrupt mode ERR_FAULT is returned
**                           only once after the fault error occured.
**                           This error is supported only on the CPUs
**                           supports the faul mode function - where
**                           <Fault mode> property is available.
** ===================================================================
*/
byte RxTx_spi_RecvChar(RxTx_spi_TComData *Chr)
{
  byte FlagTmp;

  if (!(SerFlag & CHAR_IN_RX)) {       /* Is any char in RX buffer? */
    return ERR_RXEMPTY;                /* If no then error */
  }
  EnterCritical();                     /* Save the PS register */
  *Chr = BufferRead;                   /* Read the char */
  FlagTmp = SerFlag;                   /* Safe the flags */
  SerFlag &= ~(OVERRUN_ERR | CHAR_IN_RX | FULL_RX); /* Clear flag "char in RX buffer" */
  ExitCritical();                      /* Restore the PS register */
  if ((FlagTmp & OVERRUN_ERR) != 0) {  /* Has the overrun occured? */
    return ERR_OVERRUN;                /* If yes then return error */
  } else {
    return ERR_OK;
  }
}

/*
** ===================================================================
**     Method      :  RxTx_spi_SendChar (component SynchroMaster)
**
**     Description :
**         Sends one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte RxTx_spi_SendChar(RxTx_spi_TComData Chr)
{
  if (SerFlag & FULL_TX) {             /* Is any char in the TX buffer? */
    return ERR_TXFULL;                 /* If yes then error */
  }
  EnterCritical();                     /* Save the PS register */
  BufferWrite = Chr;                   /* Store char to the temporary variable */
  if(EnUser) {                         /* Is the device enabled by user? */
    (void)SPI2S;                       /* Read the status register */
    SPI2D = Chr;                       /* Store char to transmitter register */
  }
  SerFlag |= FULL_TX;                  /* Set the flag "full TX buffer" */
  ExitCritical();                      /* Restore the PS register */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  RxTx_spi_SetShiftClockPolarity (component SynchroMaster)
**
**     Description :
**         Sets the shift clock polarity at runtime. Output data will
**         be shifted on the selected edge polarity. The method will
**         disable communication (if enabled), change the shift clock
**         polarity end re-enable the communication (if it was enabled
**         before).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Edge            - Edge polarity.
**                           0-falling edge
**                           1-rising edge
**     Returns     :
**         ---             - Error code, possible codes: ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte RxTx_spi_SetShiftClockPolarity(byte Edge)
{
  bool EnUserTemp;

  if(Edge > 1) {
    return ERR_RANGE;
  }
  EnUserTemp = EnUser;                 /* Safe EnUser flag */
  if(EnUser) {                         /* Is device enabled? */
    EnUser = FALSE;
    HWEnDi();                          /* Disable device */
  }
  SPI2C1_CPHA = SPI2C1_CPOL ^ Edge;    /* Set phase bit */
  if(EnUserTemp) {                     /* Was device enabled? */
    EnUser=TRUE;
    HWEnDi();                          /* Enable device */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  RxTx_spi_SetIdleClockPolarity (component SynchroMaster)
**
**     Description :
**         Sets the idle clock polarity at runtime. If the
**         communication does not run, the clock signal will have
**         required level. The method will disable communication (if
**         enabled), change the idle clock polarity end re-enable the
**         communication (if it was enabled before).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Level           - Idle clock polarity:
**                           0-low
**                           1-high
**     Returns     :
**         ---             - Error code, possible codes: ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte RxTx_spi_SetIdleClockPolarity(byte Level)
{
  bool EnUserTemp;

  if(Level > 1) {
    return ERR_RANGE;
  }
  if(SPI2C1_CPOL != Level) {
    PTDD_PTDD7 = Level;                /* Define level on clk pin when device is disabled */
    EnUserTemp = EnUser;               /* Safe EnUser flag */
    if(EnUser) {                       /* Is device enabled? */
      EnUser = FALSE;
      HWEnDi();                        /* Disable device */
    }
    SPI2C1 ^= 0x0C;                    /* Invert polarity and phase bit */
    if(EnUserTemp) {                   /* Was device enabled? */
      EnUser=TRUE;
      HWEnDi();                        /* Enable device */
    }
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  RxTx_spi_GetError (component SynchroMaster)
**
**     Description :
**         Returns a set of errors on the channel (errors that cannot
**         be returned in given methods). The component accumulates
**         errors in a set; after calling [GetError] this set is
**         returned and cleared. This method is available only if the
**         "Interrupt service/event" property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Err             - A pointer to the returned set of errors
**     Returns     :
**         ---             - Error code (if GetError did not succeed),
**                           possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte RxTx_spi_GetError(RxTx_spi_TError *Err)
{
  EnterCritical();                     /* Save the PS register */
  Err->err = 0;
  Err->errName.OverRun = ((ErrFlag & OVERRUN_ERR) != 0); /* Overrun error */
  ErrFlag = 0x00;                      /* Reset error flags */
  ExitCritical();                      /* Restore the PS register */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  RxTx_spi_Interrupt (component SynchroMaster)
**
**     Description :
**         The method services the error interrupt of the selected 
**         peripheral(s) and eventually invokes the component's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_ERROR       0x01
#define ON_FULL_RX     0x02
#define ON_RX_CHAR     0x04
#define ON_FREE_TX     0x08
#define ON_TX_CHAR     0x10
#define ON_RX_CHAR_EXT 0x20
ISR(RxTx_spi_Interrupt)
{
  RxTx_spi_TComData Data = 0;          /* Temporary variable for data */
  byte Status;                         /* Temporary variable for flags */

  Status = SPI2S;                      /* Read the device error register */
  Data = SPI2D;                        /* Read data from receiver */
  if (SerFlag & CHAR_IN_RX) {          /* Is the overrun error flag set? */
    SerFlag |= OVERRUN_ERR;            /* If yes then set the Overrun error flag */
  }
  SerFlag |= CHAR_IN_RX;               /* Set flag "char in RX buffer" */
  BufferRead = Data;                   /* Read data from receiver */
  ErrFlag |= SerFlag;                  /* Update error flag mirror */
  SerFlag &= ~FULL_TX;                 /* Reset flag "full TX buffer" */

  if (RxTx_spi_EnEvent) {              /* Are the events enabled? */
    RxTx_spi_OnTxChar();               /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  RxTx_spi_Init (component SynchroMaster)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void RxTx_spi_Init(void)
{
  SerFlag = 0;                         /* Reset all flags */
  ErrFlag = 0;                         /* Reset all flags in mirror */
  RxTx_spi_EnEvent = TRUE;             /* Enable events */
  EnUser = TRUE;                       /* Enable device */
  (void)SPI2S;                         /* Read the status register */
  (void)SPI2D;                         /* Read the device register */
  /* SPI2BR: ??=0,SPPR2=0,SPPR1=1,SPPR0=0,??=0,SPR2=0,SPR1=0,SPR0=1 */
  setReg8(SPI2BR, 0x21);               /* Set the baud rate register */ 
  /* SPI2C2: ??=0,??=0,??=0,MODFEN=0,BIDIROE=0,??=0,SPISWAI=0,SPC0=0 */
  setReg8(SPI2C2, 0x00);               /* Configure the SPI port - control register 2 */ 
  /* SPI2C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=1,CPHA=1,SSOE=0,LSBFE=0 */
  setReg8(SPI2C1, 0x1C);               /* Configure the SPI port - control register 1 */ 
  HWEnDi();                            /* Enable/disable device according to the status flags */
}


/* END RxTx_spi. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
