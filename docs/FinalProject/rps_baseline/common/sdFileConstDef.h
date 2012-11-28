#ifndef DEF_CONSTDEF2_H
    #define DEF_CONSTDEF2_H
/*============================================================================================================
                                $RCSfile: ConstDef.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.16 $
       $Date: 2007/06/15 18:38:30 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/ConstDef.h,v $

=============================================================================================================*/
//============================================================================================================
//--------------------------------------------------------------------------------------------------
//(((((((((((((((((((((((((((((((((( sd Def's )))))))))))))))))))))))))))))))))))))))))))))))
//----------------------------------------------------------------------------------------------------
#define _SECOND_FAT_ON_
#define _FAT12_ON_
//#define _READ_ONLY_
//#define _DEBUG_ON_
#define _DIRECTORIES_SUPPORTED_
#define _NO_MALLOC_
#define _BYTES_PER_SEC_512_

// The settings below should be modified
// to match your hardware/software settings

//#define _LITTLE_ENDIAN_
#define _BIG_ENDIAN_

#ifdef _NO_MALLOC_
    #define _FF_MAX_FILES_OPEN    2
#endif

#ifdef _DEBUG_ON_
    //#define _DEBUG_FUNCTIONS_
#endif

//#define RX_COUNTER1 (SCISR1 & SCI_SR1_RDRF)                // tests if char received
#ifndef _READ_ONLY_
    #define _SD_BLOCK_WRITE_
#endif

#define _FF_strcpyf                                         strcpy
#define _FF_sprintf                                         sprintf
#define _FF_strrchr                                         strrchr
#define _FF_strncmp                                         strncmp
#define _FF_strlen                                          strlen

#endif             
