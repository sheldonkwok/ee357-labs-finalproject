/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : i2c.c
**     Project   : sndLib
**     Processor : MCF51CN128CLK
**     Component : InternalI2C
**     Version   : Component 01.270, Driver 01.26, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:09 PM
**     Abstract  :
**          This component encapsulates the internal I2C communication 
**          interface. The implementation of the interface is based 
**          on the Philips I2C-bus specification version 2.0. 
**          Interface features:
**          MASTER mode
**            - Multi master communication
**            - The combined format of communication possible 
**              (see "Automatic stop condition" property)
**            - 7-bit slave addressing (10-bit addressing can be made as well)
**            - Acknowledge polling provided
**            - No wait state initiated when a slave device holds the SCL line low
**            - Holding of the SCL line low by slave device recognized as 'not available bus'
**            - Invalid start/stop condition detection provided
**          SLAVE mode
**            - 7-bit slave addressing
**            - General call address detection provided
**     Settings  :
**         Serial channel              : IIC2
**
**         Protocol
**             Mode                    : MASTER
**             Auto stop condition     : yes
**             SCL frequency           : 6000000 Hz
**
**         Initialization
**
**             Target slave address    : 8
**             Component init          : Enabled
**             Events                  : Enabled
**
**         Registers
**             Input buffer            : IIC2D     [0xFFFF8224]
**             Output buffer           : IIC2D     [0xFFFF8224]
**             Control register        : IIC2C1    [0xFFFF8222]
**             Status register         : IIC2S     [0xFFFF8223]
**             Baud setting reg.       : IIC2F     [0xFFFF8221]
**             Address register        : IIC2A1    [0xFFFF8220]
**             Glitch filter register  : IIC2FLT   [0xFFFF822A]
**
**         Interrupt
**             Vector name             : Viic2
**             Priority                : 135
**
**         Used pins                   :
**       ----------------------------------------------------------
**            Function    | On package |    Name
**       ----------------------------------------------------------
**              SDA       |     48     |  PTC7_SDA2_SPSCK1_ADP8
**              SCL       |     47     |  PTC6_SCL2_MISO1_ADP9
**       ----------------------------------------------------------
**     Contents  :
**         Enable          - byte i2c_Enable(void);
**         Disable         - byte i2c_Disable(void);
**         EnableEvent     - byte i2c_EnableEvent(void);
**         DisableEvent    - byte i2c_DisableEvent(void);
**         SendChar        - byte i2c_SendChar(byte Chr);
**         RecvChar        - byte i2c_RecvChar(byte *Chr);
**         SendBlock       - byte i2c_SendBlock(void* Ptr, word Siz, word *Snt);
**         RecvBlock       - byte i2c_RecvBlock(void* Ptr, word Siz, word *Rcv);
**         GetCharsInTxBuf - word i2c_GetCharsInTxBuf(void);
**         GetCharsInRxBuf - word i2c_GetCharsInRxBuf(void);
**         SelectSlave     - byte i2c_SelectSlave(byte Slv);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE i2c. */


#include "Events.h"
#include "i2c.h"

/*SerFlag bits*/
#define OVERRUN_ERR      0x01          /* Overrun error flag bit   */
#define WAIT_RX_CHAR     0x02          /* Wait for received char. flag bit (Master)  */
#define CHAR_IN_TX       0x04          /* Char is in TX buffer (Master)    */
#define CHAR_IN_RX       0x08          /* Char is in RX buffer     */
#define FULL_TX          0x10          /* Full transmit buffer     */
#define IN_PROGRES       0x20          /* Communication is in progress (Master) */
#define FULL_RX          0x40          /* Full receive buffer      */
#define MSxSL            0x80          /* Master x Slave flag bit  */

static bool EnUser;                    /* Enable/Disable device */
volatile bool i2c_EnEvent;             /* Enable/Disable events */
static byte i2c_SlaveAddr;             /* Variable for Slave address */
static word InpLenM;                   /* Length of input bufer's content */
static byte *InpPtrM;                  /* Pointer to input buffer for Master mode */
static word OutLenM;                   /* Length of output bufer's content */
static byte * OutPtrM;                 /* Pointer to output buffer for Master mode */
volatile word i2c_SndRcvTemp;          /* Temporary variable for SendChar (RecvChar) when they call SendBlock (RecvBlock) */
static byte ChrTemp;                   /* Temporary variable for SendChar method */
volatile byte i2c_SerFlag;             /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Wait for received char. flag bit (Master) */
                                       /*       2 - Char is in TX buffer (Master) */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Master x Slave */
/*
** ===================================================================
**     Method      :  i2c_Interrupt (component InternalI2C)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define RXAK 0x01
#define SRW  0x04
#define IBAL 0x10
#define IAAS 0x40

#define ON_ARBIT_LOST 0x01
#define ON_FULL_RX    0x02
#define ON_RX_CHAR    0x04
#define ON_FREE_TX    0x08
#define ON_TX_CHAR    0x10
#define ON_OVERRUN    0x20
#define ON_TX_EMPTY   0x40

ISR(i2c_Interrupt)
{
  byte Status;                         /* Safe status register */

  Status = IIC2S;
  IIC2S_IICIF = 1;                     /* Clear interrupt flag and ARBL flag if set (by means of the read modify write effect) */
  if(IIC2C1_MST) {                     /* Is device in master mode? */
    if (i2c_EnEvent) {
      i2c_OnByteTransfer();            /* Invoke OnByteTransfer event */
    }
    if(IIC2C1_TX) {                    /* Is device in Tx mode? */
      if(Status & RXAK) {              /* NACK received? */
        IIC2C1_MST = 0;                /* Switch device to slave mode (stop signal sent) */
        IIC2C1_TX = 0;                 /* Switch to Rx mode */
        OutLenM = 0;                   /* No character for sending */
        InpLenM = 0;                   /* No character for reception */
        i2c_SerFlag &= ~(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES); /* No character for sending or reception*/
        if (i2c_EnEvent) {
          i2c_OnNACK();                /* Invoke OnNACK event */
        }
      }
      else {
        if(OutLenM) {                  /* Is any char. for transmitting? */
          OutLenM--;                   /* Decrease number of chars for the transmit */
          IIC2D = *(OutPtrM)++;        /* Send character */
        }
        else {
          if(InpLenM) {                /* Is any char. for reception? */
            if(InpLenM == 1) {         /* If only one char to receive */
              IIC2C1_TXAK = 1;         /* then transmit ACK disable */
            }
            else {
              IIC2C1_TXAK = 0;         /* else transmit ACK enable */
            }
            IIC2C1_TX = 0;             /* Switch to Rx mode */
            (void)IIC2D;               /* Dummy read character */
          }
          else {
            i2c_SerFlag &= ~IN_PROGRES; /* Clear flag "busy" */
            IIC2C1_MST = 0;            /* Switch device to slave mode (stop signal sent) */
            IIC2C1_TX = 0;             /* Switch to Rx mode */
            if (i2c_EnEvent) {
              i2c_OnTransmitData();    /* Invoke OnTransmitData event */
            }
          }
        }
      }
    }
    else {
      InpLenM--;                       /* Decrease number of chars for the receive */
      if(InpLenM) {                    /* Is any char. for reception? */
        if(InpLenM == 1) {
          IIC2C1_TXAK = 1;             /* Transmit ACK disable */
        }
      }
      else {
        IIC2C1_MST = 0;                /* If no, switch device to slave mode (stop signal sent) */
        IIC2C1_TXAK = 0;               /* Transmit ACK enable */
      }
      *(InpPtrM)++ = IIC2D;            /* Receive character */
      if(!InpLenM) {                   /* Is any char. for reception? */
        if (i2c_EnEvent) {
          i2c_OnReceiveData();         /* Invoke OnReceiveData event */
        }
      }
    }
  }
  else {
    if(Status & IBAL) {                /* Arbitration lost? */
      OutLenM = 0;                     /* No character for sending */
      InpLenM = 0;                     /* No character for reception */
      i2c_SerFlag &= ~(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES); /* No character for sending or reception*/
      IIC2C1_TX = 0;                   /* Switch to Rx mode */
      if (i2c_EnEvent) {
        i2c_OnArbitLost();             /* Invoke OnArbitLost event */
      }
    }
  }
}

/*
** ===================================================================
**     Method      :  i2c_Enable (component InternalI2C)
**
**     Description :
**         Enables I2C component. Events may be generated
**         ("DisableEvent"/"EnableEvent").
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte i2c_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    i2c_SerFlag &= ~0x6F;              /* Clear the status variable */
    IIC2C1_IICEN = 1;                  /* Enable device */
    /* IIC2C1: IICEN=1,IICIE=1,MST=0,TX=0,TXAK=0,RSTA=0,??=0,??=0 */
    IIC2C1 = 0xC0;                     /* Control register settings */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  i2c_Disable (component InternalI2C)
**
**     Description :
**         Disables I2C component. No events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte i2c_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    /* IIC2C1: IICEN=0,IICIE=0,MST=0,TX=0,TXAK=0,RSTA=0,??=0,??=0 */
    IIC2C1 = 0;                        /* Disable device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  i2c_EnableEvent (component InternalI2C)
**
**     Description :
**         Enables the events.
**         This method is enabled only if any event is generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte i2c_EnableEvent(void)

**  This method is implemented as a macro. See i2c.h file.  **
*/

/*
** ===================================================================
**     Method      :  i2c_DisableEvent (component InternalI2C)
**
**     Description :
**         Disables the events.
**         This method is enabled only if any event is generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte i2c_DisableEvent(void)

**  This method is implemented as a macro. See i2c.h file.  **
*/

/*
** ===================================================================
**     Method      :  i2c_SendChar (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 0 to the I2C bus and then writes one
**         character (byte) to the bus. The slave address must be
**         specified before, by the "SelectSlave" or "SelectSlave10"
**         method or in the component initialization section, "Target
**         slave address init" property. If interrupt service is
**         enabled and the method returns ERR_OK, it doesn't mean that
**         transmission was successful. The state of transmission is
**         obtainable from (OnTransmitData, OnError or OnArbitLost)
**         events. 
**         When working as a SLAVE, this method writes a character to
**         the internal output slave buffer and, after the master
**         starts the communication, to the I2C bus. If no character is
**         ready for a transmission (internal output slave buffer is
**         empty), the Empty character will be sent (see "Empty
**         character" property).
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only in
**                           master mode and when interrupt service is
**                           disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full (slave
**                           mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
** ===================================================================
*/
byte i2c_SendChar(byte Chr)
{
  if((IIC2S_BUSY)||(InpLenM)||(i2c_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  ChrTemp = Chr;                       /* Save character */
  return (i2c_SendBlock(&ChrTemp, (word)1, (word*)&i2c_SndRcvTemp)); /* Send character and return */
}

/*
** ===================================================================
**     Method      :  i2c_RecvChar (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 1 to the I2C bus, then reads one
**         character (byte) from the bus and then sends the stop
**         condition. The slave address must be specified before, by
**         the "SelectSlave" or "SelectSlave10" method or in component
**         initialization section, property "Target slave address init".
**         If interrupt service is enabled and the method returns
**         ERR_OK, it doesn't mean that transmission was finished
**         successfully. The state of transmission must be tested by
**         means of events (OnReceiveData, OnError or OnArbitLost). In
**         case of successful transmission, received data is ready
**         after OnReceiveData event is called. 
**         When working as a SLAVE, this method reads a character from
**         the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Received character.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge (only
**                           in master mode and when interrupt service
**                           is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - No data in receiver (slave
**                           mode only)
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last character or block received
**                           (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
**                           ERR_NOTAVAIL - Method is not available in
**                           current mode - see the comment in the
**                           generated code
** ===================================================================
*/
/*
byte i2c_RecvChar(byte *Chr)

**  This method is implemented as a macro. See i2c.h file.  **
*/

/*
** ===================================================================
**     Method      :  i2c_SendBlock (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 0 to the I2C bus and then writes the
**         block of characters to the bus. The slave address must be
**         specified before, by the "SelectSlave" or "SlaveSelect10"
**         method or in component initialization section, "Target slave
**         address init" property. If interrupt service is enabled and
**         the method returns ERR_OK, it doesn't mean that transmission
**         was successful. The state of transmission is detectable by
**         means of events (OnTransmitData, OnError or OnArbitLost).
**         Data to be send is not copied to an internal buffer and
**         remains in the original location. Therefore the content of
**         the buffer should not be changed until the transmission is
**         complete. Event OnTransmitData can be used to detect the end
**         of the transmission.
**         When working as a SLAVE, this method writes a block of
**         characters to the internal output slave buffer and then,
**         after the master starts the communication, to the I2C bus.
**         If no character is ready for a transmission (internal output
**         slave buffer is empty), the "Empty character" will be sent
**         (see "Empty character" property). In SLAVE mode the data are
**         copied to an internal buffer, if specified by "Output buffer
**         size" property.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send.
**         Siz             - Size of the block.
**       * Snt             - Amount of data sent (moved to a buffer).
**                           In master mode, if interrupt support is
**                           enabled, the parameter always returns the
**                           same value as the parameter 'Siz' of this
**                           method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by the acknowledge (only
**                           in master mode and when interrupt service
**                           is disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot transmit data
**                           ERR_TXFULL - Transmitter is full. Some data
**                           has not been sent. (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
** ===================================================================
*/
byte i2c_SendBlock(void * Ptr,word Siz,word *Snt)
{
  if (!Siz) {                          /* Test variable Size on zero */
    *Snt = 0;
    return ERR_OK;                     /* If zero then OK */
  }
  if(!EnUser) {                        /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if((IIC2S_BUSY)||(InpLenM)||(i2c_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  EnterCritical();                     /* Enter the critical section */
  i2c_SerFlag |= IN_PROGRES;           /* Set flag "busy" */
  OutLenM = Siz;                       /* Set lenght of data */
  OutPtrM = (byte *)Ptr;               /* Save pointer to data for transmitting */
  IIC2C1_TX = 1;                       /* Set TX mode */
  if(IIC2C1_MST) {                     /* Is device in master mode? */
    IIC2C1_RSTA = 1;                   /* If yes then repeat start cycle generated */
  }
  else {
    IIC2C1_MST = 1;                    /* If no then start signal generated */
  }
  IIC2D = i2c_SlaveAddr;               /* Send slave address */
  ExitCritical();                      /* Exit the critical section */
  *Snt = Siz;                          /* Dummy number of really sent chars */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  i2c_RecvBlock (component InternalI2C)
**
**     Description :
**         When working as a MASTER, this method writes one (7-bit
**         addressing) or two (10-bit addressing) slave address bytes
**         inclusive of R/W bit = 1 to the I2C bus, then reads the
**         block of characters from the bus and then sends the stop
**         condition. The slave address must be specified before, by
**         the "SelectSlave" or "SelectSlave10" method or in component
**         initialization section, "Target slave address init" property.
**         If interrupt service is enabled and the method returns
**         ERR_OK, it doesn't mean that transmission was finished
**         successfully. The state of transmission must be tested by
**         means of events (OnReceiveData, OnError or OnArbitLost). In
**         case of successful transmission, received data is ready
**         after OnReceiveData event is called. 
**         When working as a SLAVE, this method reads a block of
**         characters from the input slave buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - A pointer to the block space for received
**                           data.
**         Siz             - The size of the block.
**       * Rcv             - Amount of received data. In master mode,
**                           if interrupt support is enabled, the
**                           parameter always returns the same value as
**                           the parameter 'Siz' of this method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  Device is disabled
**                           ERR_BUSY - The slave device is busy, it
**                           does not respond by an acknowledge (only in
**                           master mode and when interrupt service is
**                           disabled)
**                           ERR_BUSOFF - Clock timeout elapsed or
**                           device cannot receive data
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data. Only
**                           available data (or no data) has been
**                           returned  (slave mode only).
**                           ERR_OVERRUN - Overrun error was detected
**                           from last character or block receiving
**                           (slave mode only)
**                           ERR_ARBITR - Arbitration lost (only when
**                           interrupt service is disabled and in master
**                           mode)
**                           ERR_NOTAVAIL - Method is not available in
**                           current mode - see the comment in the
**                           generated code.
** ===================================================================
*/
byte i2c_RecvBlock(void* Ptr,word Siz,word *Rcv)
{
  if (!Siz) {                          /* Test variable Size on zero */
    *Rcv = 0;
    return ERR_OK;                     /* If zero then OK */
  }
  if(!EnUser) {                        /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if((IIC2S_BUSY)||(InpLenM)||(i2c_SerFlag&(CHAR_IN_TX|WAIT_RX_CHAR|IN_PROGRES))) { /* Is the bus busy */
    return ERR_BUSOFF;                 /* If yes then error */
  }
  EnterCritical();                     /* Enter the critical section */
  InpLenM = Siz;                       /* Set lenght of data */
  InpPtrM = (byte *)Ptr;               /* Save pointer to data for reception */
  IIC2C1_TX = 1;                       /* Set TX mode */
  if(IIC2C1_MST) {                     /* Is device in master mode? */
    IIC2C1_RSTA = 1;                   /* If yes then repeat start cycle generated */
  }
  else {
    IIC2C1_MST = 1;                    /* If no then start signal generated */
  }
  IIC2D = (byte)(i2c_SlaveAddr+1);     /* Send slave address */
  ExitCritical();                      /* Exit the critical section */
  *Rcv = Siz;                          /* Dummy number of really received chars */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  i2c_GetCharsInTxBuf (component InternalI2C)
**
**     Description :
**         Returns number of characters in the output buffer. In SLAVE
**         mode returns the number of characters in the internal slave
**         output buffer. In MASTER mode returns number of characters
**         to be sent from the user buffer (passed by SendBlock method).
**         This method is not supported in polling mode.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the output buffer.
** ===================================================================
*/
word i2c_GetCharsInTxBuf(void)
{
  return(OutLenM);                     /* Return number of chars remaining in the Master Tx buffer */
}

/*
** ===================================================================
**     Method      :  i2c_GetCharsInRxBuf (component InternalI2C)
**
**     Description :
**         Returns number of characters in the input buffer. In SLAVE
**         mode returns the number of characters in the internal slave
**         input buffer. In MASTER mode returns number of characters to
**         be received into a user buffer (passed by RecvChar or
**         RecvBlock method).
**         This method is not supported in polling mode.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the input buffer.
** ===================================================================
*/
word i2c_GetCharsInRxBuf(void)
{
  return(InpLenM);                     /* Return number of chars remaining in the Master Tx buffer */
}

/*
** ===================================================================
**     Method      :  i2c_SelectSlave (component InternalI2C)
**
**     Description :
**         This method selects a new slave for communication by its
**         7-bit slave address value. Any send or receive method
**         directs to or from selected device, until a new slave device
**         is selected by this method. This method is not available for
**         the SLAVE mode.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Slv             - 7-bit slave address value.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_BUSY - The device is busy, wait until
**                           the current operation is finished.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED -  The device is disabled
** ===================================================================
*/
byte i2c_SelectSlave(byte Slv)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    return ERR_DISABLED;               /* If yes then error */
  }
  if (IIC2C1_MST == 1) {               /* Is the device in the active state? */
    return ERR_BUSY;                   /* If yes then error */
  }
  i2c_SlaveAddr = (byte)(Slv << 1);    /* Set slave address */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  i2c_Init (component InternalI2C)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void i2c_Init(void)
{
  i2c_SerFlag = 0x80;                  /* Reset all flags */
  i2c_EnEvent = TRUE;                  /* Enable events */
  EnUser = TRUE;                       /* Enable device */
  i2c_SlaveAddr = 0x10;                /* Set variable for slave address */
  InpLenM = 0;                         /* No data to be received */
  IIC2FLT = 0x00;
  /* IIC2F: MULT1=1,MULT0=0,ICR5=0,ICR4=0,ICR3=1,ICR2=0,ICR1=0,ICR0=1 */
  setReg8(IIC2F, 0x89);                 
  IIC2C1_IICEN = 1;                    /* Enable device */
  /* IIC2C1: IICEN=1,IICIE=1,MST=0,TX=0,TXAK=0,RSTA=0,??=0,??=0 */
  IIC2C1 = 0xC0;                       /* Control register settings */
}


/* END i2c. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
