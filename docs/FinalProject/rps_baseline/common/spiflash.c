#include "RPS_BldOptions.h"

#include "EnumDef.h"

#include "spiFlash.h"

#include "grphEnumDef.h"
#include "lcdConstDef.h"
#include "ExtGlobalVarDeclar.h"

#include "TypeDef.h"
#include "spiFlash.h"
#include "flashCS.h"

#include "sdEnumDef.h"
#include "sdFileConstDef.h"
#include "sdWaveConstDef.h"
#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdFcnDef.h"
#include "sdExtFileDeclar.h"
#include "FcnDef.h"
#include "Fonts.h"

#ifdef RPS_SPIFLASH_FULL_LIB
    #include "spiFlash_opt.c"
#endif

/*-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( u8Fn_flshGetStatusReg ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   u8Fn_flshGetStatusReg
    Function:           read the status register
    Input Parameter:    none
    Return Type:        uint8 register data
    
    Notes:
--------------------------------------------------------------------------------------------------*/
uint8 u8Fn_flshGetStatusReg (void)
{
uint8 u8_status_reg;

    flashCS_ClrVal();                                   // assert CS

    u8Fn_RxTx_spi(SPIFLASH_CMD_RDSR);                        // send read status register command
    u8_status_reg = u8Fn_RxTx_spi(0x00);                        // clock in status register

    flashCS_SetVal();                                   // release CS

    return (u8_status_reg);
}

/**-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( u32Fn_flshFileOpen ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   u32Fn_flshFileOpen
    Function:           open the flash file
    Input Parameter:    uint16 u16_file_index
                        uint16     u16_file_type
    Return Type:        start address of fle
    
    Notes:              This is hard coded at present to avoid the need for a directory structure
                            see spiFlash.h for file size defines
                        Check the max files of the file type, do not exceed, if index is > than
                            max defined in spiFlash.h, then set index to last index, ie max file entries
                        Index file location 0 will be overwritten if error in index range
--------------------------------------------------------------------------------------------------*/
uint32 u32Fn_flshFileOpen (uint16   u16_file_index,
                           uint16   u16_file_type)
{
uint32  u32_start_flash_addr, cmd_seg_start, cmd_file_size;

    u32_start_flash_addr = 0;                           // if index error default to 0 start address

    if(u16_file_type == ge_WAV_FILE){
        if(u16_file_index > MAX_WAV_FILES){
            u16_file_index = MAX_WAV_FILES;             // set index to last file if index exceeded
        }else {
            u32_start_flash_addr = (u16_file_index * WAV_FILE_SIZE) + WAV_SEG_START;  // calc start point
        }
    }else {
    if(u16_file_type == ge_CMD_FILE){
        if(u16_file_index > MAX_CMD_FILES){
            u16_file_index = MAX_CMD_FILES;             // set index to last file if index exceeded
        }else {
            cmd_seg_start = CMD_SEG_START;
            cmd_file_size = CMD_FILE_SIZE;
            u32_start_flash_addr = (u16_file_index * cmd_file_size) + cmd_seg_start;  // calc start point
//rwl             u32_start_flash_addr = (u16_file_index * CMD_SEG_START) + CMD_SEG_START;  // calc start point
        }
    }else {
    if(u16_file_type == ge_DAT_FILE){
        if(u16_file_index > MAX_DAT_FILES){
            u16_file_index = MAX_DAT_FILES;             // set index to last file if index exceeded
        }else {
            u32_start_flash_addr = (u16_file_index * DAT_FILE_SIZE) + DAT_SEG_START;  // calc start point
        }
    }}}

    return(u32_start_flash_addr);
}

/**-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( u16Fn_flshWriteToFlash ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   u16Fn_flshWriteToFlash
    Function:           flash a file to fixed file size in spi Flash
    Input Parameter:    int8 *sound_file, pointer to file name string
                        uint16  u16_file_type,
                        uint16  file_index)

    Return Type:        uint8 status
    
    Notes:      using defines
                    define max # files
                    define flash memory size
                    define with calc max file size
                calc max # pages in flash file
                read the data file one page of FLASH_PAGE_SIZE at a time
                    until the EOF_CHAR is received or 
                    max # pages in flash file reached
                    after each page read, program the flash with that page
                although file name passed, this is not used, index calculates offset start addr

                termination conditions:
                    file - total number of bytes of file reached
                           EOF returned by get char file function

                    flash - total number of bytes of max file allocated size reached or
                            total number of bytes of file reached
                            can not terminate on EOF, we are not using full file functions to check status
--------------------------------------------------------------------------------------------------*/
uint16 u16Fn_flshWriteToFlash (char   *cstr_src_ptr,
                               uint16  u16_file_type,
                               uint16  u16_file_index)
{
char    cstr_file_data[FLASH_PAGE_SIZE], cstr_page_string[10],          // buffer for file read
            cstr_create_date[10], cstr_mod_date[10];
int16   i16_file_word = 0;

uint8   uint8_attribute;
uint16  u16_fclus, u16_status;
uint32  u32_char_cntr = 0, u32_page_cntr = 0, u32_page_100_cntr_last, 
            u32_page_100_cntr, u32_fsize, u32_byte_cntr, u32_start_flash_addr, u32_type_file_size;

FilePtr                     ps_file_ptr;                // pointer to the file when assigned by fcreatec

    if(u16_file_type == ge_WAV_FILE){
        u32_type_file_size = WAV_FILE_SIZE;
    }else {
    if(u16_file_type == ge_CMD_FILE){
        u32_type_file_size = CMD_FILE_SIZE;
    }else {
    if(u16_file_type == ge_DAT_FILE){
        u32_type_file_size = DAT_FILE_SIZE;
    }}}

    vFn_grphClrView(FontSizeCurr);                      // clean draw outline

    DispStringNoCursor
       (grphLINE_TWO,                                   // display title
        START_OF_LINE,        
         (uint8 *)"Flashing File",
        ge_CLEAR_LINE,           
        FontSizeCurr);      
    DispStringNoCursor
       (grphLINE_THREE,                                 // display page
        5,        
        (uint8 *)"Pages",
        ge_CLEAR_LINE,           
        FontSizeCurr);      

    u32_start_flash_addr = u32Fn_flshFileOpen
                                (u16_file_index,        // index # of file
                                 u16_file_type);

    ps_file_ptr = fopen_iMn(cstr_src_ptr, READ);         //Open file for input

    fseek_iMn
        (ps_file_ptr,
         0L,
         SD_SEEK_SET);                                  //Point to beginning of in_file

    u16_status = ge_SUCCESS;
#ifdef RPS_SPIFLASH_FULL_LIB
    u16_status = u16Fn_flshFileErase
                     (u32_start_flash_addr,
                      u16_file_type);                   // erase entire file, pointed to in type
#endif
    if(u16_status == ge_SUCCESS){
        u32_byte_cntr = 0;                              // track total char's per file
        i16_file_word = 0;
        u32_page_100_cntr_last = 0;

        fget_file_info
             (cstr_src_ptr,                             // todo use rtc to set mod date in spi flash
             &u32_fsize,                                // need size
              cstr_create_date,
              cstr_mod_date,
             &uint8_attribute,
             &u16_fclus);

        do {
            u32_char_cntr = 0;                                  // counts up to one page of data
            memset(cstr_file_data, 0xFF, sizeof(cstr_file_data)); // clear data array
            while( (u32_char_cntr < FLASH_PAGE_SIZE) &&         // exceed allocated or
                   (u32_byte_cntr < u32_fsize) ){               // actual file size

                i16_file_word = fgetc_iMn(ps_file_ptr);
                cstr_file_data[u32_char_cntr++] = (i16_file_word & 0xFF);// store char into data array
                u32_byte_cntr++;
            }                                                   // get up to a page of data from file

            u32_page_100_cntr = u32_page_cntr / DISP_PAGE_SET;  // display only sets of n pages

            if(u32_page_100_cntr != u32_page_100_cntr_last){    // visual indicator of progress
                u32_page_100_cntr_last = u32_page_100_cntr;
                itoa
                    (cstr_page_string,                           // convert the page cntr to an ascii string
                     u32_page_cntr,
                     0x10);
                DispStringNoCursor
                   (grphLINE_FOUR,
                    6,
                    (uint8*)cstr_page_string,
                    ge_CLEAR_LINE,
                    FontSizeCurr);
            }

            vFn_flshFileWrite
                (u32_start_flash_addr,                          // write page, 
                 FLASH_PAGE_SIZE,
                 cstr_file_data);

#ifdef ge_FLASH_FILE_VERIFY

            u16_status = u16Fn_flshFileVerify
                            (u32_start_flash_addr,              // verify page just written
                             u32_char_cntr,
                             cstr_file_data);
            if(u16_status == ge_FAIL){
                break;                                      // break out of loop if verify fails
            }
#endif
            u32_start_flash_addr = u32_start_flash_addr + FLASH_PAGE_SIZE; // calc next page address
            u32_page_cntr++;
        }while( (u32_byte_cntr < u32_fsize)         &&
                (u32_byte_cntr < u32_type_file_size) );
    }                                                   // if erase failed then just exit with flash status

    fclose_iMn(ps_file_ptr);                          //Close in_file

    return(u16_status);
}

/**-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( u8Fn_flshStatusRegEnable ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   u8Fn_flshStatusRegEnable
    Function:           enable write to status register
    Input Parameter:    none
    Return Type:        status register value
    
    Notes:              WEL must be set prior to every write, erase, status reg change function
                        Each write/erase/modify will clear the WEL register, there this call must 
                            occur before all the above
--------------------------------------------------------------------------------------------------*/
uint8 u8Fn_flshStatusRegEnable (void)
{
uint8 u8_status_reg;

    flashCS_ClrVal();                                   // assert CS
    u8Fn_RxTx_spi(SPIFLASH_CMD_WREN);                   // enable write
    flashCS_SetVal();                                   // release CS
    while(u8Fn_flshGetStatusReg() & SPIFLASH_STATUS_BUSY){};// wait for op to complete
    u8_status_reg = u8Fn_flshGetStatusReg();

    return(u8_status_reg);
}

/**-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( u32Fn_flshFileRead ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   u32Fn_flshFileRead
    Function:           read u32_num_bytes starting at addr
    Input Parameter:    start address, 
                        uint32 u32_num_bytes, 
                        uint8 *pu8_data, ptr to data destination
    Return Type:        uint32 u32_addr
    
    Notes:              returns next address
--------------------------------------------------------------------------------------------------*/
uint32 u32Fn_flshFileRead (uint32 u32_addr,
                           uint32 u32_num_bytes,
                           char  *cstr_data_ptr)
{
    flashCS_ClrVal();                                   // assert CS

    u8Fn_RxTx_spi(SPIFLASH_CMD_READ);                   // read
    u8Fn_RxTx_spi(u32_addr >> 16);                      // address
    u8Fn_RxTx_spi(u32_addr >> 8);
    u8Fn_RxTx_spi(u32_addr >> 0);

    while(u32_num_bytes--){
        *cstr_data_ptr++ = u8Fn_RxTx_spi(0x00);              // read data
         u32_addr++;
    }

    flashCS_SetVal();                                   // release CS

    return(u32_addr);
}

/**-------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((((((( vFn_flshFileWrite ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
   vFn_flshFileWrite
    Function:           write nbytes starting at addr
    Input Parameter:    uint32      u32_addr start address,  
                        uint32      u32_nbytes
                        uint8      *pu8_data_ptr ptr to data source
    Return Type:        none
    
    Notes:              This does buffered page write when CS is released
                        u32_addr *MUST* be on page boundry
--------------------------------------------------------------------------------------------------*/
void vFn_flshFileWrite (uint32      u32_addr, 
                        uint32      u32_nbytes, 
                        char       *cstr_data_ptr)
{
uint16  u16_i;

    u8Fn_flshStatusRegEnable();                         // enable flash to be written

    flashCS_ClrVal();                                   // assert CS, enable status reg update
    u8Fn_RxTx_spi(SPIFLASH_CMD_PAGEPROG);               // write page command
    u8Fn_RxTx_spi(u32_addr>>16);                        // send the start address
    u8Fn_RxTx_spi(u32_addr>>8);
    u8Fn_RxTx_spi(u32_addr>>0);

    for(u16_i = 0; u16_i < u32_nbytes; u16_i++){
        u8Fn_RxTx_spi(*cstr_data_ptr++);                // transfer page
    }
    flashCS_SetVal();                                   // release CS, flash memory will write page

    flashCS_ClrVal();                                   // assert CS, to allow access to status reg
    while(u8Fn_flshGetStatusReg() & SPIFLASH_STATUS_BUSY){};// wait for op to complete
    flashCS_SetVal();                                   // release CS

    u8Fn_RxTx_spi(0x00);                                // delay

    return;
}
//========================================================================================================


