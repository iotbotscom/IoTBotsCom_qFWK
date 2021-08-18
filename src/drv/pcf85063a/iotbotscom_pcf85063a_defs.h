#ifndef IOTBOTSCOM_RTC_DEFS_H
#define IOTBOTSCOM_RTC_DEFS_H
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


    Filename: iotbotscom_pcf85063a_defs.h

    General Description: qFWK RTC Chip driver

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

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _RTC_STATUS_T
{
    UINT8 is_on             :1;
    UINT8 unused            :7;
}RTC_STATUS_T;

typedef struct _RTC_DATA_T
{
    RTC_STATUS_T status;
}RTC_DATA_T;

typedef struct _RTC_CONTROL_T
{
    UINT8 is_enabled        :1;
    UINT8 unused            :7;
}RTC_CONTROL_T;

typedef struct _RTC_SETTINGS_T
{
    RTC_CONTROL_T control;
}RTC_SETTINGS_T;

typedef struct _RTC_T
{
    RTC_DATA_T data;
    RTC_SETTINGS_T settings;
}RTC_T;

typedef struct _RTC_DEVICE_T
{
    MODE_T mode;
    TD_T td;
    RTC_SETTINGS_T settings;
}RTC_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/
extern RTC_DEVICE_T rtc_device;

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

