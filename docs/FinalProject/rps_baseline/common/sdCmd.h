//#include "sdn_typedef.h"

/***************************** C HEADER File_iMn ********************************
**
**  Project:    FlashFile
**  Filename:   SD_CMD.H 
**  Version:    3.0 
**  Date:       March 29, 2006
**  
*****************************************************************************
*****************************************************************************
**
**  VERSION HISTORY:
**  ----------------
**  Version:    3.0 
**  Date:       March 398, 2006
**  Revised by: Erick M. Higa
**  Description:
**      - See "FILE_SYS.C" file for any chages up to this point.
**
****************************************************************************/

#ifndef _SD_CMD_INCLUDED
#define _SD_CMD_INCLUDED

#define _FF_PATH_LENGTH                                 100
#define EOF_CHAR                                        -1

/****************************************************************************
**
**    MODULES USED 
**
****************************************************************************/

/****************************************************************************
**
**    DEFINITIONS AND MACROS
**
****************************************************************************/
// SD/MMC COMMANDS             ARGUMENT       RESPONSE TYPE        DESCRIPTION                 
#define CMD0     0x40        // NO_ARG           RESP_1            GO_IDLE_STATE               
#define CMD1     0x41        // NO_ARG           RESP_1            SEND_OP_COND (ACMD41 = 0x69)
#define CMD9     0x49        // NO_ARG           RESP_1            SEND_CSD                    
#define CMD10    0x4A        // NO_ARG           RESP_1            SEND_CID                    
#define CMD12    0x4C        // NO_ARG           RESP_1            STOP_TRANSMISSION           
#define CMD13    0x4D        // NO_ARG           RESP_2            SEND_STATUS                 
#define CMD16    0x50        // BLOCK_LEN        RESP_1            SET_BLOCKLEN                
#define CMD17    0x51        // DATA_ADDR        RESP_1            READ_SINGLE_BLOCK           
#define CMD18    0x52        // DATA_ADDR        RESP_1            READ_MULTIPLE_BLOCK         
#define CMD24    0x58        // DATA_ADDR        RESP_1            WRITE_BLOCK                 
#define CMD25    0x59        // DATA_ADDR        RESP_1            WRITE_MULTIPLE_BLOCK        
#define CMD27    0x5B        // NO_ARG           RESP_1            PROGRAM_CSD                 
#define CMD28    0x5C        // DATA_ADDR        RESP_1b           SET_WRITE_PROT              
#define CMD29    0x5D        // DATA_ADDR        RESP_1b           CLR_WRITE_PROT              
#define CMD30    0x5E        // DATA_ADDR        RESP_1            SEND_WRITE_PROT             
#define CMD32    0x60        // DATA_ADDR        RESP_1            TAG_SECTOR_START            
#define CMD33    0x61        // DATA_ADDR        RESP_1            TAG_SECTOR_END              
#define CMD34    0x62        // DATA_ADDR        RESP_1            UNTAG_SECTOR                
#define CMD35    0x63        // DATA_ADDR        RESP_1            TAG_ERASE_GROUP_START       
#define CMD36    0x64        // DATA_ADDR        RESP_1            TAG_ERASE_GROUP_END         
#define CMD37    0x65        // DATA_ADDR        RESP_1            TAG_ERASE_GROUP             
#define CMD38    0x66        // STUFF_BITS       RESP_1b           ERASE                       
#define CMD42    0x6A        // STUFF_BITS       RESP_1b           LOCK_UNLOCK                 
#define CMD58    0x7A        // NO_ARG           RESP_3            READ_OCR                    
#define CMD59    0x7B        // STUFF_BITS       RESP_1            CRC_ON_OFF                  
#define ACMD41   0x69        // NO_ARG           RESP_1                                        
 
#define SD_START_TOKEN    0xFE
#define SD_MULTI_START    0xFC
#define SD_STOP_TRANS     0xFD

#ifdef _DEBUG_FUNCTIONS_
    #define _FF_READ_DEBUG_
    #define _FF_WRITE_DEBUG_
    #define _DEBUG_MULTIBLOCK_BYTE_
    #define _DEBUG_MULTI_BLOCK_
    #define _DEBUG_INIT_SD_
#endif

enum {SD_IDLE, SD_SPI};

/****************************************************************************
**
**    TYPEDEFS AND STRUCTURES
**
****************************************************************************/
typedef struct
{
    #ifndef _BIG_ENDIAN_
        uint16 nib_0 : 4;
        uint16 nib_1 : 4;
        uint16 nib_2 : 4;
        uint16 nib_3 : 4;
    #else
        uint16 nib_3 : 4;
        uint16 nib_2 : 4;
        uint16 nib_1 : 4;
        uint16 nib_0 : 4;
    #endif
} Nibbles16Struct;

typedef struct 
{
    #ifndef _BIG_ENDIAN_
        uint8 lo;
        uint8 ml;
        uint8 mh;
        uint8 hi;
    #else
        uint8 hi;
        uint8 mh;
        uint8 ml;
        uint8 lo;
    #endif
} HiLo_8_32_Struct;

typedef struct 
{
    #ifndef _BIG_ENDIAN_
        uint8 lo;
        uint8 hi;
    #else
        uint8 hi;
        uint8 lo;
    #endif
} HiLo_8_16_Struct;

typedef struct 
{
    #ifndef _BIG_ENDIAN_
        uint16 lo;
        uint16 hi;
    #else
        uint16 hi;
        uint16 lo;
    #endif
} HiLo_16_32_Struct;


typedef union 
{
    uint32 uval32;
    HiLo_8_32_Struct uval8;
    HiLo_16_32_Struct uval16;
} HiLo32Union;

typedef union 
{
    uint16 uval16;
    HiLo_8_16_Struct uval8;
    Nibbles16Struct nib;
} HiLo16Union;


typedef struct
{
    uint8 Active_Flagg;
    uint8 Starting_Head;
    uint8 Starting_Cylinder_LO;
    uint8 Starting_Cylinder_HI;
    uint8 Partition_Type;
    uint8 Ending_Head;
    uint8 Ending_Cylinder_LO;
    uint8 Ending_Cylinder_HI;
    uint8 Starting_Sector_LO;
    uint8 Starting_Sector_ML;
    uint8 Starting_Sector_MH;
    uint8 Starting_Sector_HI;
    uint8 Partition_Length_LO;
    uint8 Partition_Length_ML;
    uint8 Partition_Length_MH;
    uint8 Partition_Length_HI;
} PartitionEntryStruct;

typedef struct
{
    uint8 jmpBoot[3];
    uint8 OEMName[8];
    uint8 BytsPerSec_LO;
    uint8 BytsPerSec_HI;
    uint8 SecPerClus;
    uint8 RsvdSecCnt_LO;
    uint8 RsvdSecCnt_HI;
    uint8 NumFATs;
    uint8 RootEntCnt_LO;
    uint8 RootEntCnt_HI;
    uint8 TotSec16_LO;
    uint8 TotSec16_HI;
    uint8 Media;
    uint8 FATSz16_LO;
    uint8 FATSz16_HI;
    uint8 SecPerTrk_LO;
    uint8 SecPerTrk_HI;
    uint8 NumHeads_LO;
    uint8 NumHeads_HI;
    uint8 HiddSec_LO;
    uint8 HiddSec_ML;
    uint8 HiddSec_MH;
    uint8 HiddSec_HI;
    uint8 TotSec32_LO;
    uint8 TotSec32_ML;
    uint8 TotSec32_MH;
    uint8 TotSec32_HI;
    uint8 DrvNum;
    uint8 Reserved1;
    uint8 BootSig;
    uint8 VolID_LO;
    uint8 VolID_ML;
    uint8 VolID_MH;
    uint8 VolID_HI;
    uint8 VolLab[11];
    uint8 FilSysType[8];
    uint8 junk[384];
    PartitionEntryStruct PartEnt[4];
    uint8 SigWord_LO;
    uint8 SigWord_HI;
} FAT16BootSectStruct;
#endif  /*_SD_CMD_INCLUDED*/

/****************************************************************************
** 
**    EXPORTED VARIABLES
**
****************************************************************************/
#ifndef _SD_CMD_C_SRC
    extern uint8 _FF_Buff[512];
    #ifndef _BYTES_PER_SEC_512_
        extern HiLo16Union BPB_BytsPerSec;
    #endif
    extern uint8 BPB_SecPerClus;
    extern uint8 BPB_FATType;
    extern int8 BS_VolLab[12];
    extern uint16 _FF_PartitionAddr;
    extern uint32 _FF_RootAddr;
    extern HiLo16Union BPB_RsvdSecCnt;
    extern HiLo16Union BPB_FATSz16;
    extern HiLo32Union BS_VolSerial;
    extern HiLo16Union BPB_RootEntCnt;
    #ifdef _DIRECTORIES_SUPPORTED_
        extern uint32 _FF_DirAddr;
    #endif
    extern uint16 _FF_Fat1Addr;
    extern uint32 _FF_Fat2Addr;
    extern uint16 FirstDataSector;
    extern uint8 _FF_error;
    extern uint32 _FF_BuffAddr;
    extern uint16 DataClusTot;
    extern int8 _FF_FullPath[_FF_PATH_LENGTH];
    extern uint16 Clus0Counter;
    #if defined(_SD_BLOCK_WRITE_) && !defined(_READ_ONLY_)
        extern uint32 SDBlockWriteBlockCnt;
    #endif
    #ifdef _DEBUG_ON_
        extern int8 const _FF_CRLFStr[];
        extern int8 const _FF_02XStr[];
    #endif
#endif  /*_SD_CMD_C_SRC*/

/****************************************************************************
**
**    EXPORTED FUNCTIONS
**
****************************************************************************/
#ifdef _DEBUG_ON_
    int16 _FF_putchar(char send_char);
    int16 _FF_getchar(void);
    void printf(int8 const *pstr, ...);
        void printf(int8 const *pstr, ...);
#endif
int8 reset_sd(void);
int8 init_sd(void);
#ifdef _DEBUG_ON_
    void _FF_read_disp(uint32 sd_addr);
#endif
int8 _FF_read(uint32 sd_addr, uint8 *sd_read_buff);
uint8 initialize_media(void);
int8 set_SD_mode(uint8 mode);
#ifndef _READ_ONLY_
    int8 _FF_write(uint32 sd_addr, uint8 *sd_write_buff);
    #if defined(_SD_BLOCK_WRITE_)
        int8 SD_write_block_start(uint32 sd_addr, uint32 blk_size);
        int8 SD_write_block_byte(uint8 sd_data);
        int8 SD_write_block_end(void);
    #endif
#endif


/****************************************************************************
**
**    EOF 
**
****************************************************************************/
