/*============================================================================================================
                                $RCSfile: sdEnumDef.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.3 $
       $Date: 2007/06/20 03:11:52 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/sdEnumDef.h,v $

=============================================================================================================*/
/*--------------------------------------------------------------------------------------------------
                                  sdEnumdef.h                                                      
----------------------------------------------------------------------------------------------------

    Function:   Global enumerated variables
    
    Input:      none
  
    Return:     none
  
    Notes:      Placed in a single file to make visibility obvious
--------------------------------------------------------------------------------------------------*/
#ifndef __SDEnumDef_H
   #define __SDEnumDef_H

 /*   enum    {CMD0=0, CMD1, CMD9, CMD10, CMD12, CMD13, CMD16,
             CMD17, CMD18, CMD24, CMD25, CMD27, CMD28, CMD29,
             CMD30, CMD32, CMD33, CMD34, CMD35, CMD36, CMD37,
             CMD38, CMD42, CMD58, CMD59, ACMD41, CMD_TOT};
 */

    enum    {GO_IDLE_STATE=0,
             SEND_OP_COND,
             SEND_CSD,
             SEND_CID,
             STOP_TRANSMISSION,
             SEND_STATUS,
             SET_BLOCKLEN,
             READ_SINGLE_BLOCK,
             READ_MULTIPLE_BLOCK,
             WRITE_BLOCK,
             WRITE_MULTIPLE_BLOCK,
             PROGRAM_CSD, SET_WRITE_PROT,
             CLR_WRITE_PROT,
             SEND_WRITE_PROT,
             TAG_SECTOR_START,
             TAG_SECTOR_END,
             UNTAG_SECTOR,
             TAG_ERASE_GROUP_START,
             TAG_ERASE_GROUP_END,
             UNTAG_ERASE_GROUP,
             ERASE,
             LOCK_UNLOCK,
             READ_OCR,
             CRC_ON_OFF};
            
    enum    {NO_ERR=0, INIT_ERR, FILE_ERR, WRITE_ERR, READ_ERR, NAME_ERR, READONLY_ERR,
             SOF_ERR, EOF_ERR, ALLOC_ERR, POS_ERR, MODE_ERR, FAT_ERR, DISK_FULL, 
             PATH_ERR, NO_ENTRY_AVAL, EXIST_ERR};

    enum    {RESP_1, RESP_1b, RESP_2, RESP_3};

    enum    {NO_ARG, BLOCK_LEN, DATA_ADDR, STUFF_BITS};

    enum    {CLOSED = 0, READ, WRITE, APPEND};
    enum    {SCAN_DIR_R, SCAN_DIR_W, SCAN_FILE_R, SCAN_FILE_W};
    enum    {CHAIN_R, CHAIN_W, SINGLE, START_CHAIN, END_CHAIN};
    enum    {SD_SEEK_CUR, SD_SEEK_END, SD_SEEK_SET};
    
#endif
