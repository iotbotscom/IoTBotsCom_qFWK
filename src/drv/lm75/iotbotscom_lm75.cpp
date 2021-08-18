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


    Filename: iotbotscom_lm75.cpp

    General Description: qFWK LM75 Temperature Sensor driver

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
#include "iotbotscom_lm75.h"
#include "../../hal/i2c/iotbotscom_i2c.h"

/* Other component includes. */

/* Logging... */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define LM75_TEMP_REG       0x00  // Temperature Register
#define LM75_CONF_REG       0x01  // Configuration Register
#define LM75_THYS_REG       0x02  // Temperature Register
#define LM75_TOS_REG        0x03  // Over-temp Shutdown threshold Register

#define TEMPERATURE_THYS    31
#define TEMPERATURE_TOS     32

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
LM75_DEVICE_T lm75_device;

/*---- Function prototypes -------------------------------------------------------------*/
void lm75_init(void );
BOOL lm75_on(void );
BOOL lm75_off(void );
BOOL lm75_suspend(void );
BOOL lm75_resume(void );
BOOL lm75_config(void );
BOOL lm75_get_temperature(SINT16 *p_temperature);

BOOL lm75_write_reg(UINT8 addr, UINT16 data);
BOOL lm75_read_reg(UINT8 addr, UINT16 * p_data);
BOOL lm75_write_confreg(UINT8 addr, UINT8 data);
BOOL lm75_read_confreg(UINT8 addr, UINT8 * p_data);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Init(void )
{
    if(lm75_device.mode != MODE_NONE)
    {
        return FALSE;
    }

    lm75_init();

    lm75_device.mode = MODE_INIT;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_On(void )
{
    if(!(lm75_device.mode == MODE_INIT || lm75_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    return lm75_on();
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Off(void )
{
    if(lm75_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return lm75_off();
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Resume(void )
{
    if(lm75_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    return lm75_resume();
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Suspend(void )
{
    if(lm75_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return lm75_suspend();
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Config(LM75_SETTINGS_T  * p_lm75_settings)
{
    BOOL ret = FALSE;

    if(p_lm75_settings->control.is_enabled != lm75_device.settings.control.is_enabled)
    {
        if(p_lm75_settings->control.is_enabled == FALSE)
        {
            if(lm75_device.mode == MODE_ON)
            {
                ret = lm75_off();
            }
        }
        else
        {
            if(lm75_device.mode == MODE_OFF || lm75_device.mode == MODE_INIT)
            {
                ret = lm75_on();
            }
        }
    }
    else
    {
        lm75_device.settings = *p_lm75_settings;

        if(lm75_device.mode == MODE_ON)
        {
            ret = lm75_config();
        }
    }

    return ret;
}

/*--------------------------------------------------------------------------------------*/
MODE_T LM75_Get_Mode(void )
{
    return lm75_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL LM75_Get_Data(LM75_SAMPLE_T *p_sample)
{
    return lm75_get_temperature(&p_sample->temperature);
}

/*--------------------------------------------------------------------------------------*/
void lm75_init(void )
{
    hal_i2c_init();

    lm75_device.settings.control.is_enabled = TRUE;
    lm75_device.settings.hyst = TEMPERATURE_THYS << 8;
    lm75_device.settings.temperature_high_alarm_level = TEMPERATURE_TOS << 8;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_on(void )
{
    UINT8 v8 = 0;
    UINT16 v16 = 0;

    //GET_DEVICE_INFO(DEVICE_SETTINGS_TSENSOR_PARAM, (void *)&lm75_device.selm75ings);

    if(lm75_device.settings.control.is_enabled == FALSE)
    {
        return FALSE;
    }

    hal_i2c_on(LM75_I2C_ADDRESS);

    if(lm75_read_confreg(LM75_CONF_REG, &v8 ) == TRUE)
    {
        lm75_write_confreg(LM75_CONF_REG, 0x02);
        lm75_read_confreg(LM75_CONF_REG, &v8 );

        lm75_config();

        lm75_read_reg(LM75_TEMP_REG, &v16);
        lm75_device.temperature = (((v16 << 8) & 0xff00) | ((v16 >> 8) & 0x00ff)) >> 7;

        lm75_device.mode = MODE_ON;

        return TRUE;
    }
    else
    {
        hal_i2c_off(LM75_I2C_ADDRESS);

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_off(void )
{
    lm75_write_confreg(LM75_CONF_REG, 0x03);

    hal_i2c_off(LM75_I2C_ADDRESS);

    lm75_device.mode = MODE_OFF;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_suspend(void )
{
    lm75_write_confreg(LM75_CONF_REG, 0x03);

    hal_i2c_suspend(LM75_I2C_ADDRESS);

    lm75_device.mode = MODE_POWERSAVE;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_resume(void )
{
    hal_i2c_resume(LM75_I2C_ADDRESS);

    lm75_write_confreg(LM75_CONF_REG, 0x02);

    lm75_device.mode = MODE_ON;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_config(void )
{
    UINT16 v16 = 0;

    lm75_write_reg(LM75_THYS_REG, lm75_device.settings.hyst);
    lm75_read_reg(LM75_THYS_REG, &v16 );

    lm75_write_reg(LM75_TOS_REG, lm75_device.settings.os);
    lm75_read_reg(LM75_TOS_REG, &v16 );

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_get_temperature(SINT16 *p_temperature)
{
    UINT16 v16 = 0;

    if(lm75_device.mode != MODE_ON)
    {
        *p_temperature = -128;

        return FALSE;
    }

    lm75_read_reg(LM75_TEMP_REG, &v16);
    lm75_device.temperature = (((v16 << 8) & 0xff00) | ((v16 >> 8) & 0x00ff)) >> 7;

    if(LOG_USR_CHECK(USR_LOG_TSENSOR))
    {
        Serial.println(String("LM75 : ") + (((float)lm75_device.temperature) / 2.0) + String(" C"));
    }

    *p_temperature = lm75_device.temperature;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_write_reg(UINT8 addr, UINT16 data)
{
    return hal_i2c_write_register(LM75_I2C_ADDRESS, addr, 2, (UINT8 *)&data);
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_read_reg(UINT8 addr, UINT16 * p_data)
{
    return hal_i2c_read_register(LM75_I2C_ADDRESS, addr, 2, (UINT8 *)p_data);
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_write_confreg(UINT8 addr, UINT8 data)
{

    return hal_i2c_write_register(LM75_I2C_ADDRESS, addr, 1, &data);
}

/*--------------------------------------------------------------------------------------*/
BOOL lm75_read_confreg(UINT8 addr, UINT8 * p_data)
{
    return hal_i2c_read_register(LM75_I2C_ADDRESS, addr, 1, p_data);
}

