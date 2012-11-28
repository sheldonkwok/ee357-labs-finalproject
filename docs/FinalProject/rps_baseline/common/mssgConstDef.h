#ifndef CAN_CONST_DEF_H
    #define CAN_CONST_DEF_H
/*============================================================================================================
                                $RCSfile: ConstDef.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.16 $
       $Date: 2007/06/15 18:38:30 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/ConstDef.h,v $

=============================================================================================================*/
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( ConstDef ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Definition of constants

    Input:          None

    Return:         None

    Notes:          Advisory: do not define local variables here
----------------------------------------------------------------------------------------------------*/
// TRUE and FALSE defined in CodeWarrior defines

#define MSSG_STATUS_MASK                  0xF0          // 1111,0000     
#define MSSG_STATUS                       0x0F          // 0000,1111     
#define MSSG_BUFF_SIZE                    0x48          // size of     zigbee packet
#define LINE_STRING_SIZE                   140          // max reserved size of text string

#endif             
