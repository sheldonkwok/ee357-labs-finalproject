#include "cpu.h"
//rwl #include "SCI.h"

#include "TypeDef.h"                            // typedefs

#include "EnumDef.h"                            // Enumeration statements, kept in one place for obvious visibility
#include "sdEnumDef.h"                            // Enumeration statements, kept in one place for obvious visibility

#include "sdCmd.h"

#include "sdFileSys.h"

#include "sdExtFileDeclar.h"

#include "sdFcnDef.h"

#include    <stdio.h>

int16           PutCharStio                                (char  put_char);
extern uint8    OutPutMode;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                                SCI Rtns CW
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*--------------------------------------------------------------------------------------------------
                                  putchar                                                      
----------------------------------------------------------------------------------------------------

    Function:   putchar - Put Character to output device set in globle variable OutPutMode
  
    Input:      char
  
    Return:     return_value, if file system, status, else char sent
  
    Notes:

--------------------------------------------------------------------------------------------------*/
void TERMIO_PutChar (char put_char)
{

//rwl     SCI_SendChar(put_char);

    return;
}

/*--------------------------------------------------------------------------------------------------
                                  Print_iMn                                                      
----------------------------------------------------------------------------------------------------

    Function:   XGate modules have deleted printf, therefore this is a substitute
  
    Input:      uint8 *print_char
  
    Return:     none
  
    Notes:

--------------------------------------------------------------------------------------------------*/
void Print_iMn (uint8 *print_char)
{
uint8 put_char, i;

    i = 0;
    while(print_char[i] != NULL_CHAR){
//rwl         put_char = print_char[i++];
//rwl         SCI_SendChar(put_char);
    }

   return;
}

/*--------------------------------------------------------------------------------------------------
                                  PutCharFile                                                      
----------------------------------------------------------------------------------------------------

    Function:   Put Character to File
  
    Input:      char
  
    Return:     return_value
  
    Notes:

--------------------------------------------------------------------------------------------------*/
int16 PutCharFile (int8 put_char)
{
unsigned int return_value;

//rwl   return_value = fputc_iMn(put_char, FilePtr1);   

  return return_value;
}

/*--------------------------------------------------------------------------------------------------
                                  FF_putchar                                                      
----------------------------------------------------------------------------------------------------

    Function:   putchar - Put Character to output device set in globle variable OutPutMode
  
    Input:      char
  
    Return:     return_value, if file system, status, else char sent
  
    Notes:

--------------------------------------------------------------------------------------------------*/
int16 _FF_putchar (char put_char)
{
    switch(OutPutMode){
        case ge_GRPH_LCD:
             break;
        case ge_STDIO:
//rwl             SCI_SendChar(put_char);
             break;
        case ge_FILE_PRT:
//rwl              PutCharFile(put_char);
             break;
    }

    return(put_char);
}

/*--------------------------------------------------------------------------------------------------
                                  _FF_getchar                                                      
----------------------------------------------------------------------------------------------------

    Function:   Get Character from SCI
  
    Input:      None
  
    Return:     int
  
    Notes:      int is used here rather then unsigned int because the getchar here must match the prototype in
                    the ImageCraft library
    int16 _FF_getchar(void);
--------------------------------------------------------------------------------------------------*/
int16 _FF_getchar (void)
{
uint8 recv_data = 0;

//rwl     SCI_RecvChar(&recv_data);                           // get input

    return((uint16)recv_data);                          // return input data
}

