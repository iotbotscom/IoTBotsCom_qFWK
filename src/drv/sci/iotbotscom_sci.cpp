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


    Filename: iotbotscom_sci.cpp

    General Description: qFWK SCI driver

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
#include "iotbotscom_sci.h"

/* Other component includes. */

/* SDK includes */

/* Logging */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define SCI_UART_SPEED 115200

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
SCI_DEVICE_T sci_device;

/*---- Function prototypes -------------------------------------------------------------*/
void sci_init(void );
void sci_on(void );
void sci_off(void );
void sci_suspend(void );
void sci_resume(void );
void sci_processor(void );
BOOL sci_send(void );
BOOL sci_recv(void );

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Init(void )
{
    if(sci_device.mode != MODE_NONE)
    {
        return FALSE;
    }

    sci_init();

    sci_device.mode = MODE_INIT;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_On(void )
{
    if(!(sci_device.mode == MODE_INIT || sci_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    sci_on();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Off(void )
{
    if(sci_device.mode != MODE_ON)
    {
        return FALSE;
    }

    sci_off();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Resume(void )
{
    if(sci_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    sci_resume();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Suspend(void )
{
    if(sci_device.mode != MODE_ON)
    {
        return FALSE;
    }

    sci_suspend();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Processor(void )
{
    if(sci_device.mode != MODE_ON)
    {
        return FALSE;
    }

    sci_processor();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
MODE_T SCI_Get_Mode(void )
{
    return sci_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_Is_Active(BOOL * p_status)
{
    if(sci_device.mode != MODE_ON || p_status == NULL)
    {
        return FALSE;
    }

    *p_status = (BOOL)sci_device.status.is_active;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SCI_SetProcessorCall(SPC_T processor_call)
{
    if(sci_device.mode != MODE_INIT)
    {
        return FALSE;
    }

    if(processor_call != NULL)
    {
        sci_device.processor_call = processor_call;
    }
    else
    {
        sci_device.processor_call = NULL;
    }

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void sci_init(void )
{
    sci_device.rx.buf = sci_device.buf_rx;
    sci_device.rx.len = 0;
    sci_device.rx.buf[sci_device.rx.len] = 0;

    sci_device.tx.buf = sci_device.buf_tx;
    sci_device.tx.len = 0;
    sci_device.tx.buf[sci_device.tx.len] = 0;

    sci_device.processor_call = NULL;

    Serial.begin(SCI_UART_SPEED);
    delay(100);

    sci_device.status.is_active = FALSE;
}

/*--------------------------------------------------------------------------------------*/
void sci_on(void )
{
    sci_device.rx.len = 0;
    sci_device.rx.buf[sci_device.rx.len] = 0;

    sci_device.tx.len = 0;
    sci_device.tx.buf[sci_device.tx.len] = 0;

    sci_device.status.is_active = FALSE;

    sci_device.mode = MODE_ON;
}

/*--------------------------------------------------------------------------------------*/
void sci_off(void )
{
    Serial.end();

    sci_device.status.is_active = FALSE;

    sci_device.mode = MODE_OFF;
}

/*--------------------------------------------------------------------------------------*/
void sci_suspend(void )
{
    sci_device.status.is_active = FALSE;

    sci_device.mode = MODE_POWERSAVE;
}

/*--------------------------------------------------------------------------------------*/
void sci_resume(void )
{
    sci_device.rx.len = 0;
    sci_device.rx.buf[sci_device.rx.len] = 0;

    sci_device.tx.len = 0;
    sci_device.tx.buf[sci_device.tx.len] = 0;

    sci_device.status.is_active = FALSE;

    sci_device.mode = MODE_ON;
}

/*--------------------------------------------------------------------------------------*/
void sci_processor(void )
{
    DBG_SCI("sci_processor begin");

    if(sci_recv() == TRUE)
    {
        if(sci_device.processor_call != NULL)
        {
            if(sci_device.processor_call(&sci_device.rx, &sci_device.tx) == TRUE)
            {
                sci_send();
            }
        }
#if 0
        else
        {
            if(MDS(&sci_device.rx, &sci_device.tx) == TRUE)
            {
                sci_send();
            }
        }
#endif
        sci_device.rx.len = 0;
        sci_device.rx.buf[sci_device.rx.len] = 0;
    }

    DBG_SCI("sci_processor end");
}

/*--------------------------------------------------------------------------------------*/
BOOL sci_send(void )
{
    UINT8 ret = FALSE;

    if(sci_device.tx.len == 0)
    {
        return FALSE;
    }

    if(Serial.write(sci_device.tx.buf, sci_device.tx.len) == sci_device.tx.len)
    {
        ret = TRUE;
    }

    sci_device.tx.len = 0;
    sci_device.tx.buf[sci_device.tx.len] = 0;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sci_recv(void )
{
    UINT8 byte;

    sci_device.status.is_active = FALSE;

    while(Serial.available())
    {
        byte = Serial.read();

        sci_device.status.is_active = TRUE;

        if(byte == '\n')
        {
            sci_device.rx.buf[sci_device.rx.len++] = byte;

            sci_device.rx.buf[sci_device.rx.len] = 0;

            DBG_SCI("sci_processor : data received");

            if(LOG_DBG_CHECK(DBG_LOG_SCI))
            {
                Serial.write(sci_device.rx.buf, sci_device.rx.len);
                Serial.print(", ");
                Serial.print(sci_device.rx.len);
                Serial.println("");
            }
            return TRUE;
        }
        else
        {
            sci_device.rx.buf[sci_device.rx.len++] = byte;

            //DBG_SCI("sci_processor : some byte received");
        }
    }

    return FALSE;
}

