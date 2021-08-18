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


    Filename: iotbotscom_rs485.cpp

    General Description: qFWK RS485 Serial driver

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
#include "iotbotscom_rs485.h"

/* Other component includes. */

/* SDK includes */
#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32))
#include <HardwareSerial.h>
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP))
#include <SoftwareSerial.h>
#endif

/* Logging */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define RS485_RX_COMPLETE_TIMEOUT   200

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32))
HardwareSerial rs485_serial(2);
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP))
SoftwareSerial rs485_serial(RS485_RX_PIN, RS485_TX_PIN);
#endif
RS485_DEVICE_T rs485_device;

/*---- Function prototypes -------------------------------------------------------------*/
void rs485_init(void );
void rs485_on(void );
void rs485_off(void );
void rs485_suspend(void );
void rs485_resume(void );
void rs485_processor(void );
BOOL rs485_send(void );
BOOL rs485_recv(UINT16 timeout);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Init(void )
{
    if(rs485_device.mode != MODE_NONE)
    {
        return FALSE;
    }

    rs485_init();

    rs485_device.mode = MODE_INIT;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_On(void )
{
    if(!(rs485_device.mode == MODE_INIT || rs485_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    rs485_on();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Off(void )
{
    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    rs485_off();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Resume(void )
{
    if(rs485_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    rs485_resume();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Suspend(void )
{
    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    rs485_suspend();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Processor(void )
{
    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    rs485_processor();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Send(PBUF_T * p_tx)
{
    UINT8 ret = FALSE;
    UINT16 i;

    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    if(!(p_tx != NULL && p_tx->len != 0 && p_tx->buf != NULL))
    {
        return FALSE;
    }

    for(i = 0; i < p_tx->len; i++)
    {
        rs485_device.tx.buf[i] = p_tx->buf[i];
        rs485_device.tx.len++;
    }
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    ret = rs485_send();

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Recv(PBUF_T * p_rx, UINT16 timeout)
{
    UINT8 ret = FALSE;
    UINT16 i;

    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    if(!(p_rx != NULL && p_rx->buf != NULL))
    {
        return FALSE;
    }

    if(rs485_recv(timeout) != FALSE)
    {
        p_rx->len = 0;
        for(i = 0; i < rs485_device.rx.len; i++)
        {
            p_rx->buf[i] = rs485_device.rx.buf[i];
            p_rx->len++;
        }

        ret = TRUE;
    }

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Trsf(PBUF_T * p_tx, PBUF_T * p_rx, UINT16 timeout)
{
    UINT8 ret = FALSE;
    UINT16 i;

    if(rs485_device.mode != MODE_ON)
    {
        return FALSE;
    }

    if(!(p_rx != NULL && p_rx->buf != NULL
        && p_tx != NULL && p_tx->len != 0 && p_tx->buf != NULL))
    {
        return FALSE;
    }

    for(i = 0; i < p_tx->len; i++)
    {
        rs485_device.tx.buf[i] = p_tx->buf[i];
        rs485_device.tx.len++;
    }
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    if(rs485_send() == TRUE)
    {
        if(rs485_recv(timeout) != FALSE)
        {
            for(i = 0; i < rs485_device.rx.len; i++)
            {
                p_rx->buf[i] = rs485_device.rx.buf[i];
                p_rx->len++;
            }

            ret = TRUE;
        }
    }

    return ret;
}


/*--------------------------------------------------------------------------------------*/
MODE_T RS485_Get_Mode(void )
{
    return rs485_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL RS485_Is_Active(BOOL * p_status)
{
    if(rs485_device.mode != MODE_ON || p_status == NULL)
    {
        return FALSE;
    }

    *p_status = (BOOL)rs485_device.status.is_active;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void rs485_init(void )
{
    rs485_device.rx.buf = rs485_device.buf_rx;
    rs485_device.rx.len = 0;
    rs485_device.rx.buf[rs485_device.rx.len] = 0;

    rs485_device.tx.buf = rs485_device.buf_tx;
    rs485_device.tx.len = 0;
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    // RS Control Pin
    pinMode(RS485_CNTL_PIN, OUTPUT);
    digitalWrite(RS485_CNTL_PIN, LOW);

#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32))
    rs485_serial.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP))
    rs485_serial.begin(9600);
#else
    Serial1.begin(9600);
#endif

    delay(100);

    rs485_device.status.is_active = FALSE;
}

/*--------------------------------------------------------------------------------------*/
void rs485_on(void )
{
    rs485_device.rx.len = 0;
    rs485_device.rx.buf[rs485_device.rx.len] = 0;

    rs485_device.tx.len = 0;
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    rs485_device.status.is_active = FALSE;

    digitalWrite(RS485_CNTL_PIN, LOW);

    rs485_device.mode = MODE_ON;
}

/*--------------------------------------------------------------------------------------*/
void rs485_off(void )
{
#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_SAMD_MKR))
    Serial1.begin(9600);
#else
    rs485_serial.end();
#endif

    rs485_device.status.is_active = FALSE;

    digitalWrite(RS485_CNTL_PIN, LOW);

    rs485_device.mode = MODE_OFF;
}

/*--------------------------------------------------------------------------------------*/
void rs485_suspend(void )
{
    rs485_device.status.is_active = FALSE;

    digitalWrite(RS485_CNTL_PIN, LOW);

    rs485_device.mode = MODE_POWERSAVE;
}

/*--------------------------------------------------------------------------------------*/
void rs485_resume(void )
{
    rs485_device.rx.len = 0;
    rs485_device.rx.buf[rs485_device.rx.len] = 0;

    rs485_device.tx.len = 0;
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    rs485_device.status.is_active = FALSE;

    digitalWrite(RS485_CNTL_PIN, LOW);

    rs485_device.mode = MODE_ON;
}

/*--------------------------------------------------------------------------------------*/
void rs485_processor(void )
{
    DBG_RS485("rs485_processor begin");

    // Noting to do

    DBG_RS485("rs485_processor end");
}

/*--------------------------------------------------------------------------------------*/
BOOL rs485_send(void )
{
    UINT8 ret = FALSE;
    UINT8 delay_timeout = 0;
    UINT16 i;

    digitalWrite(RS485_CNTL_PIN, HIGH);

    delay(5);

#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_SAMD_MKR))
    if(Serial1.write(rs485_device.tx.buf, rs485_device.tx.len) == rs485_device.tx.len)
#else
    if(rs485_serial.write(rs485_device.tx.buf, rs485_device.tx.len) == rs485_device.tx.len)
#endif
    {
        delay_timeout = 1 * rs485_device.tx.len;

        ret = TRUE;

        if(LOG_DBG_CHECK(DBG_LOG_RS485))
        {
            Serial.print("TX: ");

            for(i = 0; i < rs485_device.tx.len; i++)
            {
                Serial.print("0x");
                Serial.print(rs485_device.tx.buf[i], HEX);
                Serial.print(" ");
            }
            Serial.print(", ");
            Serial.print(rs485_device.tx.len);
            Serial.println("");
        }
    }

    rs485_device.tx.len = 0;
    rs485_device.tx.buf[rs485_device.tx.len] = 0;

    if(delay_timeout != 0)
    {
        delay(delay_timeout + 5);
    }

    digitalWrite(RS485_CNTL_PIN, LOW);

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL rs485_recv(UINT16 timeout)
{
    UINT8 byte;
    unsigned long start_rx_time = millis();
    unsigned long start_timeout_time = millis();
    BOOL is_first_byte_received = FALSE;
    UINT16 i;

    rs485_device.status.is_active = FALSE;

    rs485_device.rx.len = 0;
    rs485_device.rx.buf[rs485_device.rx.len] = 0;

    while(((millis() - start_rx_time < timeout) && (is_first_byte_received == FALSE))
            || ((is_first_byte_received == TRUE) && (millis() - start_timeout_time < RS485_RX_COMPLETE_TIMEOUT)))
    {
#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_SAMD_MKR))
        if(Serial1.available())
#else
        if(rs485_serial.available())
#endif
        {
            if(is_first_byte_received == FALSE)
            {
                is_first_byte_received = TRUE;
            }

            start_timeout_time = millis();

#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_SAMD_MKR))
            byte = Serial1.read();
#else
            byte = rs485_serial.read();
#endif

            rs485_device.rx.buf[rs485_device.rx.len++] = byte;

            rs485_device.status.is_active = TRUE;
        }
    }

    if(is_first_byte_received == TRUE)
    {
        rs485_device.rx.buf[rs485_device.rx.len] = 0;

        //DBG_RS485("Some Data Received");

        if(LOG_DBG_CHECK(DBG_LOG_RS485))
        {
            Serial.print("RX: ");

            for(i = 0; i < rs485_device.rx.len; i++)
            {
                Serial.print("0x");
                Serial.print(rs485_device.rx.buf[i], HEX);
                Serial.print(" ");
            }
            Serial.print(", ");
            Serial.print(rs485_device.rx.len);
            Serial.println("");
        }

        return TRUE;
    }
    else
    {
        DBG_RS485("No Data Received");

        return FALSE;
    }
}

