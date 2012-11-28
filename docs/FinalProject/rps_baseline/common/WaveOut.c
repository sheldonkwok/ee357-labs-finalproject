#include <stdio.h>
#include <string.h>

#include "sdOptions.h"                                      // includes various options here, read only, read write code for SD etc
#include "cpu.h"
#include "sndPWM.h"
#include "ModulusTimer.h"

#include "TypeDef.h"                                        // typedefs
#include "sdEnumDef.h"
#include "EnumDef.h"

#include "SwitchStructDef.h"
#include "ExtSwitchDeclar.h"

#include "sdFileConstDef.h"
#include "sdWaveConstDef.h"
#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdFcnDef.h"
#include "sdExtFileDeclar.h"

#include "WaveStructDeclar.h"
#include "FcnDef.h"
#include "ExtGlobalVarDeclar.h"

#include "spiFlash.h"
#include "flashCs.h"

#include "RxTx_spi.h"
#include "audEnable.h"

uint8           gu8_FileFinishedFlag, gu8_BlockFinishedFlag;

uint16          gu16_BlockRemainder, gu16_LastElement, gu16_DataIndex, u16_BitsPerSample;
uint32          gu32_TotalBlockNumber, gu32_ByteCntr, gu32_DataChunkSize, gu32_BlockCntr;
uint32          u32_curr_flash_addr;

char            c_SampleIndex, wave_buff[80], *gc_WaveDataPtr,
                   gca_WaveData1[READ_BUFF_SIZE], gca_WaveData2[READ_BUFF_SIZE];

void    vFn_set_sample_freq             (void);
void    vFn_swap_endian_quad            (uint32            *pu32_src_ptr);
void    vFn_swap_endian_dual            (uint16            *pu16_src_ptr);

uint32  u32Fn_get_data_block_size       (uint16             u16_input_stream,
                                         uint32             u32_curr_flash_addr,
                                         FilePtr            ps_wave_file_ptr);

uint32  vFn_read_sub_chunk              (uint16              u16_input_stream,
                                         uint32             u32_curr_flash_addr,
                                         FilePtr            ps_wave_file_ptr);

u_chunk                     gs_Chunk;                       // define the format and data chunks
u_chunk_ptr                 gs_ChunkPtr;
f_sub_chunk                 gs_SubChunk;                    // define the fact format and chunk
f_sub_chunk_ptr             gs_SubChunkPtr;                 // define the fact format and chunk

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( u16Fn_WaveOut )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       reads input wave file from sd card, triggers playing of file in pwm 
    
    Input:          pointer to wave file name

    Return:         none
    
    Notes:          see sdWaveConstDef for buffer sizes
                    manual load of duty cycle register, instead of PE bean
                        PWMDTY4_pwm = *(gc_WaveDataPtr + ((gu16_DataIndex * gu8_SampleIndex) + gu8_ChannelSide));     // 8 bit, load first count to start current block
//rwl         fread_iMn
//rwl             (wave_buff,                  // read in RIFF, type WAVE, and Format chunk
//rwl              sizeof(wave_buff), 
//rwl              1, 
//rwl              ps_wave_file_ptr);                          //Read whole format chunk
                if(gu32_BlockCntr == 0x415){
                    u32_byte_cntr_sav++;                // debugging statement, block
                }
                if(gu32_ByteCntr < gu32_DataChunkSize){
                    u32_byte_cntr_sav = gu32_ByteCntr;
                }else {
                    u32_byte_cntr_sav++;
                }


----------------------------------------------------------------------------------------------------*/
uint16 u16Fn_WaveOut (char      *cstr_sound_file_ptr,
                      uint16    u16_input_stream,
                      uint16    u16_file_index)
{

uint32          u32_data_chunk_size;
uint32          u32_byte_cntr_sav;

uint16  u16_extra_format_bytes, u16_wave_status;

uint8           ua8_data_id[4], u8_refresh_buffer_flag, u8_wave_array_index;
uint8           u8_first_value;
char            *cstr_buffer_ptr; 
                    

FilePtr         ps_wave_file_ptr;                           // pointer to the file when assigned by fcreatec

    swResetFlags
       (ge_SW_LFT,
        ge_SW_RT,
        ge_SW_UP,
        ge_SW_DOWN,
        ge_SW_SEL);                                         // use switch to exit, therefore init flags

    RxTx_spi_Enable();
    RxTx_spi_EnableEvent();
    audEnable_SetVal();                                     // enable amp

    gs_ChunkPtr = &gs_Chunk;
    gs_SubChunkPtr = &gs_SubChunk;                          // for data chunk
    u16_wave_status = ge_SUCCESS;

    if(u16_input_stream == ge_SD_FILE_STREAM){
        ps_wave_file_ptr = fopen_iMn
                         (cstr_sound_file_ptr, 
                          READ);                            //Open file for input
        if(ps_wave_file_ptr != 0){
            fseek_iMn
                (ps_wave_file_ptr,
                 0L,
                 SD_SEEK_SET);                              //Point to beginning of in_file
            fread_iMn
                ( &(gs_ChunkPtr->main_chunk.riff_chunk),
                 sizeof(gs_ChunkPtr->main_chunk.riff_chunk),
                 1, 
                 ps_wave_file_ptr);                         //read in RIFF, type WAVE
            fread_iMn
                ( &(gs_ChunkPtr->main_chunk.fmt_chunk),
                 sizeof(gs_ChunkPtr->main_chunk.fmt_chunk), 
                 1, 
                 ps_wave_file_ptr);                         //Read fmt subchunkID and size
         }else {
             u16_wave_status = ge_FAIL_FILE_OPEN;           // file not found
         }
    }else {
    if(u16_input_stream == ge_FLASH_MEMORY_STREAM){
       u32_curr_flash_addr = u32Fn_flshFileOpen
                                (u16_file_index,
                                 ge_WAV_FILE);              // index # of file
       u32_curr_flash_addr = u32Fn_flshFileRead
                                ( u32_curr_flash_addr,
                                  sizeof(gs_ChunkPtr->main_chunk.riff_chunk), 
                          (char *)&(gs_ChunkPtr->main_chunk.riff_chunk) ); //Read rif chunk
       u32_curr_flash_addr = u32Fn_flshFileRead
                                ( u32_curr_flash_addr,
                                  sizeof(gs_ChunkPtr->main_chunk.fmt_chunk), 
                          (char *)&(gs_ChunkPtr->main_chunk.fmt_chunk) ); //Read fmt chunk
    }}

    if(u16_wave_status = ge_SUCCESS){

        vFn_swap_endian_quad( &(gs_ChunkPtr->main_chunk.fmt_chunk.u32_chunk_data_size) ); // cvt to big endian for chunksize

        if(u16_input_stream == ge_SD_FILE_STREAM){
            fread_iMn
                ( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder),
                 gs_ChunkPtr->main_chunk.fmt_chunk.u32_chunk_data_size, 
                 1, 
                 ps_wave_file_ptr);                             //Read fmt chunk
        }else {
        if(u16_input_stream == ge_FLASH_MEMORY_STREAM){
            u32_curr_flash_addr = u32Fn_flshFileRead
                                    ( u32_curr_flash_addr,
                                      gs_ChunkPtr->main_chunk.fmt_chunk.u32_chunk_data_size, 
                              (char *)&(gs_ChunkPtr->main_chunk.fmt_chunk_remainder) );                             //Read rest of chunk
        }}

        vFn_swap_endian_dual( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_comp_code) );          // this is file data size, cvt to big endian
        if( (strncmp( (gs_ChunkPtr->main_chunk.riff_chunk.c_chunk_id), "RIFF", 4)    == 0) && // not null terminated, use 4 byte compare
            (strncmp( (gs_ChunkPtr->main_chunk.riff_chunk.c_riff_type), "WAVE", 4)   == 0) && 
            (gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_comp_code               == 0x0001) ){ // file type must be PCM 

            vFn_swap_endian_quad( &(gs_ChunkPtr->main_chunk.riff_chunk.u32_chunk_data_size) ); // cvt to big endian for chunksize
            vFn_swap_endian_dual( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_num_channels) );     // cvt to big endian
            vFn_swap_endian_quad( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u32_samples_rate) );     // cvt to big endian, sub chunk size
            vFn_swap_endian_quad( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u32_avg_bytes_per_sec) );// cvt to big endian, sub chunk size
            vFn_swap_endian_dual( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_bits_per_sample) );  // cvt to big endian
            vFn_swap_endian_dual( &(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_extra_format_bytes) );               // cvt to big endian

            c_SampleIndex = gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_num_channels;
            u16_BitsPerSample = gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u16_bits_per_sample;

            gu32_DataChunkSize = u32Fn_get_data_block_size
                                    (u16_input_stream,
                                     u32_curr_flash_addr,
                                     ps_wave_file_ptr);         // return the size of data remaining based on file type

            gc_WaveDataPtr = gca_WaveData1;                  // pick up the address of the 1st wave data array
            u8_wave_array_index = 1;                            // indicate first buffer is being used

            if(u16_input_stream == ge_SD_FILE_STREAM){
                fread_iMn
                    (gc_WaveDataPtr,
                     READ_BUFF_SIZE, 
                     1, 
                     ps_wave_file_ptr);                        //first read if first block to prime buffer, assumes always at least one block
            }else {
            if(u16_input_stream == ge_FLASH_MEMORY_STREAM){
                u32_curr_flash_addr = u32Fn_flshFileRead
                                        (u32_curr_flash_addr,
                                         READ_BUFF_SIZE, 
                                         gc_WaveDataPtr);       //first read if first block to prime buffer, assumes always at least one block
            }}

            ModulusTimer_SetFreqHz(gs_ChunkPtr->main_chunk.fmt_chunk_remainder.u32_samples_rate);

            gu16_DataIndex = 0;                                 // cntr for number of bytes in a block based on # channels
            gu32_ByteCntr = 0;
            gu32_BlockCntr = 0;                                 // start at 0 count up
            gu8_FileFinishedFlag = ge_NO;

            gu32_TotalBlockNumber    = gu32_DataChunkSize / READ_BUFF_SIZE;    // calc number of read buffers in file
            gu16_BlockRemainder   = gu32_DataChunkSize % READ_BUFF_SIZE;    // calc remainder of last read buffers in file

            sndPWM_Enable();                                    // turn on PWM
            sndPWM_EnableEvent();

            ModulusTimer_Enable();                              // sets sample interrupt rate
            ModulusTimer_EnableEvent();

            do{
                gu32_ByteCntr = gu32_ByteCntr + c_SampleIndex;  // mono = 1, stereo = 2

                u8_refresh_buffer_flag = ge_YES;                // indicates buffer should be filled
                gu8_BlockFinishedFlag = ge_NO;                  // indicates block is still being processed

                do{                                             // loop to control output of one block
                    if(u8_refresh_buffer_flag == ge_YES){       // loop waiting for for buffer to be output, spin while event.c outputs block,
                        if(u8_wave_array_index == 1){          
                            cstr_buffer_ptr = gca_WaveData2;    // pick up the address of the 2st wave data array
                            u8_wave_array_index = 2;           
                        }else {                             
                            cstr_buffer_ptr = gca_WaveData1;    // pick up the address of the 2st wave data array
                            u8_wave_array_index = 1;
                        }

                        memset(cstr_buffer_ptr, 0x00, READ_BUFF_SIZE);

                        if(gu32_BlockCntr < gu32_TotalBlockNumber){     // if this is not the last block cont
                            gu16_LastElement = READ_BUFF_SIZE;          // need to adjust last index for stereo or mono
                        }else {                                     
                            gu16_LastElement = gu16_BlockRemainder;     // stop at the end of the read buffer remainder
                        }                    

                        if(u16_input_stream == ge_SD_FILE_STREAM){
                            fread_iMn
                                (cstr_buffer_ptr,                        // read in the alt buffer using backup buffer ptr
                                 gu16_LastElement, 
                                 1, 
                                 ps_wave_file_ptr);
                        }else {
                        if(u16_input_stream == ge_FLASH_MEMORY_STREAM){
                            u32_curr_flash_addr = u32Fn_flshFileRead
                                                      (u32_curr_flash_addr,  // read in the alt buffer using backup buffer ptr
                                                       gu16_LastElement, 
                                                       cstr_buffer_ptr);
                        }}

                        u8_refresh_buffer_flag = ge_NO;         // alternate buffer full
                    }
                }while( (gu8_BlockFinishedFlag == ge_NO)    &&
                        (gu8_FileFinishedFlag != ge_YES)    &&
                        (gu32_ByteCntr < gu32_DataChunkSize) ); // waiting for Modulus interrupt rtn to clear flag, and adj gu32_ByteCntr

                gc_WaveDataPtr = cstr_buffer_ptr;              // point to the next array of data, interrupt rtn uses this ptr

            }while( (gu8_FileFinishedFlag != ge_YES)   &&
                    (gu32_ByteCntr < gu32_DataChunkSize)      &&
                    (swStatusFlagsPtr->LftOrRtOrUpOrDwnOrSel.Status != ge_SW_FLAG_SET) );

            if(u16_input_stream == ge_SD_FILE_STREAM){
                fclose_iMn(ps_wave_file_ptr);           //Close in_file
            }
        }else {
            u16_wave_status = ge_FAIL;
        }

        ModulusTimer_Disable();
        ModulusTimer_DisableEvent();

        sndPWM_DisableEvent();                          // wave file finish, turn off PWM
        sndPWM_Disable();
    }

    audEnable_ClrVal();                                 // disable amp

    return(u16_wave_status);
}


/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( u32Fn_get_data_block_size(u16_input_stream) )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       returns the data block size

    Input:          u16_input_stream, either 
                            ge_SD_FILE_STREAM    or
                            ge_FLASH_MEMORY_STREAM
    Return:         u32_chunk_data_size, little-endian
    
    Notes:          selection based on flash or SD file type
                    file pointer, returned pointing at data area (if successful)
------------------------------------------------------------------------------------------------*/
uint32 u32Fn_get_data_block_size(uint16     u16_input_stream,
                                 uint32     u32_curr_flash_addr,
                                 FilePtr    ps_wave_file_ptr){

uint32 u32_chunk_data_size, u32_curr_flash_addr_curr;

    u32_chunk_data_size = 0;                            // init to zero

       u32_curr_flash_addr_curr = vFn_read_sub_chunk
                                     (u16_input_stream,
                                      u32_curr_flash_addr,
                                      ps_wave_file_ptr);
    if(strncmp( (gs_SubChunkPtr->c_chunk_id), "data", 4)     == 0){     // not null terminated string
        vFn_swap_endian_quad( &(gs_SubChunkPtr->u32_chunk_data_size) ); // cvt to big endian for chunksize
        u32_chunk_data_size = gs_SubChunkPtr->u32_chunk_data_size;
    }else {
       u32_curr_flash_addr_curr = vFn_read_sub_chunk
                                    (u16_input_stream,
                                     u32_curr_flash_addr_curr,
                                     ps_wave_file_ptr);
        if(strncmp( (gs_SubChunkPtr->c_chunk_id), "data", 4)     == 0){ // not null terminated string
            vFn_swap_endian_quad( &(gs_SubChunkPtr->u32_chunk_data_size) ); // cvt to big endian for chunksize
            u32_chunk_data_size = gs_SubChunkPtr->u32_chunk_data_size;
        }
    }

    if(u16_input_stream == ge_FLASH_MEMORY_STREAM){      // do not exceed max size, if this is flash wav file
        if(u32_chunk_data_size > WAV_FILE_SIZE){
            u32_chunk_data_size = WAV_FILE_SIZE;     // reset gu32_DataChunkSize or flash wrap around occurs 
        }
    }

    return(u32_chunk_data_size);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_read_sub_chunk(u16_input_stream) )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       reads on sub chunk from file stream

    Input:          u16_input_stream, either 
                            ge_SD_FILE_STREAM    or
                            ge_FLASH_MEMORY_STREAM
    Return:         uint32 the adjusted flash address
    
    Notes:          selection based on flash or SD file type
                    file pointer bumped, sub chunk filled in, returned pointing after chunk
------------------------------------------------------------------------------------------------*/
uint32  vFn_read_sub_chunk(uint16      u16_input_stream,
                           uint32      u32_curr_flash_addr,
                           FilePtr     ps_wave_file_ptr){

    if(u16_input_stream == ge_SD_FILE_STREAM){
        fread_iMn
            (gs_SubChunkPtr,
             sizeof(gs_SubChunk), 
             1, 
             ps_wave_file_ptr);                         //first read 1st sub chunk
    }else {
    if(u16_input_stream == ge_FLASH_MEMORY_STREAM){
        u32_curr_flash_addr = u32Fn_flshFileRead
                                (u32_curr_flash_addr,
                                 sizeof(gs_SubChunk), 
                                 (char*)&(gs_SubChunk));       //read 1st sub chunk
    }}

    return(u32_curr_flash_addr);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_swap_endian_quad )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       swaps high low nibbles in each of two words, four bytes
    
    Input:          uint8            *pu32_src_ptr,

    Return:         none
    
    Notes:
----------------------------------------------------------------------------------------------------*/
void vFn_swap_endian_quad (uint32       *pu32_src_ptr)
{
union{
    uint32 long_in;
    uint8 bytes[4];
} endian_in;

union{
    uint32 long_out;
    uint8 bytes[4];
} endian_out;

    endian_in.long_in = *pu32_src_ptr;                       // move 32 bit word into union

    endian_out.bytes[3] = endian_in.bytes[0];
    endian_out.bytes[2] = endian_in.bytes[1];
    endian_out.bytes[1] = endian_in.bytes[2];
    endian_out.bytes[0] = endian_in.bytes[3];

    *pu32_src_ptr = endian_out.long_out;                     // send long back

/*    endian.ULong = pu32_src_ptr[0];
    endian.ULong += (pu32_src_ptr[1] * 0x00000100ul);
    endian.ULong += (pu32_src_ptr[2] * 0x00010000ul);
    endian.ULong += (pu32_src_ptr[3] * 0x01000000ul);

    pu32_src_ptr[0] = endian.bytes[0];
    pu32_src_ptr[1] = endian.bytes[1];
    pu32_src_ptr[2] = endian.bytes[2];
    pu32_src_ptr[3] = endian.bytes[3];
*/
/*    endian.ULong  = (*(uint8 *)(&pu32_src_ptr));
    endian.ULong += (*(uint8 *)(&pu32_src_ptr + 1)) * 0x00000100ul;
    endian.ULong += (*(uint8 *)(&pu32_src_ptr + 2)) * 0x00010000ul;
    endian.ULong += (*(uint8 *)(&pu32_src_ptr + 3)) * 0x01000000ul;


    (*(uint8 *)(&pu32_src_ptr))     = endian.bytes[0];
    (*(uint8 *)(&pu32_src_ptr + 1)) = endian.bytes[1];
    (*(uint8 *)(&pu32_src_ptr + 2)) = endian.bytes[2];
    (*(uint8 *)(&pu32_src_ptr + 3)) = endian.bytes[3];
*/

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_swap_endian_dual )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       swaps high low nibbles in one word, two bytes
    
    Input:          uint8            *pu32_src_ptr,

    Return:         none
    
    Notes: 
----------------------------------------------------------------------------------------------------*/
void vFn_swap_endian_dual (uint16            *pu16_src_ptr)
{

/*rwl
    byte_low    = src_ptr[0];
    byte_high   = src_ptr[1];
    src_ptr[0]  = byte_high;
    src_ptr[1]  = byte_low;
*/

    *pu16_src_ptr     = (((*pu16_src_ptr) >> 8) | (*pu16_src_ptr) & 0x0F);

    return;
}



