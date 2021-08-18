#ifndef IOTBOTSCOM_RS485_DEFS_H
#define IOTBOTSCOM_RS485_DEFS_H
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


    Filename: iotbotscom_rs485_defs.h

    General Description: qFWK RS485 Serial driver

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* Data types includes. */
#include "../../ghdr/iotbotscom_basic_types.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define RS485_RX_LEN 128
#define RS485_TX_LEN 128

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _RS485_STATUS_T
{
    UINT8 is_on             :1;
    UINT8 is_active         :1;
    UINT8 unused            :6;
}RS485_STATUS_T;

typedef struct _RS485_DATA_T
{
    RS485_STATUS_T status;
}RS485_DATA_T;

typedef struct _RS485_CONTROL_T
{
    UINT8 is_enabled        :1;
    UINT8 unused            :7;
}RS485_CONTROL_T;

typedef struct _RS485_SETTINGS_T
{
    RS485_CONTROL_T control;
}RS485_SETTINGS_T;

typedef struct _RS485_T
{
    RS485_DATA_T data;
    RS485_SETTINGS_T settings;
}RS485_T;

typedef struct _RS485_DEVICE_T
{
    RS485_STATUS_T status;
    RS485_CONTROL_T control;
    MODE_T mode;

    PBUF_T tx;
    UINT8 buf_tx[RS485_TX_LEN];
    PBUF_T rx;
    UINT8 buf_rx[RS485_RX_LEN];
}RS485_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

