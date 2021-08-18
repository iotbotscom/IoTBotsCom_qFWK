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


    Filename: iotbotscom_mds.cpp

    General Description: qFWK Message Dispatch Service Module

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* Component includes. */
#include "iotbotscom_mds.h"

/* Other components includes. */
#include "../../ghdr/iotbotscom_versions.h"

/* SDK includes */

/* HW platform includes. */
#include "../../ghdr/iotbotscom_hw_config.h"

/* Logging */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define FIELD_DELIMITER_SYMBOL      ';'      //FIELD_DELIMITER_SYMBOL_WIM;
#define SUBFIELD_DELIMITER_SYMBOL   ','      //SUBFIELD_DELIMITER_SYMBOL_WIM;

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
MDS_DEVICE_T mds_device;

/*---- Function prototypes -------------------------------------------------------------*/
BOOL mds(PBUF_T * p_rx, PBUF_T * p_tx);
BOOL mds_getfield(char *pData, char *pField, int nFieldNum, int nMaxFieldLen);
UINT8 get_hex_from_string(char * buf);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL MDS(PBUF_T * p_rx, PBUF_T * p_tx)
{
    if(p_rx == NULL || p_tx == NULL || p_rx->len == 0)
    {
        return FALSE;
    }

    return mds(p_rx, p_tx);
}

/*--------------------------------------------------------------------------------------*/
BOOL mds(PBUF_T * p_rx, PBUF_T * p_tx)
{
    BOOL ret = FALSE;
    UINT8 * data_ptr = NULL;
    UINT8 command_len = 0;
    UINT8 data_len = 0;
    BOOL write_msg = FALSE;
    UINT8 i = 0;
    LOGGING_SETTINGS_T logging_mode;
    SINT8 s8;
    //char buf[12];
    //UINT8 u8;
    //UINT16 u16;
    //UINT32 u32;
    //char * ptr;

    if(LOG_DBG_CHECK(DBG_LOG_MDS))
    {
        Serial.print("MDS : Input: ");
        Serial.write(p_rx->buf, p_rx->len);
        Serial.print("");
    }

    for(i = 0; i < p_rx->len; i++)
    {
        if(p_rx->buf[i] == FIELD_DELIMITER_SYMBOL)
        {
            if(write_msg == FALSE)
            {
                write_msg = TRUE;
                mds_device.command.buf[i] = 0;
                command_len = strlen((char *)mds_device.command.buf);

                if(LOG_DBG_CHECK(DBG_LOG_MDS))
                {
                    Serial.print("MDS : Command: ");
                    Serial.write((UINT8 *)mds_device.command.buf, (UINT16)command_len);
                    Serial.println("");
                }
            }
            else
            {
                if(data_len == 0)
                {
                    data_ptr = &p_rx->buf[i];
                }
                data_len++;
            }
        }
        else if(p_rx->buf[i] == '\r' || p_rx->buf[i] == '\n')
        {
            p_rx->buf[i] = 0;

            if(write_msg == FALSE)
            {
                mds_device.command.buf[i] = 0;
                command_len = strlen((char *)mds_device.command.buf);

                if(LOG_DBG_CHECK(DBG_LOG_MDS))
                {
                    Serial.print("MDS : Command: ");
                    Serial.write((UINT8 *)mds_device.command.buf, (UINT16)command_len);
                    Serial.println("");
                }
            }
            break;
        }
        else
        {
            if(write_msg == FALSE)
            {
                mds_device.command.buf[i] = p_rx->buf[i];
            }
            else
            {
                if(data_len == 0)
                {
                    data_ptr = &p_rx->buf[i];
                }
                data_len++;
            }
        }
    }

    if(data_ptr != NULL && data_len > 0)
    {
        for(i = 0; i < MDS_DATA_FIELDS_MAX; i++)
        {
            if(mds_getfield((char *)data_ptr, mds_device.data[i].buf, i, MDS_DATA_FIELD_LEN_MAX))
            {
                mds_device.data[i].buf[MDS_DATA_FIELD_LEN_MAX - 1] = 0x00;

                if(LOG_DBG_CHECK(DBG_LOG_MDS))
                {
                    Serial.print("MDS : Data: ");
                    Serial.println((char *)mds_device.data[i].buf);
                }
            }
            else
            {
                mds_device.data[i].buf[0] = 0x00;

                if(LOG_DBG_CHECK(DBG_LOG_MDS))
                {
                    Serial.println("MDS : No Data");
                }
            }
        }
    }

    // Reply
    p_tx->len = 0;
    strcpy((char *)&p_tx->buf[p_tx->len], (char *)mds_device.command.buf);
    p_tx->len = strlen((char *)p_tx->buf);
    p_tx->buf[p_tx->len++] = FIELD_DELIMITER_SYMBOL;

    if(command_len == 0 || (write_msg == TRUE && data_len == 0))
    {
        p_tx->buf[p_tx->len++] = 'E';
        p_tx->buf[p_tx->len++] = 'R';
        p_tx->buf[p_tx->len++] = 'R';
        p_tx->buf[p_tx->len++] = 'O';
        p_tx->buf[p_tx->len++] = 'R';
        p_tx->buf[p_tx->len++] = '\r';
        p_tx->buf[p_tx->len++] = '\n';
        p_tx->buf[p_tx->len] = 0;

        return TRUE;
    }

    if(strcmp((char *)mds_device.command.buf, "VER") == 0)
    {
        strcpy((char *)&p_tx->buf[p_tx->len], HW_PRODUCT_TYPE);
        p_tx->len = strlen((char *)p_tx->buf);
        p_tx->buf[p_tx->len++] = FIELD_DELIMITER_SYMBOL;
        strcpy((char *)&p_tx->buf[p_tx->len], FLEX_VERSION);
        strcpy((char *)&p_tx->buf[p_tx->len], FW_VERSION);
        p_tx->len = strlen((char *)p_tx->buf);
        p_tx->buf[p_tx->len++] = FIELD_DELIMITER_SYMBOL;
        strcpy((char *)&p_tx->buf[p_tx->len], FLEX_VERSION);
        p_tx->len = strlen((char *)p_tx->buf);
        p_tx->buf[p_tx->len++] = FIELD_DELIMITER_SYMBOL;
        strcpy((char *)&p_tx->buf[p_tx->len], MODIFICATION_DATE);
        p_tx->len = strlen((char *)p_tx->buf);
        p_tx->buf[p_tx->len] = 0;
        ret = TRUE;
    }
    else if(strcmp((char *)mds_device.command.buf, "LOG") == 0)
    {
        Logman_Get_Settings(&logging_mode);

        if(write_msg == FALSE)
        {
            sprintf((char *)&p_tx->buf[p_tx->len], "%04X", (unsigned int)(logging_mode.logging_usr >> 1));
            p_tx->len = strlen((char *)p_tx->buf);
            p_tx->buf[p_tx->len++] = FIELD_DELIMITER_SYMBOL;
            sprintf((char *)&p_tx->buf[p_tx->len], "%04X", (unsigned int)(logging_mode.logging_dbg >> 1));
            p_tx->len = strlen((char *)p_tx->buf);
            p_tx->buf[p_tx->len] = 0;
            ret = TRUE;
        }
        else
        {
            if(mds_device.data[0].buf[0] != 0x00)
            {
                if(strcmp(mds_device.data[0].buf, "RESET") == 0)
                {
                    logging_mode.logging_usr = 0;
                    logging_mode.logging_dbg = 0;

                    ret = TRUE;
                }
                else
                {
                    s8 = atoi((char *)mds_device.data[0].buf);
                    if(s8 == 0)
                    {
                        if(mds_device.data[0].buf[0] == '-')
                        {
                            logging_mode.logging_usr = 0;
                        }
                        else
                        {
                            logging_mode.logging_usr &= (~(1 << USR_LOG_ENABLE));
                        }
                        ret = TRUE;
                    }
                    else if(s8 > 0 && s8 <= (UINT8)USR_LOG_END)
                    {
                        logging_mode.logging_usr |= ((1 << s8) | (1 << 0));
                        ret = TRUE;
                    }
                    else if(s8 < 0)
                    {
                        s8 = 0 - s8;
                        if(s8 > 0 && s8 <= (UINT8)USR_LOG_END)
                        {
                            logging_mode.logging_usr &= (~(1 << s8));
                            ret = TRUE;
                        }
                    }
                }
            }
        
            if(mds_device.data[1].buf[0] != 0x00)
            {
                s8 = atoi((char *)mds_device.data[1].buf);
                if(s8 == 0)
                {
                    if(mds_device.data[1].buf[0] == '-')
                    {
                        logging_mode.logging_dbg = 0;
                    }
                    else
                    {
                        logging_mode.logging_dbg &= (~(1 << DBG_LOG_ENABLE));
                    }
                    ret = TRUE;
                }
                else if(s8 > 0 && s8 <= (UINT8)DBG_LOG_END)
                {
                    logging_mode.logging_dbg |= ((1 << s8) | (1 << 0));
                    ret = TRUE;
                }
                else if(s8 < 0)
                {
                    s8 = 0 - s8;
                    if(s8 > 0 && s8 <= (UINT8)DBG_LOG_END)
                    {
                        logging_mode.logging_dbg &= (~(1 << s8));
                        ret = TRUE;
                    }
                }
            }

            if(ret == TRUE)
            {
                ret = Logman_Set_Settings(&logging_mode);
            }
        }
    }

    if(ret == TRUE)
    {
        if(write_msg == TRUE)
        {
            p_tx->buf[p_tx->len++] = 'O';
            p_tx->buf[p_tx->len++] = 'K';
        }
    }
    else
    {
        p_tx->buf[p_tx->len++] = 'E';
        p_tx->buf[p_tx->len++] = 'R';
        p_tx->buf[p_tx->len++] = 'R';
        p_tx->buf[p_tx->len++] = 'O';
        p_tx->buf[p_tx->len++] = 'R';
    }

    p_tx->buf[p_tx->len++] = '\r';
    p_tx->buf[p_tx->len++] = '\n';
    p_tx->buf[p_tx->len] = 0;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL mds_getfield(char *pData, char *pField, int nFieldNum, int nMaxFieldLen)
{
    UINT8 i = 0;
    UINT8 j = 0;
    UINT8 nField = 0;

    if(pData == NULL || pField == NULL || nMaxFieldLen <= 0 || pData[i] == '\0')
    {
        return FALSE;
    }

    while(nField != nFieldNum && pData[i])
    {
        if(pData[i] == FIELD_DELIMITER_SYMBOL)
        {
            nField++;
        }
        i++;

        if(pData[i] == NULL)
        {
            pField[0] = '\0';
            return FALSE;
        }
    }

    if(pData[i] == FIELD_DELIMITER_SYMBOL)
    {
        pField[0] = '\0';
        return FALSE;
    }

    while(pData[i] != FIELD_DELIMITER_SYMBOL && pData[i])
    {
        pField[j] = pData[i];
        j++;
        i++;

        if(j >= nMaxFieldLen)
        {
            j = nMaxFieldLen - 1;
            break;
        }
    }
    pField[j] = '\0';

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
UINT8 get_hex_from_string(char * buf)
{
    UINT8 i;
    UINT8 digit = 0;
    UINT8 len = strlen(buf);
    UINT8 temp = 0;

    if(len == 0)
    {
        digit = 0;
    }
    else if(len == 1)
    {
        if(buf[0] >= '0' && buf[0] <= '9')
        {
            digit = buf[0] - '0';
        }
        else if(buf[0] >= 'A' && buf[0] <= 'F')
        {
            digit = buf[0] - 'A' + 10;
        }
        else if(buf[0] >= 'a' && buf[0] <= 'f')
        {
            digit = buf[0] - 'a' + 10;
        }
        else
        {
            digit = 0;
        }
    }
    else
    {
        len = 2;
        digit = 0;
        for(i = 0; i < len; i++)
        {
            if(*(buf + len - 1 - i) >= '0' && *(buf + len - 1 - i) <= '9')
            {
                temp = *(buf + len - 1 - i) - '0';
            }
            else if(*(buf + len - 1 - i) >= 'A' && *(buf + len - 1 - i) <= 'F')
            {
                temp = *(buf + len - 1 - i) - 'A' + 10;
            }
            else if(*(buf + len - 1 - i) >= 'a' && *(buf + len - 1 - i) <= 'f')
            {
                temp = *(buf + len - 1 - i) - 'a' + 10;
            }
            else
            {
                temp = 0;
            }

            if(i == 0)
            {
                digit = temp;
            }
            else
            {
                digit |= (temp << 4) & 0xf0;
            }
        }
    }

    return digit;
}

