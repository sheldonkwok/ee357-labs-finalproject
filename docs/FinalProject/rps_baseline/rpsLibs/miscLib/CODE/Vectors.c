/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Vectors.c
**     Project   : miscLib
**     Processor : MCF51CN128CLK
**     Version   : Component 01.001, Driver 01.11, CPU db: 3.00.030
**     Compiler  : CodeWarrior ColdFireV1 C Compiler
**     Date/Time : 9/1/2010, 7:40 PM
**     Abstract  :
**         This component "MCF51CN128_80" contains initialization of the
**         CPU and provides basic methods and events for CPU core
**         settings.
**     Settings  :
**
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
#include "Cpu.h"
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
#include "startcf.h"

extern unsigned long far _SP_INIT[];

/* ISR prototype */
typedef void (*const tIsrFunc)(void);

const tIsrFunc _InterruptVectorTable[115] @0x00000000 = { /* Interrupt vector table */
  /* ISR name                               No. Address      Lvl Pri Name          Description */
  (tIsrFunc)_SP_INIT,                  /* 0x00  0x00000000   -   -   ivINITSP      used by PE */
  _startup,                            /* 0x01  0x00000004   -   -   ivINITPC      used by PE */
  Cpu_Interrupt,                       /* 0x02  0x00000008   -   -   ivVaccerr     unused by PE */
  Cpu_Interrupt,                       /* 0x03  0x0000000C   -   -   ivVadderr     unused by PE */
  Cpu_Interrupt,                       /* 0x04  0x00000010   -   -   ivViinstr     unused by PE */
  Cpu_Interrupt,                       /* 0x05  0x00000014   -   -   ivVReserved5  unused by PE */
  Cpu_Interrupt,                       /* 0x06  0x00000018   -   -   ivVReserved6  unused by PE */
  Cpu_Interrupt,                       /* 0x07  0x0000001C   -   -   ivVReserved7  unused by PE */
  Cpu_Interrupt,                       /* 0x08  0x00000020   -   -   ivVprviol     unused by PE */
  Cpu_Interrupt,                       /* 0x09  0x00000024   -   -   ivVtrace      unused by PE */
  Cpu_Interrupt,                       /* 0x0A  0x00000028   -   -   ivVunilaop    unused by PE */
  Cpu_Interrupt,                       /* 0x0B  0x0000002C   -   -   ivVunilfop    unused by PE */
  Cpu_Interrupt,                       /* 0x0C  0x00000030   -   -   ivVdbgi       unused by PE */
  Cpu_Interrupt,                       /* 0x0D  0x00000034   -   -   ivVReserved13 unused by PE */
  Cpu_Interrupt,                       /* 0x0E  0x00000038   -   -   ivVferror     unused by PE */
  Cpu_Interrupt,                       /* 0x0F  0x0000003C   -   -   ivVReserved15 unused by PE */
  Cpu_Interrupt,                       /* 0x10  0x00000040   -   -   ivVReserved16 unused by PE */
  Cpu_Interrupt,                       /* 0x11  0x00000044   -   -   ivVReserved17 unused by PE */
  Cpu_Interrupt,                       /* 0x12  0x00000048   -   -   ivVReserved18 unused by PE */
  Cpu_Interrupt,                       /* 0x13  0x0000004C   -   -   ivVReserved19 unused by PE */
  Cpu_Interrupt,                       /* 0x14  0x00000050   -   -   ivVReserved20 unused by PE */
  Cpu_Interrupt,                       /* 0x15  0x00000054   -   -   ivVReserved21 unused by PE */
  Cpu_Interrupt,                       /* 0x16  0x00000058   -   -   ivVReserved22 unused by PE */
  Cpu_Interrupt,                       /* 0x17  0x0000005C   -   -   ivVReserved23 unused by PE */
  Cpu_Interrupt,                       /* 0x18  0x00000060   -   -   ivVspuri      unused by PE */
  Cpu_Interrupt,                       /* 0x19  0x00000064   -   -   ivVReserved25 unused by PE */
  Cpu_Interrupt,                       /* 0x1A  0x00000068   -   -   ivVReserved26 unused by PE */
  Cpu_Interrupt,                       /* 0x1B  0x0000006C   -   -   ivVReserved27 unused by PE */
  Cpu_Interrupt,                       /* 0x1C  0x00000070   -   -   ivVReserved28 unused by PE */
  Cpu_Interrupt,                       /* 0x1D  0x00000074   -   -   ivVReserved29 unused by PE */
  Cpu_Interrupt,                       /* 0x1E  0x00000078   -   -   ivVReserved30 unused by PE */
  Cpu_Interrupt,                       /* 0x1F  0x0000007C   -   -   ivVReserved31 unused by PE */
  Cpu_Interrupt,                       /* 0x20  0x00000080   -   -   ivVtrap0      unused by PE */
  Cpu_Interrupt,                       /* 0x21  0x00000084   -   -   ivVtrap1      unused by PE */
  Cpu_Interrupt,                       /* 0x22  0x00000088   -   -   ivVtrap2      unused by PE */
  Cpu_Interrupt,                       /* 0x23  0x0000008C   -   -   ivVtrap3      unused by PE */
  Cpu_Interrupt,                       /* 0x24  0x00000090   -   -   ivVtrap4      unused by PE */
  Cpu_Interrupt,                       /* 0x25  0x00000094   -   -   ivVtrap5      unused by PE */
  Cpu_Interrupt,                       /* 0x26  0x00000098   -   -   ivVtrap6      unused by PE */
  Cpu_Interrupt,                       /* 0x27  0x0000009C   -   -   ivVtrap7      unused by PE */
  Cpu_Interrupt,                       /* 0x28  0x000000A0   -   -   ivVtrap8      unused by PE */
  Cpu_Interrupt,                       /* 0x29  0x000000A4   -   -   ivVtrap9      unused by PE */
  Cpu_Interrupt,                       /* 0x2A  0x000000A8   -   -   ivVtrap10     unused by PE */
  Cpu_Interrupt,                       /* 0x2B  0x000000AC   -   -   ivVtrap11     unused by PE */
  Cpu_Interrupt,                       /* 0x2C  0x000000B0   -   -   ivVtrap12     unused by PE */
  Cpu_Interrupt,                       /* 0x2D  0x000000B4   -   -   ivVtrap13     unused by PE */
  Cpu_Interrupt,                       /* 0x2E  0x000000B8   -   -   ivVtrap14     unused by PE */
  Cpu_Interrupt,                       /* 0x2F  0x000000BC   -   -   ivVtrap15     unused by PE */
  Cpu_Interrupt,                       /* 0x30  0x000000C0   -   -   ivVReserved48 unused by PE */
  Cpu_Interrupt,                       /* 0x31  0x000000C4   -   -   ivVReserved49 unused by PE */
  Cpu_Interrupt,                       /* 0x32  0x000000C8   -   -   ivVReserved50 unused by PE */
  Cpu_Interrupt,                       /* 0x33  0x000000CC   -   -   ivVReserved51 unused by PE */
  Cpu_Interrupt,                       /* 0x34  0x000000D0   -   -   ivVReserved52 unused by PE */
  Cpu_Interrupt,                       /* 0x35  0x000000D4   -   -   ivVReserved53 unused by PE */
  Cpu_Interrupt,                       /* 0x36  0x000000D8   -   -   ivVReserved54 unused by PE */
  Cpu_Interrupt,                       /* 0x37  0x000000DC   -   -   ivVReserved55 unused by PE */
  Cpu_Interrupt,                       /* 0x38  0x000000E0   -   -   ivVReserved56 unused by PE */
  Cpu_Interrupt,                       /* 0x39  0x000000E4   -   -   ivVReserved57 unused by PE */
  Cpu_Interrupt,                       /* 0x3A  0x000000E8   -   -   ivVReserved58 unused by PE */
  Cpu_Interrupt,                       /* 0x3B  0x000000EC   -   -   ivVReserved59 unused by PE */
  Cpu_Interrupt,                       /* 0x3C  0x000000F0   -   -   ivVReserved60 unused by PE */
  Cpu_Interrupt,                       /* 0x3D  0x000000F4   -   -   ivVunsinstr   unused by PE */
  Cpu_Interrupt,                       /* 0x3E  0x000000F8   -   -   ivVReserved62 unused by PE */
  Cpu_Interrupt,                       /* 0x3F  0x000000FC   -   -   ivVReserved63 unused by PE */
  rtcINT_Interrupt,                    /* 0x40  0x00000100   7   mid   ivVirq        used by PE */
  Cpu_Interrupt,                       /* 0x41  0x00000104   -   -   ivVlvd        unused by PE */
  Cpu_Interrupt,                       /* 0x42  0x00000108   -   -   ivVlol        unused by PE */
  TimerDelay_Interrupt,                /* 0x43  0x0000010C   6   5   ivVtpm1ch0    used by PE */
  swDebounceTimer_Interrupt,           /* 0x44  0x00000110   6   4   ivVtpm1ch1    used by PE */
  ModulusTimer_Interrupt,              /* 0x45  0x00000114   6   3   ivVtpm1ch2    used by PE */
  Cpu_Interrupt,                       /* 0x46  0x00000118   -   -   ivVtpm1ovf    unused by PE */
  Cpu_Interrupt,                       /* 0x47  0x0000011C   -   -   ivVmtim1      unused by PE */
  Cpu_Interrupt,                       /* 0x48  0x00000120   -   -   ivVtpm2ch0    unused by PE */
  Cpu_Interrupt,                       /* 0x49  0x00000124   -   -   ivVtpm2ch1    unused by PE */
  Cpu_Interrupt,                       /* 0x4A  0x00000128   -   -   ivVtpm2ch2    unused by PE */
  sndPWM_Interrupt,                    /* 0x4B  0x0000012C   5   4   ivVtpm2ovf    used by PE */
  Cpu_Interrupt,                       /* 0x4C  0x00000130   -   -   ivVspi1       unused by PE */
  RxTx_spi_Interrupt,                  /* 0x4D  0x00000134   5   2   ivVspi2       used by PE */
  Cpu_Interrupt,                       /* 0x4E  0x00000138   -   -   ivVmtim2      unused by PE */
  Cpu_Interrupt,                       /* 0x4F  0x0000013C   -   -   ivVsci1err    unused by PE */
  Cpu_Interrupt,                       /* 0x50  0x00000140   -   -   ivVsci1rx     unused by PE */
  Cpu_Interrupt,                       /* 0x51  0x00000144   -   -   ivVsci1tx     unused by PE */
  Cpu_Interrupt,                       /* 0x52  0x00000148   -   -   ivVsci2err    unused by PE */
  Cpu_Interrupt,                       /* 0x53  0x0000014C   -   -   ivVsci2rx     unused by PE */
  Cpu_Interrupt,                       /* 0x54  0x00000150   -   -   ivVsci2tx     unused by PE */
  Cpu_Interrupt,                       /* 0x55  0x00000154   -   -   ivVsci3or     unused by PE */
  Cpu_Interrupt,                       /* 0x56  0x00000158   -   -   ivVfectxf     unused by PE */
  Cpu_Interrupt,                       /* 0x57  0x0000015C   -   -   ivVfecrxf     unused by PE */
  Cpu_Interrupt,                       /* 0x58  0x00000160   -   -   ivVfecother   unused by PE */
  Cpu_Interrupt,                       /* 0x59  0x00000164   -   -   ivVfechberr   unused by PE */
  Cpu_Interrupt,                       /* 0x5A  0x00000168   -   -   ivVfecbabr    unused by PE */
  Cpu_Interrupt,                       /* 0x5B  0x0000016C   -   -   ivVfecbabt    unused by PE */
  Cpu_Interrupt,                       /* 0x5C  0x00000170   -   -   ivVfecgra     unused by PE */
  Cpu_Interrupt,                       /* 0x5D  0x00000174   -   -   ivVfectxb     unused by PE */
  Cpu_Interrupt,                       /* 0x5E  0x00000178   -   -   ivVfecrxb     unused by PE */
  Cpu_Interrupt,                       /* 0x5F  0x0000017C   -   -   ivVfecmii     unused by PE */
  Cpu_Interrupt,                       /* 0x60  0x00000180   -   -   ivVfeceberr   unused by PE */
  Cpu_Interrupt,                       /* 0x61  0x00000184   -   -   ivVfeclc      unused by PE */
  Cpu_Interrupt,                       /* 0x62  0x00000188   -   -   ivVfecrl      unused by PE */
  Cpu_Interrupt,                       /* 0x63  0x0000018C   -   -   ivVfecun      unused by PE */
  Cpu_Interrupt,                       /* 0x64  0x00000190   -   -   ivVsci3err    unused by PE */
  Cpu_Interrupt,                       /* 0x65  0x00000194   -   -   ivVsci3rx     unused by PE */
  Cpu_Interrupt,                       /* 0x66  0x00000198   -   -   ivVsci3tx     unused by PE */
  Cpu_Interrupt,                       /* 0x67  0x0000019C   -   -   ivVL7swi      unused by PE */
  Cpu_Interrupt,                       /* 0x68  0x000001A0   -   -   ivVL6swi      unused by PE */
  Cpu_Interrupt,                       /* 0x69  0x000001A4   -   -   ivVL5swi      unused by PE */
  Cpu_Interrupt,                       /* 0x6A  0x000001A8   -   -   ivVL4swi      unused by PE */
  Cpu_Interrupt,                       /* 0x6B  0x000001AC   -   -   ivVL3swi      unused by PE */
  Cpu_Interrupt,                       /* 0x6C  0x000001B0   -   -   ivVL2swi      unused by PE */
  Cpu_Interrupt,                       /* 0x6D  0x000001B4   -   -   ivVL1swi      unused by PE */
  Cpu_Interrupt,                       /* 0x6E  0x000001B8   -   -   ivViic1       unused by PE */
  i2c_Interrupt,                       /* 0x6F  0x000001BC   1   mid   ivViic2       used by PE */
  Cpu_Interrupt,                       /* 0x70  0x000001C0   -   -   ivVadc        unused by PE */
  Cpu_Interrupt,                       /* 0x71  0x000001C4   -   -   ivVkeyboard   unused by PE */
  peTimer_Interrupt                    /* 0x72  0x000001C8   1   1   ivVrtc        used by PE */
};
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
