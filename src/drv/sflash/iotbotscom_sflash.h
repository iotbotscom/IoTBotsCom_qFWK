#ifndef IOTBOTSCOM_SFLASH_H
#define IOTBOTSCOM_SFLASH_H
/*========================================================================================
Copyright (c) 2021 IOT-BOTS.COM LLC. All right reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USEOR OTHER
DEALINGS IN THE SOFTWARE.


    Module Name: iotbotscom_sflash.h

    General Description: qFWK Serial Flash driver

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* Component includes. */
#include "iotbotscom_sflash_defs.h"

/*---- Defines  ------------------------------------------------------------------------*/

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/
BOOL SFLASH_Init(void );
BOOL SFLASH_On(void );
BOOL SFLASH_Off(void );
BOOL SFLASH_Resume(void );
BOOL SFLASH_Suspend(void );
MODE_T SFLASH_Get_Mode(void );

BOOL SFLASH_Write(UINT32 p_sFlash_addr, UINT8 * p_RAM_addr, UINT16 count);
BOOL SFLASH_Read(UINT32 p_sFlash_addr, UINT8 * p_RAM_addr, UINT16 count);
BOOL SFLASH_Bulk_Erasure(void );

/*---- Function declarations------------------------------------------------------------*/

#endif

