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


    Filename: iotbotscom_logman.cpp

    General Description: qFWK Logging Manager

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* HW platform includes. */
#include "../../ghdr/iotbotscom_hw_config.h"

/* Component includes. */
#include "iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define LOG_BUF_SIZE_MAX 160

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
static LOGGING_SETTINGS_T LoggingMode;
static char log_buf[LOG_BUF_SIZE_MAX];

/*---- Function prototypes -------------------------------------------------------------*/
void logman_hex_to_ascii(PBUF_T * p_buf, BOOL stop);
char logman_get_upper(UINT8 byte);
char logman_get_lower(UINT8 byte);
char logman_get_ascii(UINT8 data);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL Logman_Set_Settings(LOGGING_SETTINGS_T * p_config)
{
    if(p_config == NULL)
    {
        return FALSE;
    }

    LoggingMode = *p_config;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_Get_Settings(LOGGING_SETTINGS_T * p_config)
{
    if(p_config == NULL)
    {
        return FALSE;
    }

    *p_config = LoggingMode;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_CheckUSRMode(LOGGING_USR_T usr_log_mode)
{
    if((LoggingMode.logging_usr & (1 << (UINT32)USR_LOG_ENABLE)) && (LoggingMode.logging_usr & (1 << (UINT32)usr_log_mode)))
    {
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_Usr(LOGGING_USR_T mode, const char* log_buf)
{
    if((LoggingMode.logging_usr & (1 << (UINT32)USR_LOG_ENABLE)) && (LoggingMode.logging_usr & (1 << (UINT32)mode)))
    {
        return Serial.print(log_buf);
    }
    return FALSE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_CheckDBGMode(LOGGING_DBG_T dbg_log_mode)
{
    if((LoggingMode.logging_dbg & (1 << (UINT32)DBG_LOG_ENABLE)) && (LoggingMode.logging_dbg & (1 << (UINT32)dbg_log_mode)))
    {
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_Dbg(LOGGING_DBG_T mode, const char* log_buf)
{
    if((LoggingMode.logging_dbg & (1 << (UINT32)DBG_LOG_ENABLE)) && (LoggingMode.logging_dbg & (1 << (UINT32)mode)))
    {
        switch(mode)
        {
            case(DBG_LOG_DEVICE):
                Serial.print("\r\nFC: ");
                break;

            case(DBG_LOG_GSM):
                Serial.print("\r\nGSM: ");
                break;

            case(DBG_LOG_GNSS):
                Serial.print("\r\nGNSS: ");
                break;

            case(DBG_LOG_WIFI):
                Serial.print("\r\nWIFI: ");
                break;

            case(DBG_LOG_BT):
                Serial.print("\r\nBT: ");
                break;

            case(DBG_LOG_GPIO):
                Serial.print("\r\nGPIO: ");
                break;

            case(DBG_LOG_MSENSOR):
                Serial.print("\r\nMSENSOR: ");
                break;

            case(DBG_LOG_TSENSOR):
                Serial.print("\r\nTSENSOR: ");
                break;

            case(DBG_LOG_LSENSOR):
                Serial.print("\r\nLSENSOR: ");
                break;

            case(DBG_LOG_I2C):
                Serial.print("\r\nI2C: ");
                break;

            case(DBG_LOG_SPI):
                Serial.print("\r\nSPI: ");
                break;

            case(DBG_LOG_RTC):
                Serial.print("\r\nRTC: ");
                break;

            case(DBG_LOG_IOE):
                Serial.print("\r\nIOE: ");
                break;

            case(DBG_LOG_PWM):
                Serial.print("\r\nPWM: ");
                break;

            case(DBG_LOG_RGB):
                Serial.print("\r\nRGB: ");
                break;

            case(DBG_LOG_FB):
                Serial.print("\r\nFB: ");
                break;

            case(DBG_LOG_MQTT):
                Serial.print("\r\nMQTT: ");
                break;

            case(DBG_LOG_DATASERVER):
                Serial.print("\r\nDATASERVER: ");
                break;

            case(DBG_LOG_MDS):
                Serial.print("\r\nMDS: ");
                break;

            case(DBG_LOG_PUBLISHER):
                Serial.print("\r\nPUBLISHER: ");
                break;

            case(DBG_LOG_SCI):
                Serial.print("\r\nSCI: ");
                break;

            case(DBG_LOG_RS485):
                Serial.print("\r\nRS485: ");
                break;

            case(DBG_LOG_OW):
                Serial.print("\r\nOW: ");
                break;

            case(DBG_LOG_BLNK):
                Serial.print("\r\nBLNK: ");
                break;

            case(DBG_LOG_APP):
                Serial.print("\r\nAPP: ");
                break;

            case(DBG_LOG_QUARTA):
                Serial.print("\r\nQUARTA: ");
                break;

            case(DBG_LOG_QUARTB):
                Serial.print("\r\nQUARTB: ");
                break;

            default:
                Serial.print("???: ");
                break;
        }

        Serial.print(log_buf);

        Serial.print("\r\n");

        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_HEX(LOGGING_USR_T logging_mode, PBUF_T * p_buf)
{
    UINT8 mode = (UINT8)logging_mode;
    BOOL stop = FALSE;
    PBUF_T pbuf;
    UINT16 len;

    if(!((LoggingMode.logging_usr & (1 << (UINT32)USR_LOG_ENABLE)) && (LoggingMode.logging_usr & (1 << (UINT32)mode))))
    {
        return FALSE;
    }

    if(p_buf == NULL || p_buf->len == 0)
    {
        return FALSE;
    }

    pbuf = *p_buf;
    if(pbuf.len > 64)
    {
        stop = FALSE;

        pbuf.len = 64;
        logman_hex_to_ascii(&pbuf, stop);

        len = strlen((char *)log_buf);
        log_buf[len++] = '.';
        log_buf[len++] = '.';
        log_buf[len++] = '.';
        log_buf[len++] = '\r';
        log_buf[len++] = '\n';
        log_buf[len++] = 0;
    }
    else
    {
        stop = TRUE;

        logman_hex_to_ascii(&pbuf, stop);
    }

    Serial.println(log_buf);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_Binary(LOGGING_USR_T logging_mode, PBUF_T * p_buf)
{
    UINT8 mode = (UINT8)logging_mode;
    BOOL stop = FALSE;
    PBUF_T pbuf;
    UINT16 idx = 0;
    UINT16 len;

    if(!((LoggingMode.logging_usr & (1 << (UINT32)USR_LOG_ENABLE)) && (LoggingMode.logging_usr & (1 << (UINT32)mode))))
    {
        return FALSE;
    }

    if(p_buf == NULL || p_buf->len == 0)
    {
        return FALSE;
    }

    pbuf = *p_buf;
    len = pbuf.len;

    do
    {
        if(len > 32)
        {
            pbuf.buf += idx;
            pbuf.len = 32;
            idx = 32;
            len -= 32;
        }
        else
        {
            pbuf.buf += idx;
            pbuf.len = len;

            stop = TRUE;
        }

        logman_hex_to_ascii(&pbuf, stop);

        Serial.println(log_buf);

        delay(100);
    }
    while(stop != TRUE);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL Logman_SYS(const char* logging_buf)
{
    Serial.println(logging_buf);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void logman_hex_to_ascii(PBUF_T * p_buf, BOOL stop)
{
    UINT16 idx = 0;
    UINT16 len = 0;

    while(idx < p_buf->len)
    {
        log_buf[len++] = logman_get_upper(p_buf->buf[idx]);
        log_buf[len++] = logman_get_lower(p_buf->buf[idx]);
        log_buf[len++] = ',';
        idx++;

        if(len >= LOG_BUF_SIZE_MAX - 3)
        {
            len = LOG_BUF_SIZE_MAX - 4;
        }
    }

    if(stop == TRUE)
    {
        len--;
        log_buf[len++] = '\r';
        log_buf[len++] = '\n';
        log_buf[len++] = 0;
    }
    else
    {
        log_buf[len++] = 0;
    }
}

/*--------------------------------------------------------------------------------------*/
char logman_get_upper(UINT8 byte)
{
    char data;

    data = ((byte >> 4) & 0x0f);
    data = logman_get_ascii(data);

    return data;
}

/*--------------------------------------------------------------------------------------*/
char logman_get_lower(UINT8 byte)
{
    char data;

    data = (byte & 0x0f);
    data = logman_get_ascii(data);

    return data;
}

/*--------------------------------------------------------------------------------------*/
char logman_get_ascii(UINT8 data)
{
    if(data <= 9)
    {
        data = '0' + data;
    }
    else if(data >= 0x0a && data <= 0x0f)
    {
        data = 'a' + data - 0x0a;
    }

    return data;
}

