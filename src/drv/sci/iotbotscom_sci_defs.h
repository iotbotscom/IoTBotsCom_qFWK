#ifndef IOTBOTSCOM_SCI_DEFS_H
#define IOTBOTSCOM_SCI_DEFS_H
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


    Filename: iotbotscom_sci_defs.h

    General Description: qFWK SCI driver

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
#define SCI_RX_LEN 128
#define SCI_TX_LEN 128

/*---- Typedefs ------------------------------------------------------------------------*/
typedef BOOL (* SPC_T)(PBUF_T * p_rx, PBUF_T * p_tx);

typedef struct _SCI_STATUS_T
{
    UINT8 is_on             :1;
    UINT8 is_active         :1;
    UINT8 unused            :6;
}SCI_STATUS_T;

typedef struct _SCI_DATA_T
{
    SCI_STATUS_T status;
}SCI_DATA_T;

typedef struct _SCI_CONTROL_T
{
    UINT8 is_enabled        :1;
    UINT8 unused            :7;
}SCI_CONTROL_T;

typedef struct _SCI_SETTINGS_T
{
    SCI_CONTROL_T control;
}SCI_SETTINGS_T;

typedef struct _SCI_T
{
    SCI_DATA_T data;
    SCI_SETTINGS_T settings;
}SCI_T;

typedef struct _SCI_DEVICE_T
{
    SCI_STATUS_T status;
    SCI_CONTROL_T control;
    MODE_T mode;

    SPC_T processor_call;

    PBUF_T tx;
    UINT8 buf_tx[SCI_TX_LEN];
    PBUF_T rx;
    UINT8 buf_rx[SCI_RX_LEN];
}SCI_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

