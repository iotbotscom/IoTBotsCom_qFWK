#ifndef IOTBOTSCOM_RS485_H
#define IOTBOTSCOM_RS485_H
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


    Filename: iotbotscom_rs485.h

    General Description: qFWK RS485 Serial driver

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* Component includes. */
#include "iotbotscom_rs485_defs.h"

/*---- Defines  ------------------------------------------------------------------------*/

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/
BOOL RS485_Init(void );
BOOL RS485_On(void );
BOOL RS485_Off(void );
BOOL RS485_Resume(void );
BOOL RS485_Suspend(void );
MODE_T RS485_Get_Mode(void );
BOOL RS485_Processor(void );
BOOL RS485_Is_Active(void );
BOOL RS485_Send(PBUF_T * p_tx);
BOOL RS485_Recv(PBUF_T * p_rx, UINT16 timeout);
BOOL RS485_Trsf(PBUF_T * p_tx, PBUF_T * p_rx, UINT16 timeout);

/*---- Function declarations------------------------------------------------------------*/

#endif

