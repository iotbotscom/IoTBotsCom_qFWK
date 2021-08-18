#ifndef IOTBOTSCOM_LOGMAN_H
#define IOTBOTSCOM_LOGMAN_H
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


    Filename: iotbotscom_logman.h

    General Description: qFWK Logging Manager

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* Component includes. */
#include "iotbotscom_logman_defs.h"

/*---- Defines  ------------------------------------------------------------------------*/

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/
BOOL Logman_Set_Settings(LOGGING_SETTINGS_T * p_config);
BOOL Logman_Get_Settings(LOGGING_SETTINGS_T * p_config);
BOOL Logman_Usr(LOGGING_USR_T mode, const char* log_buf);
BOOL Logman_Dbg(LOGGING_DBG_T mode, const char* log_buf);
BOOL Logman_CheckUSRMode(LOGGING_USR_T usr_log_mode);
BOOL Logman_CheckDBGMode(LOGGING_DBG_T dbg_log_mode);

BOOL Logman_SYS(const char* log_buf);
BOOL Logman_HEX(PBUF_T * p_buf, UINT32 mode);
BOOL Logman_Binary(PBUF_T * p_buf, UINT32 mode);

// Common
#define LOG_USR_CHECK(usr_log_mode)  (Logman_CheckUSRMode(usr_log_mode))
#define LOG_DBG_CHECK(dbg_log_mode)  (Logman_CheckDBGMode(dbg_log_mode))

// System Logging
#define LOG_SYS(log_str)        (Logman_SYS((const char*)log_str))

// User Logging
#define LOG_DEV(log_str)        (Logman_Usr(USR_LOG_DEVICE, (const char*)log_str))
#define LOG_GSM(log_str)        (Logman_Usr(USR_LOG_GSM, (const char*)log_str))
#define LOG_GNSS(log_str)       (Logman_Usr(USR_LOG_GNSS, (const char*)log_str))
#define LOG_WIFI(log_str)       (Logman_Usr(USR_LOG_WIFI, (const char*)log_str))
#define LOG_BT(log_str)         (Logman_Usr(USR_LOG_BT, (const char*)log_str))
#define LOG_GPIO(log_str)       (Logman_Usr(USR_LOG_GPIO, (const char*)log_str))
#define LOG_MSENSOR(log_str)    (Logman_Usr(USR_LOG_MSENSOR, (const char*)log_str))
#define LOG_TSENSOR(log_str)    (Logman_Usr(USR_LOG_TSENSOR, (const char*)log_str))
#define LOG_LSENSOR(log_str)    (Logman_Usr(USR_LOG_LSENSOR, (const char*)log_str))
#define LOG_AIRQ(log_str)       (Logman_Usr(USR_LOG_AIRQ, (const char*)log_str))
#define LOG_RTC(log_str)        (Logman_Usr(USR_LOG_RTC, (const char*)log_str))
#define LOG_FB(log_str)         (Logman_Usr(USR_LOG_FB, (const char*)log_str))
#define LOG_MQTT(log_str)       (Logman_Usr(USR_LOG_MQTT, (const char*)log_str))
#define LOG_BLNK(log_str)       (Logman_Usr(USR_LOG_BLNK, (const char*)log_str))
#define LOG_APP(log_str)        (Logman_Usr(USR_LOG_APP, (const char*)log_str))
#define LOG_RS485(log_str)      (Logman_Usr(USR_LOG_RS485, (const char*)log_str))
#define LOG_OW(log_str)         (Logman_Usr(USR_LOG_OW, (const char*)log_str))

// Debug Logging
#define DBG_DEV(log_str)        (Logman_Dbg(DBG_LOG_DEVICE, log_str))
#define DBG_GNSS(log_str)       (Logman_Dbg(DBG_LOG_GNSS, log_str))
#define DBG_GSM(log_str)        (Logman_Dbg(DBG_LOG_GSM, log_str))
#define DBG_WIFI(log_str)       (Logman_Dbg(DBG_LOG_WIFI, log_str))
#define DBG_BT(log_str)         (Logman_Dbg(DBG_LOG_BT, log_str))
#define DBG_GPIO(log_str)       (Logman_Dbg(DBG_LOG_GPIO, log_str))
#define DBG_MSENSOR(log_str)    (Logman_Dbg(DBG_LOG_MSENSOR, log_str))
#define DBG_TSENSOR(log_str)    (Logman_Dbg(DBG_LOG_TSENSOR, log_str))
#define DBG_LSENSOR(log_str)    (Logman_Dbg(DBG_LOG_LSENSOR, log_str))
#define DBG_AIRQ(log_str)       (Logman_Dbg(DBG_LOG_AIRQ, log_str))
#define DBG_I2C(log_str)        (Logman_Dbg(DBG_LOG_I2C, log_str))
#define DBG_SPI(log_str)        (Logman_Dbg(DBG_LOG_SPI, log_str))
#define DBG_RTC(log_str)        (Logman_Dbg(DBG_LOG_RTC, log_str))
#define DBG_IOE(log_str)        (Logman_Dbg(DBG_LOG_IOE, log_str))
#define DBG_PWM(log_str)        (Logman_Dbg(DBG_LOG_PWM, log_str))
#define DBG_RGB(log_str)        (Logman_Dbg(DBG_LOG_RGB, log_str))
#define DBG_FB(log_str)         (Logman_Dbg(DBG_LOG_FB, log_str))
#define DBG_MQTT(log_str)       (Logman_Dbg(DBG_LOG_MQTT, log_str))
#define DBG_DATASERVER(log_str) (Logman_Dbg(DBG_LOG_DATASERVER, log_str))
#define DBG_MDS(log_str)        (Logman_Dbg(DBG_LOG_MDS, log_str))
#define DBG_PUBLISHER(log_str)  (Logman_Dbg(DBG_LOG_PUBLISHER, log_str))
#define DBG_SCI(log_str)        (Logman_Dbg(DBG_LOG_SCI, log_str))
#define DBG_RS485(log_str)      (Logman_Dbg(DBG_LOG_RS485, log_str))
#define DBG_OW(log_str)         (Logman_Dbg(DBG_LOG_OW, log_str))
#define DBG_EEPROM(log_str)     (Logman_Dbg(DBG_LOG_EEPROM, log_str))
#define DBG_BLNK(log_str)       (Logman_Dbg(DBG_LOG_BLNK, log_str))
#define DBG_APP(log_str)        (Logman_Dbg(DBG_LOG_APP, log_str))
#define DBG_QUARTA(log_str)     (Logman_Dbg(DBG_LOG_QUARTA, log_str))
#define DBG_QUARTB(log_str)     (Logman_Dbg(DBG_LOG_QUARTB, log_str))

/*---- Function declarations------------------------------------------------------------*/

#endif

