/*============================================================================================================
                                $RCSfile: TypeDef.h,v $

       $Author: Copyright iMn MicroControl Ltd. - Robert Lewis$
       $Revision: 1.1 $
       $Date: 2007/04/10 19:44:24 $
       $Source: G:/cvsrepo-may-30-2005/FreescaleDemo/Common/TypeDef.h,v $

=============================================================================================================*/
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( TypeDefines.h ))))))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
    Function:       Definition of Global constants

    Input:          None

    Return:         None

    Notes:          Advisory: do not define local variables here
----------------------------------------------------------------------------------------------------*/
#ifndef DEF_TYPEDEFINES_H
    #define DEF_TYPEDEFINES_H

    typedef unsigned char       uint8;
    typedef signed char         sint8;
    typedef unsigned short      uint16;
    typedef signed int          sint16;
    typedef unsigned long int   uint32;
    typedef long int            sint32;
    typedef unsigned char       bool_t;

    typedef char                int8;                   // unsigned in CW by default
    typedef int                 int16;                  // unsigned
    typedef long                int32;                  // unsigned

    typedef uint8               enum_8;                 // enum as a type, always defaults to smallest data size
    typedef uint16              enum_16;
#endif

#define bit(x)  (1 << (x))
#define _flag(x)  (1 << (x))

