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


    Filename: iotbotscom_pcf85063a.cpp

    General Description: qFWK RTC Chip driver

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
#include "iotbotscom_pcf85063a.h"
#include "../../hal/i2c/iotbotscom_i2c.h"

/* Logging... */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define RTC_CONTROL1_REG    0x00
#define RTC_CONTROL2_REG    0x01
#define RTC_OFFSET_REG      0x02
#define RTC_RAM_BYTE_REG    0x03
#define RTC_SECS_REG        0x04
#define RTC_MINS_REG        0x05
#define RTC_HOURS_REG       0x06
#define RTC_DAYS_REG        0x07
#define RTC_WDAYS_REG       0x08
#define RTC_MONTHS_REG      0x09
#define RTC_YEARS_REG       0x0A
// For PCF85063A ONLY
#define RTC_SEC_ALARM_REG   0x0B
#define RTC_MIN_ALARM_REG   0x0C
#define RTC_HOUR_ALARM_REG  0x0D
#define RTC_DAY_ALARM_REG   0x0E
#define RTC_WDAY_ALARM_REG  0x0F
#define RTC_TIMER_VALUE_REG 0x10
#define RTC_TIMER_MODE_REG  0x11

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
RTC_DEVICE_T rtc_device;
extern BOOL rtc_interrupt;

/*---- Function prototypes -------------------------------------------------------------*/
void rtc_init(void );
BOOL rtc_on(void );
BOOL rtc_off(void );
BOOL rtc_suspend(void );
BOOL rtc_resume(void );
BOOL rtc_config(RTC_SETTINGS_T  * p_rtc_settings);
BOOL rtc_get_td(TD_T *p_td);
BOOL rtc_set_td(TD_T *p_td);
UINT8 bcd2bin(UINT8 bcd);
UINT8 bin2bcd(UINT8 bin);
void rtc_print_time(TD_T * p_td);

// Short Term
#define GPIO_INT_RTC (1 << 1)
void rtc_interrupt_enable(UINT16 gpio_int);
void rtc_interrupt_disable(UINT16 gpio_int);
#if ((defined HW_PLATFORM_TYPE) && ((HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP) || (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32)))
void ICACHE_RAM_ATTR rtc_interrupt_handler(void );
#else
void rtc_interrupt_handler(void );
#endif

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL RTC_Init(void )
{
    if(rtc_device.mode != MODE_NONE)
    {
        return FALSE;
    }

    rtc_init();

    rtc_device.mode = MODE_INIT;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_On(void )
{
    if(!(rtc_device.mode == MODE_INIT || rtc_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    return rtc_on();
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_Off(void )
{
    if(rtc_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return rtc_off();
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_Resume(void )
{
    if(rtc_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    return rtc_resume();
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_Suspend(void )
{
    if(rtc_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return rtc_suspend();
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_Config(RTC_SETTINGS_T  * p_rtc_settings)
{
    if(rtc_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return rtc_config(p_rtc_settings);
}

/*--------------------------------------------------------------------------------------*/
MODE_T RTC_Get_Mode(void )
{
    return rtc_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_GetTD(TD_T *p_td)
{
    if(rtc_device.mode != MODE_ON)
    {
        return FALSE;
    }

    rtc_get_td(p_td);

    rtc_print_time(p_td);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL RTC_SetTD(TD_T *p_td)
{
    if(rtc_device.mode != MODE_ON)
    {
        return FALSE;
    }

    rtc_set_td(p_td);

    rtc_print_time(p_td);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void rtc_init(void )
{
    hal_i2c_init();
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_on(void )
{
    UINT8 data = 0;
    TD_T td;

    hal_i2c_on(PCF85063A_I2C_ADDRESS);

    hal_i2c_read_register(PCF85063A_I2C_ADDRESS, RTC_RAM_BYTE_REG, 1, (UINT8 *)&data);
    if(1)//data != 0xa5)
    {
        LOG_RTC("RTC info Reset\r\n");

        data = 0x01;
        hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL1_REG, 1, (UINT8 *)&data);

        data = 0x07;//0x2e;
        hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL2_REG, 1, (UINT8 *)&data);

        td.date.day = 1;
        td.date.month = 7;
        td.date.year = 18;

        td.time.hour = 22;
        td.time.minute = 12;
        td.time.second = 12;

        rtc_set_td(&td);

#if 1 // For PCF85063A ONLY
        rtc_interrupt_enable(GPIO_INT_RTC);

        data = 0x01; // 1 sec Alarm
        //data = 0x0A; // 10 sec Alarm
        //data = 0x03; // 3 sec Alarm
        //data = 0x0a; // 10 min Alarm
        hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_TIMER_VALUE_REG, 1, (UINT8 *)&data);

        data = 0x17; // 1 Hz // sec Alarm
        //data = 0x1f; // 1/60 Hz // min Alarm
        hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_TIMER_MODE_REG, 1, (UINT8 *)&data);
#endif

        data = 0xa5;
        hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_RAM_BYTE_REG, 1, (UINT8 *)&data);
    }
    else
    {
        LOG_RTC("RTC info Restored\r\n");
    }

    rtc_device.mode = MODE_ON;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_off(void )
{
    UINT8 data;

    // Disable Time Int
    data = 0x15;
    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_TIMER_MODE_REG, 1, (UINT8 *)&data);

    data = 0x07;
    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL2_REG, 1, (UINT8 *)&data);

    hal_i2c_off(PCF85063A_I2C_ADDRESS);

    rtc_device.mode = MODE_OFF;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_suspend(void )
{
    UINT8 data;

    data = 0x07;
    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL2_REG, 1, (UINT8 *)&data);

    hal_i2c_suspend(PCF85063A_I2C_ADDRESS);

    rtc_device.mode = MODE_POWERSAVE;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_resume(void )
{
    hal_i2c_resume(PCF85063A_I2C_ADDRESS);

    rtc_device.mode = MODE_ON;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_config(RTC_SETTINGS_T  * p_rtc_settings)
{
    rtc_device.settings = *p_rtc_settings;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_get_td(TD_T *p_td)
{
    UINT8 buf[7];

    hal_i2c_read_register(PCF85063A_I2C_ADDRESS, RTC_SECS_REG, 7, buf);

    rtc_device.td.time.second = bcd2bin(buf[0] & 0x7f);
    rtc_device.td.time.minute = bcd2bin(buf[1] & 0x7f);
    rtc_device.td.time.hour = bcd2bin(buf[2] & 0x3f);

    rtc_device.td.date.day = bcd2bin(buf[3] & 0x3f);
    rtc_device.td.date.wday = buf[4] & 0x07;
    rtc_device.td.date.month = bcd2bin(buf[5] & 0x1F);
    rtc_device.td.date.year = bcd2bin(buf[6]);

    *p_td = rtc_device.td;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL rtc_set_td(TD_T *p_td)
{
    UINT8 buf[7];
    UINT8 data;

    hal_i2c_read_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL1_REG, 1, (UINT8 *)&data);
    data |= 0x20;
    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL1_REG, 1, (UINT8 *)&data);

    rtc_device.td = *p_td;

    buf[0] = bin2bcd(rtc_device.td.time.second) & 0x07;
    buf[1] = bin2bcd(rtc_device.td.time.minute);
    buf[2] = bin2bcd(rtc_device.td.time.hour);

    buf[3] = bin2bcd(rtc_device.td.date.day);
    buf[4] = rtc_device.td.date.wday & 0x07;
    buf[5] = bin2bcd(rtc_device.td.date.month);
    buf[6] = bin2bcd(rtc_device.td.date.year);

    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_SECS_REG, 7, buf);

    data &= 0xdf;
    hal_i2c_write_register(PCF85063A_I2C_ADDRESS, RTC_CONTROL1_REG, 1, (UINT8 *)&data);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
UINT8 bcd2bin(UINT8 bcd)
{
    return ( ((bcd) >> 4) * 10 + ((bcd) & 0x0F) );
}

/*--------------------------------------------------------------------------------------*/
UINT8 bin2bcd(UINT8 bin)
{
    return ( (((bin) / 10) << 4) + ((bin) % 10) );
}

/*--------------------------------------------------------------------------------------*/
void rtc_print_time(TD_T * p_td)
{
    if(LOG_USR_CHECK(USR_LOG_RTC))
    {
        Serial.print("DT : ");

        //Serial.printf("%02d/%02d/%02d", p_td->date.month, p_td->date.day, p_td->date.year);
        Serial.print(p_td->date.month);
        Serial.print("/");
        Serial.print(p_td->date.day);
        Serial.print("/");
        Serial.print(p_td->date.year);

        Serial.print(" | ");

        //Serial.printf("%02d:%02d:%02d", p_td->time.hour, p_td->time.minute, p_td->time.second);
        Serial.print(p_td->time.hour);
        Serial.print(":");
        Serial.print(p_td->time.minute);
        Serial.print(":");
        Serial.print(p_td->time.second);

        Serial.println("");
    }
}

/*--------------------------------------------------------------------------------------*/
void rtc_interrupt_enable(UINT16 gpio_int)
{
    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RTC_INT_PIN), rtc_interrupt_handler, FALLING);

    LOG_DEV("RTC interrupt enabled\r\n");
}

/*--------------------------------------------------------------------------------------*/
void rtc_interrupt_disable(UINT16 gpio_int)
{
    detachInterrupt(digitalPinToInterrupt (RTC_INT_PIN));

    LOG_DEV("RTC interrupt disabled\r\n");
}

/*--------------------------------------------------------------------------------------*/
#if ((defined HW_PLATFORM_TYPE) && ((HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP) || (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32)))
void ICACHE_RAM_ATTR rtc_interrupt_handler(void )
{
    rtc_interrupt = TRUE;
}
#else
void rtc_interrupt_handler(void )
{
    rtc_interrupt = TRUE;
}
#endif


