#include "TypeDef.h"

// device commands
#define SPIFLASH_CMD_DEEP_PWR_DWN_ENTER 0xB9    // deep power down mode
#define SPIFLASH_CMD_DEEP_PWR_DWN_EXIT  0xAB    // resume from deep power down
#define SPIFLASH_CMD_WREN               0x06    // write enable
#define SPIFLASH_CMD_WRDI               0x04    // write disable
#define SPIFLASH_CMD_RDID               0x9F    // read ID register
#define SPIFLASH_CMD_RDSR               0x05    // read status register
#define SPIFLASH_CMD_WRSR               0x01    // write status register
#define SPIFLASH_CMD_READ               0x03    // read
#define SPIFLASH_CMD_FASTREAD           0x0B    // high-speed read
#define SPIFLASH_CMD_PAGEPROG           0x02    // page program
#define SPIFLASH_CMD_SECTERASE          0xD8    // sector erase
#define SPIFLASH_CMD_CHIPERASE          0xC7    // chip erase
#define SPIFLASH_CMD_BLKERASE_4K        0x20    // block erase 4k
#define SPIFLASH_CMD_BLKERASE_32K       0x52    // block erase 32k
#define SPIFLASH_CMD_BLKERASE_64K       0xD8    // block erase 64k
#define SPIFLASH_CMD_BLKERASE           SPIFLASH_CMD_BLKERASE_4K

// status register bits                 
#define SPIFLASH_STATUS_BUSY            0x01    // busy, write in progress
#define SPIFLASH_STATUS_WEN             0x02    // write enable
#define SPIFLASH_STATUS_BP0             0x04    // block protect 0
#define SPIFLASH_STATUS_BP1             0x08    // block protect 1
#define SPIFLASH_STATUS_BP2             0x10    // block protect 2
#define SPIFLASH_STATUS_WPEN            0x80    // write protect enabled

// device constants
// make sure!! SPIFLASH_CMD_BLKERASE  matches calc ERASE_SIZE below

//------------------------------------------------------------------------------
//                      FLASH Definitions
//  the order of the segments is as listed in the order following
//                  WAV_SEG 
//                  CMD_SEG 
//                  DAT_SEG
//
//  Flash memory contains segment types
//      each segment has a calc number of erase blocks
//      each seqment contains files
//      files contain pages
//      pages are a number of bytes
//      *** note *** WAV,CMD,DAT_ERASE_BLOCKS (based on ERASE_SIZE)
//              must manually take into account the ***FILE_SIZE and not be less
//              the file size, and all variables/constants must be multiples of 4k bytes
//------------------------------------------------------------------------------
#define FLASH_MEMORY_SIZE               0x100000         // 8Mb --> 1MB Byte flash memory
#define FLASH_PAGE_SIZE                 0X100           // 256 bytes/page
#define ERASE_SIZE                      0x1000          // 4k bytes block erase size
#define ge_FLASH_FILE_ERASE_BLOCKS         FLASH_MEMORY_SIZE / ERASE_SIZE

//#define SUM_SEG_SIZES                   WAV_SEG_SIZE + CMD_SEG_SIZE + DAT_SEG_SIZE
#define WAV_SEG_SIZE                    0x60000         // .6M bytes reserved for wav files
#define CMD_SEG_SIZE                    0x10000         // .1M bytes reserved for cmd files
#define DAT_SEG_SIZE                    0x10000         // .1M bytes reserved for data files

#define BASE_START                      0x000000
#define WAV_SEG_START                   BASE_START
#define CMD_SEG_START                   WAV_SEG_START + WAV_SEG_SIZE
#define DAT_SEG_START                   CMD_SEG_START + CMD_SEG_SIZE

//------------------------------------------------------------------------------
//                      WAV Definitions
//------------------------------------------------------------------------------
#define MAX_WAV_FILES                   0x02            // number of files
#define WAV_FILE_SIZE                   WAV_SEG_SIZE  / MAX_WAV_FILES
#define WAV_FILE_ERASE_BLOCKS           WAV_FILE_SIZE / ERASE_SIZE
#define WAV_PAGES_PER_FILE              WAV_FILE_SIZE / FLASH_PAGE_SIZE
#define WAV_SEG_ERASE_BLOCKS            WAV_SEG_SIZE  / ERASE_SIZE
                                        
//------------------------------------------------------------------------------
//                      CMD Definitions
//------------------------------------------------------------------------------
#define MAX_CMD_FILES                   0x02            // number of cmd files
#define CMD_FILE_SIZE                   CMD_SEG_SIZE  / MAX_CMD_FILES
#define CMD_FILE_ERASE_BLOCKS           CMD_FILE_SIZE / ERASE_SIZE
#define CMD_PAGES_PER_FILE              CMD_FILE_SIZE / FLASH_PAGE_SIZE
#define CMD_SEG_ERASE_BLOCKS            CMD_SEG_SIZE  / ERASE_SIZE

//------------------------------------------------------------------------------
//                      DATA Definitions
//------------------------------------------------------------------------------
#define MAX_DAT_FILES                  1               // number of data files
#define DAT_FILE_SIZE                  DAT_SEG_SIZE  / MAX_DAT_FILES
#define DAT_FILE_ERASE_BLOCKS          DAT_FILE_SIZE / ERASE_SIZE
#define CMD_PAGES_PER_FILE              CMD_FILE_SIZE  / FLASH_PAGE_SIZE
#define DAT_SEG_ERASE_BLOCKS           DAT_SEG_SIZE  / ERASE_SIZE


#define FILE_NAME_NOT_FOUND             0xFFFFul
#define SLOTS_FULL                      0xFF


#define FLASH_STATUS_EPE                bit(5)
//------------------------------------------------------------------------------
//          memory must be previously erased
//          u32_addr MUST start at a page break
//  OR
//          nbytes must be small enough such that page boundary is not crossed
//------------------------------------------------------------------------------

uint32  u32Fn_flshGetID                 (void);

uint8   u8Fn_flshGetStatusReg           (void);

void    vFn_flshChipErase               (void);

uint16  u16Fn_flshFileErase             (uint32     u32_addr,
                                         uint16     u16_file_type);
                                        
uint16  u16Fn_flshFileEraseVerify       (uint32     u32_addr,
                                         uint32     u32_number_blocks);

uint32  u32Fn_flshFileOpen              (uint16     u16_file_index,
                                         uint16     u16_file_type);

void    vFn_flshFileWrite               (uint32     u32_addr, 
                                         uint32     u32_nbytes, 
                                         char      *cstr_data_ptr);

uint32  u32Fn_flshFileRead              (uint32     u32_addr, 
                                         uint32     u32_nbytes,
                                         char      *c8_data_ptr);
                                        
uint16  u16Fn_flshFileVerify            (uint32     u32_addr,
                                         uint32     u32_nbytes,
                                         char      *c8_data_ptr);
                                        
uint8   u8Fn_flshStatusRegEnable        (void);

uint16  u16Fn_flshWriteToFlash          (char      *c_src_file,
                                         uint16     u16_file_type,
                                         uint16     u16_file_index);

void    vFn_flshTestWrite               (void);

void    vFn_flshTestRead                (void);

uint16  u16Fn_flshSdFileVerify          (char      *pc_sound_file,
                                         uint16     u16_file_type,
                                         uint16     u16_file_index);
