#ifndef IOTBOTSCOM_MDS_DEFS_H
#define IOTBOTSCOM_MDS_DEFS_H
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


    Filename: iotbotscom_mds_defs.h

    General Description: qFWK Message Dispatch Service Module

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
#define MDS_DATA_FIELDS_MAX             8
#define MDS_COMMAND_FIELD_LEN_MAX       14
#define MDS_DATA_FIELD_LEN_MAX          68

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _MDS_COMMAND_FIELD_T
{
    char buf[MDS_COMMAND_FIELD_LEN_MAX];
}MDS_COMMAND_FIELD_T;

typedef struct _MDS_DATA_FIELD_T
{
    char buf[MDS_DATA_FIELD_LEN_MAX];
}MDS_DATA_FIELD_T;

typedef struct _MDS_DEVICE_T
{
    MDS_COMMAND_FIELD_T command;
    MDS_DATA_FIELD_T data[MDS_DATA_FIELDS_MAX];
}MDS_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/
extern MDS_DEVICE_T mds_device;

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

