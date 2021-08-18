#ifndef IOTBOTSCOM_LIS3DH_DEFS_H
#define IOTBOTSCOM_LIS3DH_DEFS_H
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


    Filename: iotbotscom_lis3dh_defs.h

    General Description: qFWK LIS3DH Motion Sensor driver

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
typedef struct _LIS3DH_INT_SOURCE_T
{
    UINT8 active    :1;
    UINT8 motion    :1;
    UINT8 shoke     :1;
    UINT16 unused   :5;
}LIS3DH_INT_SOURCE_T;

typedef struct _LIS3DH_SAMPLE_T
{
    SINT16 x;
    SINT16 y;
    SINT16 z;
}LIS3DH_SAMPLE_T;

typedef struct _LIS3DH_STATUS_T
{
    UINT8 is_on             :1;
    UINT8 unused            :7;
}LIS3DH_STATUS_T;

typedef struct _LIS3DH_DATA_T
{
    LIS3DH_STATUS_T status;
    LIS3DH_SAMPLE_T data;
    LIS3DH_INT_SOURCE_T int_source;
}LIS3DH_DATA_T;

typedef struct _LIS3DH_CONTROL_T
{
    UINT8 is_enabled            :1;
    UINT8 motion_treshould      :1;
    UINT8 shoke_treshould       :1;
    UINT8 int_enabled           :1;
    UINT8 sleep_int_enabled     :1;
    UINT8 x_axis_direction      :1;
    UINT8 y_axis_direction      :1;
    UINT8 z_axis_direction      :1;
}LIS3DH_CONTROL_T;

typedef struct _LIS3DH_SETTINGS_T
{
    LIS3DH_CONTROL_T control;
    UINT8 motion_treshould;
    UINT8 shoke_treshould;
}LIS3DH_SETTINGS_T;

typedef struct _LIS3DH_T
{
    LIS3DH_DATA_T data;
    LIS3DH_SETTINGS_T settings;
}LIS3DH_T;

typedef struct _LIS3DH_DEVICE_T
{
    MODE_T mode;
    LIS3DH_SETTINGS_T settings;
    LIS3DH_SAMPLE_T data;
}LIS3DH_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/
extern LIS3DH_DEVICE_T lis3dh_device;

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

