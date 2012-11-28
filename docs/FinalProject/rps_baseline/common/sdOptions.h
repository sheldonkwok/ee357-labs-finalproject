/*============================================================================================================
                                $RCSfile: sdOptions.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.3 $
       $Date: 2007/06/20 03:11:53 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/sdOptions.h,v $

=============================================================================================================*/

/*--------------------------------------------------------------------------------------------------
                                  Options.h                                                      
----------------------------------------------------------------------------------------------------

    Function:   Compile options definitions
    
    Input:      none
  
    Return:     none
  
    Notes:      
--------------------------------------------------------------------------------------------------*/
#ifndef __Options_H
   #define _RTC_ON_
   #define __Options_H

   #define _SECOND_FAT_ON_
   #define _FAT12_ON_
// #define _READ_ONLY_
//   #define _DEBUG_ON_ 1
   #define _DEBUG_ON_DIRECTORY 1

   #ifndef _READ_ONLY_
       #include <stdio.h>
   #endif
   
   #define _ICCAVR_                         // ignore, left in for backwards diff checks with prev code

   #define _TEXT_MODE 0                     // printf requirement, define as 1 if CR + LF to be op for CR
   
#endif
