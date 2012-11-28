#include "TypeDef.h"

#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdEnumDef.h"
#include "sdExtFileDeclar.h"
#include "sdFcnDef.h"

#define         BLOCK_TEST_SIZE                 1048576 *5  // 5MB

void                vFn_FileSpeedTest               (void);

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( FileSpeedTest ))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:  FileSpeedTest

    Input:     none

    Return:    none
    
    Notes:     Block writes and reads from a test file called speedtst.dat
                ** Function that runs the demo program to show how to use the block writing 
                ** capabilities of the SD media.
----------------------------------------------------------------------------------------------------*/
void vFn_FileSpeedTest (void)
{
    File_iMn *fp;
    uint32 cntr = 0;
    uint8 char_cntr;

    int8 _FF_StreamNameStr[]  = "BIG_FILE.TXT";
    uint32 SDBlockWriteBlockCnt = 0;

    if (fstream_file(_FF_StreamNameStr, BLOCK_TEST_SIZE))       // Reserve maximum chars for the file
    {
        return;                                                 // exit if fail
    }

    char_cntr = 0x20;
    do{
        if(char_cntr < 0x60){
            SD_write_block_byte(char_cntr++);
        }else {
            char_cntr = 0x20;
            SD_write_block_byte('/r');
        }
        SDBlockWriteBlockCnt++;
    }while(SDBlockWriteBlockCnt < BLOCK_TEST_SIZE);

    // End the block write
    if (SD_write_block_end())
    {
        return;                                                 // exit if fail
    }

    // Reopen the file in APPEND mode to change the file
    fp = fopen_iMn(_FF_StreamNameStr, APPEND);
    if (fp == NULL_CHAR)
    {
        return;                                                 // exit if fail
    }
    // Find where the data was done writing
    if (fseek_iMn(fp, SDBlockWriteBlockCnt, SD_SEEK_SET))
    {
        return;                                                 // exit if fail
    }
    // Current cluster should be the last now, erase the cluster chain
    if (erase_clus_chain(fp->clus_current))
    {
        return;                                                 // exit if fail
    }
    // Set current cluster to EOF marker
    if (write_clus_table(fp->clus_current, EOF_CHAR, SINGLE))
    {
        return;                                                 // exit if fail
    }
    // Manually Reset File Size so the TOC gets updated right
    fp->length.uval32 = SDBlockWriteBlockCnt;

    // Close file, saves new changes
    if (fclose_iMn(fp))
    {
        return;                                                 // exit if fail
    }

    return;
}


