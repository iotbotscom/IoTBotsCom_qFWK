#ifndef IOTBOTSCOM_LOGMAN_DEFS_H
#define IOTBOTSCOM_LOGMAN_DEFS_H
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


    Filename: iotbotscom_logman_defs.h

    General Description: qFWK Logging Manager

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
typedef enum _LOGGING_USR_T
{
    USR_LOG_START = 0,
    USR_LOG_ENABLE = USR_LOG_START,

    USR_LOG_SYSTEM,
    USR_LOG_DEVICE = USR_LOG_SYSTEM,    // 1
    USR_LOG_GSM,                        // 2
    USR_LOG_GNSS,                       // 3
    USR_LOG_WIFI,                       // 4
    USR_LOG_BT,                         // 5
    USR_LOG_GPIO,                       // 6
    USR_LOG_RS485,                      // 7
    USR_LOG_OW,                         // 8

    USR_LOG_CHIPS = 16,
    USR_LOG_MSENSOR = USR_LOG_CHIPS,    // 16
    USR_LOG_TSENSOR,                    // 17
    USR_LOG_LSENSOR,                    // 18
    USR_LOG_AIRQ,                       // 19
    USR_LOG_RTC,                        // 20
    USR_LOG_SFLASH,                     // 25

    USR_LOG_APPS = 26,
    USR_LOG_APP = USR_LOG_APPS,         // 26
    USR_LOG_FB,                         // 27
    USR_LOG_MQTT,                       // 28
    USR_LOG_BLNK,                       // 29

    USR_LOG_END = 31
}LOGGING_USR_T;

typedef enum _LOGGING_DBG_T
{
    DBG_LOG_START = 0,
    DBG_LOG_ENABLE = DBG_LOG_START,

    DBG_LOG_SYSTEM,
    DBG_LOG_DEVICE = DBG_LOG_SYSTEM,    // 1
    DBG_LOG_GSM,                        // 2
    DBG_LOG_GNSS,                       // 3
    DBG_LOG_WIFI,                       // 4
    DBG_LOG_BT,                         // 5
    DBG_LOG_GPIO,                       // 6
    DBG_LOG_I2C,                        // 7
    DBG_LOG_SPI,                        // 8
    DBG_LOG_EEPROM,                     // 9
    DBG_LOG_SCI,                        // 10
    DBG_LOG_RS485,                      // 11
    DBG_LOG_OW,                         // 12
    DBG_LOG_DATASERVER,                 // 13
    DBG_LOG_MDS,                        // 14
    DBG_LOG_PUBLISHER,                  // 15

    DBG_LOG_CHIPS = 16,
    DBG_LOG_MSENSOR = DBG_LOG_CHIPS,    // 16
    DBG_LOG_TSENSOR,                    // 17
    DBG_LOG_LSENSOR,                    // 18
    DBG_LOG_AIRQ,                       // 19
    DBG_LOG_RTC,                        // 20
    DBG_LOG_IOE,                        // 21
    DBG_LOG_PWM,                        // 22
    DBG_LOG_RGB,                        // 23
    DBG_LOG_QUARTA,                     // 24
    DBG_LOG_QUARTB,                     // 25

    DBG_LOG_APPS = 26,
    DBG_LOG_APP = DBG_LOG_APPS,         // 26
    DBG_LOG_FB,                         // 27
    DBG_LOG_MQTT,                       // 28
    DBG_LOG_BLNK,                       // 29

    DBG_LOG_END = 31
}LOGGING_DBG_T;

typedef struct _LOGGING_SETTINGS_T
{
    UINT32 logging_usr;
    UINT32 logging_dbg;
}LOGGING_SETTINGS_T;

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

