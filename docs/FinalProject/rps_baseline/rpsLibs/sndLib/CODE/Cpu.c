/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Cpu.c
**     Project   : sndLib
**     Processor : MCF51CN128CLK
**     Component : MCF51CN128_80
**     Version   : Component 01.001, Driver 01.11, CPU db: 3.00.030
**     Datasheet : MCF51CN128RM Rev. 4 January 2009
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:09 PM
**     Abstract  :
**         This component "MCF51CN128_80" contains initialization of the
**         CPU and provides basic methods and events for CPU core
**         settings.
**     Settings  :
**
**     Contents  :
**         EnableInt  - void Cpu_EnableInt(void);
**         DisableInt - void Cpu_DisableInt(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE Cpu. */
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
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Events.h"
#include "Cpu.h"

/* Global variables */
volatile far word SR_reg;              /* Current CCR register */
volatile byte SR_lock;

/*Definition of global shadow variables*/
byte Shadow_PTE;


/*
** ===================================================================
**     Method      :  Cpu_Cpu_Interrupt (component MCF51CN128_80)
**
**     Description :
**         This ISR services an unused interrupt/exception vector.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(Cpu_Interrupt)
{
  /* This code can be changed using the CPU bean property "Build Options / Unhandled int code" */
  /* asm (HALT) */
}


/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MCF51CN128_80)
**
**     Description :
**         Disables maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_DisableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MCF51CN128_80)
**
**     Description :
**         Enables maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Cpu_EnableInt(void)

**      This method is implemented as macro in the header module. **
*/

/*
** ===================================================================
**     Method      :  __initialize_hardware (component MCF51CN128_80)
**
**     Description :
**         Configure the basic system functions (timing, etc.).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void __initialize_hardware(void)
{
  /* ### MCF51CN128_80 "Cpu" init code ... */
  /*  PE initialization code after reset */


  /* Common initialization of the write once registers */
  /* SOPT1: ??=0,SL=0,STOPE=0,WAITE=1,COPT=0,COPCLKS=0,COPW=0 */
  setReg8(SOPT1, 0x10);                 
  /* SPMSC1: LVDF=0,LVDACK=0,LVDIE=0,LVDRE=1,LVDSE=1,LVDE=1,??=0,BGBE=0 */
  setReg8(SPMSC1, 0x1C);                
  /* SPMSC2: LPR=0,LPRS=0,LPWUI=0,??=0,PPDF=0,PPDACK=0,PPDE=1,PPDC=0 */
  setReg8(SPMSC2, 0x02);                
  /* SPMSC3: LVDV=0,LVWV=0,LVWIE=0 */
  clrReg8Bits(SPMSC3, 0x38);            
  /* Initialization of CPU registers */
  asm {
    /* VBR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,ADDRESS=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    clr.l d0
    movec d0,VBR
    /* CPUCR: ARD=0,IRD=0,IAE=0,IME=0,BWD=0,??=0,FSD=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
    clr.l d0
    movec d0,CPUCR
  }
  /*  System clock initialization */
  if (*(unsigned char*)0x03FF != 0xFF) { /* Test if the device trim value is stored on the specified address */
    MCGTRM = *(unsigned char*)0x03FF;  /* Initialize MCGTRM register from a non volatile memory */
    MCGSC = *(unsigned char*)0x03FE;   /* Initialize MCGSC register from a non volatile memory */
  }
  /* MCGC2: BDIV=0,RANGE=0,HGO=0,LP=0,EREFS=0,ERCLKEN=0,EREFSTEN=0 */
  setReg8(MCGC2, 0x00);                /* Set MCGC2 register */ 
  /* MCGC1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
  setReg8(MCGC1, 0x06);                /* Set MCGC1 register */ 
  /* MCGC3: LOLIE=0,PLLS=0,CME=0,DIV32=0,VDIV=1 */
  setReg8(MCGC3, 0x01);                /* Set MCGC3 register */ 
  /* MCGC4: ??=0,??=0,DMX32=0,??=0,??=0,??=0,DRST_DRS=2 */
  setReg8(MCGC4, 0x02);                /* Set MCGC4 register */ 
  while(!MCGSC_LOCK) {                 /* Wait until FLL is locked */
  }
  
  /* Initialization of the ExternalBus module */
  /* CSMR0: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,V=0 */
  clrReg32Bits(CSMR0, 0xFEFFUL);        
  /* CSMR1: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,V=0 */
  clrReg32Bits(CSMR1, 0xFEFFUL);        
  /*** End of PE initialization code after reset ***/
}

/*
** ===================================================================
**     Method      :  PE_low_level_init (component MCF51CN128_80)
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
  /* SCGC1: MTIM1=1,TPM2=1,TPM1=1,ADC=1,IIC2=1,IIC1=1,SCI2=1,SCI1=1 */
  setReg8(SCGC1, 0xFF);                 
  /* SCGC2: SCI3=1,FTSR=1,IRQ=1,KBI2=1,KBI1=1,RTC=1,SPI2=1,SPI1=1 */
  setReg8(SCGC2, 0xFF);                 
  /* SCGC3: PTH=1,PTG=1,PTF=1,PTE=1,PTD=1,PTC=1,PTB=1,PTA=1 */
  setReg8(SCGC3, 0xFF);                 
  /* SCGC4: ??=1,??=1,??=1,MTIM2=1,MC=1,MB=1,FEC=1,PTJ=1 */
  setReg8(SCGC4, 0xFF);                 
  /* Common initialization of the CPU registers */
  /* PTEPF2: E3=0,E2=0,E1=2,E0=2 */
  setReg8(PTEPF2, 0x0A);                
  /* PTDPF1: D7=2,D6=0 */
  clrSetReg8Bits(PTDPF1, 0x70, 0x80);   
  /* PTED: PTED7=0,PTED4=0,PTED3=1 */
  clrSetReg8Bits(PTED, 0x90, 0x08);     
  /* PTEDD: PTEDD7=1,PTEDD4=1,PTEDD3=1,PTEDD2=0,PTEDD1=1,PTEDD0=0 */
  clrSetReg8Bits(PTEDD, 0x05, 0x9A);    
  /* PTDDD: PTDDD7=1 */
  setReg8Bits(PTDDD, 0x80);             
  /* PTDD: PTDD7=1 */
  setReg8Bits(PTDD, 0x80);              
  /* PTAD: PTAD6=0,PTAD5=0,PTAD4=1,PTAD3=0,PTAD2=1 */
  clrSetReg8Bits(PTAD, 0x68, 0x14);     
  /* PTAPE: PTAPE6=1,PTAPE5=1,PTAPE4=1,PTAPE3=1,PTAPE2=1 */
  setReg8Bits(PTAPE, 0x7C);             
  /* PTADD: PTADD6=1,PTADD5=1,PTADD4=1,PTADD3=1,PTADD2=1 */
  setReg8Bits(PTADD, 0x7C);             
  /* PTAPF2: A3=0,A2=0 */
  clrReg8Bits(PTAPF2, 0xF0);            
  /* PTJD: PTJD3=0,PTJD1=0 */
  clrReg8Bits(PTJD, 0x0A);              
  /* PTJPE: PTJPE3=1,PTJPE1=1 */
  setReg8Bits(PTJPE, 0x0A);             
  /* PTJDD: PTJDD3=1,PTJDD1=1 */
  setReg8Bits(PTJDD, 0x0A);             
  /* PTJPF2: J3=0,J1=0 */
  clrReg8Bits(PTJPF2, 0xCC);            
  /* PTEPE: PTEPE7=1,PTEPE4=1,PTEPE3=1,PTEPE2=1 */
  setReg8Bits(PTEPE, 0x9C);             
  /* PTEPF1: E7=0,E4=0 */
  clrReg8Bits(PTEPF1, 0xC3);            
  /* PTAPF1: A6=0,A5=0,A4=0 */
  clrReg8Bits(PTAPF1, 0x3F);            
  /* PTCDD: PTCDD7=0,PTCDD6=0,PTCDD4=0 */
  clrReg8Bits(PTCDD, 0xD0);             
  /* PTCPF1: C7=1,C6=1,C4=1 */
  clrSetReg8Bits(PTCPF1, 0xA2, 0x51);   
  /* PTBPE: PTBPE5=1,PTBPE4=1,PTBPE3=1,PTBPE2=1,PTBPE1=1 */
  setReg8Bits(PTBPE, 0x3E);             
  /* PTBDD: PTBDD5=0,PTBDD4=0,PTBDD3=0,PTBDD2=0,PTBDD1=0 */
  clrReg8Bits(PTBDD, 0x3E);             
  /* PTBPF2: B3=0,B2=0,B1=0 */
  clrReg8Bits(PTBPF2, 0xFC);            
  /* PTBPF1: B5=0,B4=0 */
  clrReg8Bits(PTBPF1, 0x0F);            
  /* PTCPE: PTCPE4=0 */
  clrReg8Bits(PTCPE, 0x10);             
  /* PTHPF1: H7=3 */
  setReg8Bits(PTHPF1, 0xC0);            
  /* PTHDD: PTHDD7=1 */
  setReg8Bits(PTHDD, 0x80);             
  /* PTHD: PTHD7=0 */
  clrReg8Bits(PTHD, 0x80);              
  /* PTASE: PTASE7=0,PTASE6=0,PTASE5=0,PTASE4=0,PTASE3=0,PTASE2=0,PTASE1=0,PTASE0=0 */
  setReg8(PTASE, 0x00);                 
  /* PTBSE: PTBSE7=0,PTBSE6=0,PTBSE5=0,PTBSE4=0,PTBSE3=0,PTBSE2=0,PTBSE1=0,PTBSE0=0 */
  setReg8(PTBSE, 0x00);                 
  /* PTCSE: PTCSE7=0,PTCSE6=0,PTCSE5=0,PTCSE4=0,PTCSE3=0,PTCSE2=0,PTCSE1=0,PTCSE0=0 */
  setReg8(PTCSE, 0x00);                 
  /* PTDSE: PTDSE7=0,PTDSE6=0,PTDSE5=0,PTDSE4=0,PTDSE3=0,PTDSE2=0,PTDSE1=0,PTDSE0=0 */
  setReg8(PTDSE, 0x00);                 
  /* PTESE: PTESE7=0,PTESE6=0,PTESE5=0,PTESE4=0,PTESE3=0,PTESE2=0,PTESE1=0,PTESE0=0 */
  setReg8(PTESE, 0x00);                 
  /* PTFSE: PTFSE7=0,PTFSE6=0,PTFSE5=0,PTFSE4=0,PTFSE3=0,PTFSE2=0,PTFSE1=0,PTFSE0=0 */
  setReg8(PTFSE, 0x00);                 
  /* PTGSE: PTGSE7=0,PTGSE6=0,PTGSE5=0,PTGSE4=0,PTGSE3=0,PTGSE2=0,PTGSE1=0,PTGSE0=0 */
  setReg8(PTGSE, 0x00);                 
  /* PTHSE: PTHSE7=0,PTHSE6=0,PTHSE5=0,PTHSE4=0,PTHSE3=0,PTHSE2=0,PTHSE1=0,PTHSE0=0 */
  setReg8(PTHSE, 0x00);                 
  /* PTJSE: PTJSE5=0,PTJSE4=0,PTJSE3=0,PTJSE2=0,PTJSE1=0,PTJSE0=0 */
  clrReg8Bits(PTJSE, 0x3F);             
  /* PTADS: PTADS7=1,PTADS6=1,PTADS5=1,PTADS4=1,PTADS3=1,PTADS2=1,PTADS1=1,PTADS0=1 */
  setReg8(PTADS, 0xFF);                 
  /* PTBDS: PTBDS7=1,PTBDS6=1,PTBDS5=1,PTBDS4=1,PTBDS3=1,PTBDS2=1,PTBDS1=1,PTBDS0=1 */
  setReg8(PTBDS, 0xFF);                 
  /* PTCDS: PTCDS7=1,PTCDS6=1,PTCDS5=1,PTCDS4=1,PTCDS3=1,PTCDS2=1,PTCDS1=1,PTCDS0=1 */
  setReg8(PTCDS, 0xFF);                 
  /* PTDDS: PTDDS7=1,PTDDS6=1,PTDDS5=1,PTDDS4=1,PTDDS3=1,PTDDS2=1,PTDDS1=1,PTDDS0=1 */
  setReg8(PTDDS, 0xFF);                 
  /* PTEDS: PTEDS7=1,PTEDS6=1,PTEDS5=1,PTEDS4=1,PTEDS3=1,PTEDS2=1,PTEDS1=1,PTEDS0=1 */
  setReg8(PTEDS, 0xFF);                 
  /* PTFDS: PTFDS7=1,PTFDS6=1,PTFDS5=1,PTFDS4=1,PTFDS3=1,PTFDS2=1,PTFDS1=1,PTFDS0=1 */
  setReg8(PTFDS, 0xFF);                 
  /* PTGDS: PTGDS7=1,PTGDS6=1,PTGDS5=1,PTGDS4=1,PTGDS3=1,PTGDS2=1,PTGDS1=1,PTGDS0=1 */
  setReg8(PTGDS, 0xFF);                 
  /* PTHDS: PTHDS7=1,PTHDS6=1,PTHDS5=1,PTHDS4=1,PTHDS3=1,PTHDS2=1,PTHDS1=1,PTHDS0=1 */
  setReg8(PTHDS, 0xFF);                 
  /* PTJDS: ??=0,??=0,PTJDS5=1,PTJDS4=1,PTJDS3=1,PTJDS2=1,PTJDS1=1,PTJDS0=1 */
  setReg8(PTJDS, 0x3F);                 
  /* PTAIFE: PTAIFE7=0,PTAIFE6=0,PTAIFE5=0,PTAIFE4=0,PTAIFE3=0,PTAIFE2=0,PTAIFE1=0,PTAIFE0=0 */
  setReg8(PTAIFE, 0x00);                
  /* PTBIFE: PTBIFE7=0,PTBIFE6=0,PTBIFE5=0,PTBIFE4=0,PTBIFE3=0,PTBIFE2=0,PTBIFE1=0,PTBIFE0=0 */
  setReg8(PTBIFE, 0x00);                
  /* PTCIFE: PTCIFE7=0,PTCIFE6=0,PTCIFE5=0,PTCIFE4=0,PTCIFE3=0,PTCIFE2=0,PTCIFE1=0,PTCIFE0=0 */
  setReg8(PTCIFE, 0x00);                
  /* PTDIFE: PTDIFE7=0,PTDIFE5=0,PTDIFE4=0,PTDIFE3=0,PTDIFE2=0,PTDIFE1=0,PTDIFE0=0 */
  clrReg8Bits(PTDIFE, 0xBF);            
  /* PTEIFE: PTEIFE7=0,PTEIFE6=0,PTEIFE5=0,PTEIFE4=0,PTEIFE3=0,PTEIFE2=0,PTEIFE1=0,PTEIFE0=0 */
  setReg8(PTEIFE, 0x00);                
  /* PTFIFE: PTFIFE7=0,PTFIFE6=0,PTFIFE5=0,PTFIFE4=0,PTFIFE3=0,PTFIFE2=0,PTFIFE1=0,PTFIFE0=0 */
  setReg8(PTFIFE, 0x00);                
  /* PTGIFE: PTGIFE7=0,PTGIFE6=0,PTGIFE5=0,PTGIFE4=0,PTGIFE3=0,PTGIFE2=0,PTGIFE1=0,PTGIFE0=0 */
  setReg8(PTGIFE, 0x00);                
  /* PTHIFE: PTHIFE7=0,PTHIFE6=0,PTHIFE5=0,PTHIFE4=0,PTHIFE3=0,PTHIFE2=0,PTHIFE1=0,PTHIFE0=0 */
  setReg8(PTHIFE, 0x00);                
  /* PTJIFE: PTJIFE5=0,PTJIFE4=0,PTJIFE3=0,PTJIFE2=0,PTJIFE1=0,PTJIFE0=0 */
  clrReg8Bits(PTJIFE, 0x3F);            
  /* PTCPF2: C3=0 */
  clrReg8Bits(PTCPF2, 0xC0);            
  /* SOPT3: CS=0,PCS=0 */
  clrReg8Bits(SOPT3, 0x1F);             
  /* ### Shared modules init code ... */
  /* ### TimerInt "TimerDelay" init code ... */
  TimerDelay_Init();
  /* ###  Synchro master "RxTx_spi" init code ... */
  RxTx_spi_Init();
  /* ### BitIO "sdCS" init code ... */
  /* ### BitIO "ledGreen" init code ... */
  /* ### BitIO "ledRed" init code ... */
  /* ### BitIO "lcdBKLITE" init code ... */
  /* ### BitIO "sdDETECT" init code ... */
  /* ### BitIO "grphRESET" init code ... */
  /* ### BitIO "grphSCL" init code ... */
  /* ### BitIO "grphRS" init code ... */
  /* ### BitIO "grphSI" init code ... */
  /* ### TimerInt "ModulusTimer" init code ... */
  ModulusTimer_Init();
  /* ### BitIO "flashCS" init code ... */
  /* ### TimerInt "swDebounceTimer" init code ... */
  swDebounceTimer_Init();
  /* ### BitIO "audEnable" init code ... */
  Shadow_PTE &= (byte)~0x80;           /* Initialize pin shadow variable bit */
  /* ### InternalI2C "i2c" init code ... */
  i2c_Init();
  /* ### TimerInt "peTimer" init code ... */
  peTimer_Init();
  /* ### BitsIO "swPort" init code ... */
  /* ### External interrupt "rtcINT" init code ... */
  /* IRQSC: ??=0,IRQPDD=1,IRQEDG=0,IRQPE=1,IRQF=0,IRQACK=0,IRQIE=0,IRQMOD=0 */
  setReg8(IRQSC, 0x50);                 
  /* IRQSC: IRQACK=1 */
  setReg8Bits(IRQSC, 0x04);             
  /* IRQSC: IRQIE=1 */
  setReg8Bits(IRQSC, 0x02);             
  /* ### Programable pulse generation "sndPWM" init code ... */
  sndPWM_Init();
  /* Common peripheral initialization - ENABLE */
  /* TPM1SC: CLKSB=0,CLKSA=1 */
  clrSetReg8Bits(TPM1SC, 0x10, 0x08);   
  /* INTC_WCR: ENB=1,??=0,??=0,??=0,??=0,MASK=0 */
  setReg8(INTC_WCR, 0x80);              
  SR_lock = 0x00;
  /* Set initial interrupt priority 0 */
  asm {
    move.w SR,D0;
    andi.l #0xF8FF,D0;
    move.w D0,SR;
  }
}

/* Initialization of the CPU registers in FLASH */

/* NVPROT: FPS6=1,FPS5=1,FPS4=1,FPS3=1,FPS2=1,FPS1=1,FPS0=1,FPOPEN=1 */
unsigned char NVPROT_INIT @0x0000040D = 0xFF;

/* NVOPT: KEYEN1=0,KEYEN0=1,??=1,??=1,??=1,??=1,SEC1=1,SEC0=1 */
unsigned char NVOPT_INIT @0x0000040F = 0x7F;
/* END Cpu. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
