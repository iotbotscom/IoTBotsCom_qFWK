#ifndef IOTBOTSCOM_LM75_DEFS_H
#define IOTBOTSCOM_LM75_DEFS_H
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


    Filename: iotbotscom_lm75_defs.h

    General Description: qFWK LM75 Temperature Sensor driver

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
#define TEMPERATURE_HYSTERESIS          (5)
#define TEMPERATURE_HIGH_ALARM_LEVEL    (55)
#define TEMPERATURE_LOW_ALARM_LEVEL     (-25)

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _LM75_STATUS_T
{
    UINT8 is_on             :1;
    UINT8 unused            :7;
}LM75_STATUS_T;

typedef struct _LM75_SAMPLE_T
{
    SINT16 temperature;
}LM75_SAMPLE_T;

typedef struct _LM75_DATA_T
{
    LM75_STATUS_T status;
    LM75_SAMPLE_T sample;
}LM75_DATA_T;

typedef struct _LM75_CONTROL_T
{
    UINT8 is_enabled        :1;
    UINT8 is_alarm_enabled  :1;
    UINT8 unused            :7;
}LM75_CONTROL_T;

typedef struct _LM75_SETTINGS_T
{
    LM75_CONTROL_T control;
    SINT16 temperature_low_alarm_level;
    SINT16 temperature_high_alarm_level;
    UINT16 hyst;
    UINT16 os;
}LM75_SETTINGS_T;

typedef struct _LM75_T
{
    LM75_DATA_T data;
    LM75_SETTINGS_T settings;
}LM75_T;

typedef struct _LM75_DEVICE_T
{
    MODE_T mode;
    SINT16 temperature;
    LM75_SETTINGS_T settings;
}LM75_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/
extern LM75_DEVICE_T lm75_device;

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

