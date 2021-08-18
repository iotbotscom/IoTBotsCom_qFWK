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


    Filename: iotbotscom_lis3dh.cpp

    General Description: qFWK LIS3DH Motion Sensor driver

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
#include "iotbotscom_lis3dh.h"
#include "../../hal/i2c/iotbotscom_i2c.h"

/* Other component includes. */

/* Logging... */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define LIS3DH_STATUS_REG_AUX_ADDR   0x07
#define LIS3DH_OUT_ADC1_L_ADDR       0x08
#define LIS3DH_OUT_ADC1_H_ADDR       0x09
#define LIS3DH_OUT_ADC2_L_ADDR       0x0a
#define LIS3DH_OUT_ADC2_H_ADDR       0x0b
#define LIS3DH_OUT_ADC3_L_ADDR       0x0c
#define LIS3DH_OUT_ADC3_H_ADDR       0x0d
#define LIS3DH_INT_COUNTER_REG_ADDR  0x0e
#define LIS3DH_WHO_AM_I_ADDR         0x0F
#define LIS3DH_TEMP_CFG_ADDR         0x1F
#define LIS3DH_CTRL_REG1_ADDR        0x20
#define LIS3DH_CTRL_REG2_ADDR        0x21
#define LIS3DH_CTRL_REG3_ADDR        0x22
#define LIS3DH_CTRL_REG4_ADDR        0x23
#define LIS3DH_CTRL_REG5_ADDR        0x24
#define LIS3DH_CTRL_REG6_ADDR        0x25
#define LIS3DH_REFERENCE_ADDR        0x26
#define LIS3DH_STATUS_REG_ADDR       0x27
#define LIS3DH_OUT_X_L_ADDR          0x28
#define LIS3DH_OUT_X_H_ADDR          0x29
#define LIS3DH_OUT_Y_L_ADDR          0x2A
#define LIS3DH_OUT_Y_H_ADDR          0x2B
#define LIS3DH_OUT_Z_L_ADDR          0x2C
#define LIS3DH_OUT_Z_H_ADDR          0x2D
#define LIS3DH_FIFO_CTRL_REG_ADDR    0x2E
#define LIS3DH_FIFO_SRC_REG_ADDR     0x2F
#define LIS3DH_INT1_CFG_ADDR         0x30
#define LIS3DH_INT1_SRC_ADDR         0x31
#define LIS3DH_INT1_THS_ADDR         0x32
#define LIS3DH_INT1_DURATION_ADDR    0x33
#define LIS3DH_INT2_CFG_ADDR         0x34
#define LIS3DH_INT2_SRC_ADDR         0x35
#define LIS3DH_INT2_THS_ADDR         0x36
#define LIS3DH_INT2_DURATION_ADDR    0x37
#define LIS3DH_CLICK_CFG_ADDR        0x38
#define LIS3DH_CLICK_SRC_ADDR        0x39
#define LIS3DH_CLICK_THS_ADDR        0x3A
#define LIS3DH_TIME_LIMIT_ADDR       0x3B
#define LIS3DH_TIME_LATENCY_ADDR     0x3C
#define LIS3DH_TIME_WINDOW_ADDR      0x3D
#define LIS3DH_ACT_THS_ADDR          0x3E
#define LIS3DH_INACT_DUR_ADDR        0x3F

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
LIS3DH_DEVICE_T lis3dh_device;

/*---- Function prototypes -------------------------------------------------------------*/
void lis3dh_init(void );
BOOL lis3dh_on(void );
BOOL lis3dh_off(void );
BOOL lis3dh_suspend(void );
BOOL lis3dh_resume(void );
BOOL lis3dh_config(void );
BOOL lis3dh_write_reg(UINT8 addr, UINT8 data);
BOOL lis3dh_read_reg(UINT8 addr, UINT8 * p_data);

// Short Term
#define GPIO_INT_MOTION (1 << 0)
void gpio_interrupt_enable(UINT16 gpio_int);//, void * handler);
void gpio_interrupt_disable(UINT16 gpio_int);//, void * handler);
#if ((defined HW_PLATFORM_TYPE) && ((HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP) || (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32)))
void ICACHE_RAM_ATTR gpio_interrupt_handler(void );
#else
void gpio_interrupt_handler(void );
#endif
extern BOOL motion_interrupt;

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Init(void )
{
    if(lis3dh_device.mode != MODE_NONE)
    {
        return FALSE;
    }

    lis3dh_init();

    lis3dh_device.mode = MODE_INIT;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_On(void )
{
    if(!(lis3dh_device.mode == MODE_INIT || lis3dh_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    return lis3dh_on();
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Off(void )
{
    if(lis3dh_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return lis3dh_off();
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Resume(void )
{
    if(lis3dh_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    return lis3dh_resume();
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Suspend(void )
{
    if(lis3dh_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return lis3dh_suspend();
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Config(LIS3DH_SETTINGS_T * p_settings)
{
    BOOL ret = FALSE;

    if(p_settings->control.is_enabled != lis3dh_device.settings.control.is_enabled)
    {
        if(p_settings->control.is_enabled == FALSE)
        {
            if(lis3dh_device.mode == MODE_ON)
            {
                ret = lis3dh_off();
            }
        }
        else
        {
            if(lis3dh_device.mode == MODE_OFF || lis3dh_device.mode == MODE_INIT)
            {
                ret = lis3dh_on();
            }
        }
    }
    else
    {
        lis3dh_device.settings = *p_settings;

        if(lis3dh_device.mode == MODE_ON)
        {
            ret = lis3dh_config();
        }
    }

    return ret;
}

/*--------------------------------------------------------------------------------------*/
MODE_T LIS3DH_Get_Mode(void )
{
    return lis3dh_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Get_Data(LIS3DH_SAMPLE_T *p_data)
{
    UINT8 val;
    SINT16 axis_data;

    if(lis3dh_device.mode != MODE_ON)
    {
        return FALSE;
    }

    lis3dh_read_reg(LIS3DH_STATUS_REG_ADDR, &val);

    if(val & 0x08)
    {
        lis3dh_read_reg(LIS3DH_OUT_X_L_ADDR, &val);
        axis_data = ((UINT16)val) & 0x00ff;
        lis3dh_read_reg(LIS3DH_OUT_X_H_ADDR, &val);
        axis_data |= ((((UINT16)val) << 8) & 0xff00);
        axis_data = (axis_data) >> 4;

        if(lis3dh_device.settings.control.x_axis_direction != TRUE)
        {
            lis3dh_device.data.x = -axis_data;
        }
        else
        {
            lis3dh_device.data.x = axis_data;
        }

        lis3dh_read_reg(LIS3DH_OUT_Y_L_ADDR, &val);
        axis_data = ((UINT16)val) & 0x00ff;
        lis3dh_read_reg(LIS3DH_OUT_Y_H_ADDR, &val);
        axis_data |= ((((UINT16)val) << 8) & 0xff00);
        axis_data = (axis_data) >> 4;

        if(lis3dh_device.settings.control.y_axis_direction != TRUE)
        {
            lis3dh_device.data.y = -axis_data;
        }
        else
        {
            lis3dh_device.data.y = axis_data;
        }

        lis3dh_read_reg(LIS3DH_OUT_Z_L_ADDR, &val);
        axis_data = ((UINT16)val) & 0x00ff;
        lis3dh_read_reg(LIS3DH_OUT_Z_H_ADDR, &val);
        axis_data |= ((((UINT16)val) << 8) & 0xff00);
        axis_data = (axis_data) >> 4;

        if(lis3dh_device.settings.control.z_axis_direction != TRUE)
        {
            lis3dh_device.data.z = -axis_data;
        }
        else
        {
            lis3dh_device.data.z = axis_data;
        }

        if(LOG_USR_CHECK(USR_LOG_MSENSOR))
        {
            Serial.print("LIS3DH :");
            Serial.print(" x = ");
            Serial.print(lis3dh_device.data.x);
            Serial.print(" y = ");
            Serial.print(lis3dh_device.data.y);
            Serial.print(" z = ");
            Serial.print(lis3dh_device.data.z);
            Serial.println("");
        }

        *p_data = lis3dh_device.data;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL LIS3DH_Get_IntSource(LIS3DH_INT_SOURCE_T * p_int)
{
    UINT8 val;

    if(lis3dh_device.mode != MODE_ON)
    {
        return FALSE;
    }

    p_int->active = FALSE;
    p_int->shoke= FALSE;
    p_int->motion = FALSE;

    lis3dh_read_reg(LIS3DH_INT1_SRC_ADDR, &val);

    if(val & 0x40)
    {
        p_int->active = TRUE;
        p_int->shoke= TRUE;
    }

    lis3dh_read_reg(LIS3DH_CLICK_SRC_ADDR, &val);

    if(val & 0x40)
    {
        p_int->active = TRUE;
        p_int->motion = TRUE;
    }

    lis3dh_read_reg(LIS3DH_INT2_SRC_ADDR, &val);

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void lis3dh_init(void )
{
    hal_i2c_init();

    lis3dh_device.settings.motion_treshould = 10;
    lis3dh_device.settings.shoke_treshould = 127;
    lis3dh_device.settings.control.is_enabled = TRUE;
    lis3dh_device.settings.control.motion_treshould = TRUE;
    lis3dh_device.settings.control.shoke_treshould = TRUE;
    lis3dh_device.settings.control.int_enabled = TRUE;
    lis3dh_device.settings.control.sleep_int_enabled = TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_on(void )
{
    UINT8 wai = 0;

    //GET_DEVICE_INFO(DEVICE_SETTINGS_LIS3DH_PARAM, (void *)&lis3dh_device.settings);

    if(lis3dh_device.settings.control.is_enabled == FALSE)
    {
        return FALSE;
    }

    lis3dh_device.data.x = 0;
    lis3dh_device.data.y = 0;
    lis3dh_device.data.z = 0;

    hal_i2c_on(LIS3DH_I2C_ADDRESS);

    lis3dh_read_reg(LIS3DH_WHO_AM_I_ADDR, &wai);

    if(wai == 0x33)
    {
        // 50Hz / XYZ Enabled
        lis3dh_write_reg(LIS3DH_CTRL_REG1_ADDR, 0x47);
        lis3dh_read_reg(LIS3DH_CTRL_REG1_ADDR, &wai);

        // High-pass filter Enabled for both INTs
        lis3dh_write_reg(LIS3DH_CTRL_REG2_ADDR, 0x05);
        lis3dh_read_reg(LIS3DH_CTRL_REG2_ADDR, &wai);

        // I1 CLICK & IA1 Enabled
        lis3dh_write_reg(LIS3DH_CTRL_REG3_ADDR, 0xc0);
        lis3dh_read_reg(LIS3DH_CTRL_REG3_ADDR, &wai);

        // ±2 g  & High-resolution mode On
        lis3dh_write_reg(LIS3DH_CTRL_REG4_ADDR, 0x08);
        lis3dh_read_reg(LIS3DH_CTRL_REG4_ADDR, &wai);

        // INT1 request latched
        lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
        lis3dh_read_reg(LIS3DH_CTRL_REG5_ADDR, &wai);

        // INT_POLARITY active level
        //lis3dh_write_reg(LIS3DH_CTRL_REG6_ADDR, 0x00); // HIGH
        lis3dh_write_reg(LIS3DH_CTRL_REG6_ADDR, 0x02); // LOW
        lis3dh_read_reg(LIS3DH_CTRL_REG6_ADDR, &wai);

        // Reference value  = 0
        lis3dh_write_reg(LIS3DH_REFERENCE_ADDR, 0x00);

        // Temp & ADC disabled
        lis3dh_write_reg(LIS3DH_TEMP_CFG_ADDR, 0x00);

        lis3dh_config();

        lis3dh_device.mode = MODE_ON;

        return TRUE;
    }
    else
    {
        hal_i2c_off(LIS3DH_I2C_ADDRESS);

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_off(void )
{
    // High-resolution mode Off
    lis3dh_write_reg(LIS3DH_CTRL_REG4_ADDR, 0x00);

    // Power Down & all axis disabled
    lis3dh_write_reg(LIS3DH_CTRL_REG1_ADDR, 0x00);

    hal_i2c_off(LIS3DH_I2C_ADDRESS);

    lis3dh_device.mode = MODE_OFF;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_suspend(void )
{
    UINT8 motion_duration = 10;
    UINT8 shoke_duration = 3;
    UINT8 val;
    BOOL int_enabled = FALSE;

    // Motion treshould and motion interrupt
    if(lis3dh_device.settings.control.motion_treshould == TRUE)
    {
        lis3dh_write_reg(LIS3DH_CLICK_THS_ADDR, lis3dh_device.settings.motion_treshould);
        lis3dh_write_reg(LIS3DH_TIME_LIMIT_ADDR, motion_duration);

        if(lis3dh_device.settings.control.sleep_int_enabled == TRUE)
        {
            // XYZ High Event Int Enabled
            lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x15);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);

            int_enabled = TRUE;
        }
        else
        {
            // XYZ High Event Int Disabled
            lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x00);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
        }
    }
    else
    {
        // XYZ High Event Int Disabled
        lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x00);
        // INT1 request latched
        lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
    }

    lis3dh_read_reg(LIS3DH_CLICK_SRC_ADDR, &val);

    // Shoke treshould and motion interrupt
    if(lis3dh_device.settings.control.shoke_treshould == TRUE)
    {
        lis3dh_write_reg(LIS3DH_INT1_THS_ADDR, lis3dh_device.settings.shoke_treshould);
        lis3dh_write_reg(LIS3DH_INT1_DURATION_ADDR, shoke_duration);

        if(lis3dh_device.settings.control.sleep_int_enabled == TRUE)
        {
            // XYZ High Event Int Enabled
            lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x2a);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);

            int_enabled = TRUE;
        }
        else
        {
            // XYZ High Event Int Disabled
            lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x00);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
        }
    }
    else
    {
        // XYZ High Event Int Disabled
        lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x00);
        // INT1 request latched
        lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
    }

    lis3dh_read_reg(LIS3DH_INT1_SRC_ADDR, &val);

    if(int_enabled == TRUE)
    {
        gpio_interrupt_enable(GPIO_INT_MOTION);
    }
    else
    {
        gpio_interrupt_disable(GPIO_INT_MOTION);
    }

    // Read Reference value
    lis3dh_read_reg(LIS3DH_REFERENCE_ADDR, &val);

    // High-resolution mode Off
    lis3dh_write_reg(LIS3DH_CTRL_REG4_ADDR, 0x00);

    // 10Hz / XYZ Enabled
    lis3dh_write_reg(LIS3DH_CTRL_REG1_ADDR, 0x2f);

    hal_i2c_suspend(LIS3DH_I2C_ADDRESS);

    lis3dh_device.mode = MODE_POWERSAVE;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_resume(void )
{
    UINT8 val;

    hal_i2c_resume(LIS3DH_I2C_ADDRESS);

    // Read Reference value
    lis3dh_read_reg(LIS3DH_REFERENCE_ADDR, &val);

    // ±2 g & High-resolution mode On
    lis3dh_write_reg(LIS3DH_CTRL_REG4_ADDR, 0x08);
    lis3dh_read_reg(LIS3DH_CTRL_REG4_ADDR, &val);

    // 50Hz / XYZ Enabled
    lis3dh_write_reg(LIS3DH_CTRL_REG1_ADDR, 0x47);
    lis3dh_read_reg(LIS3DH_CTRL_REG1_ADDR, &val);

    lis3dh_config();

    lis3dh_device.mode = MODE_ON;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_config(void )
{
    UINT8 motion_duration = 10;
    UINT8 shoke_duration = 3;
    UINT8 val;
    BOOL int_enabled = FALSE;

    // Motion treshould and motion interrupt
    if(lis3dh_device.settings.control.motion_treshould == TRUE)
    {
        lis3dh_write_reg(LIS3DH_CLICK_THS_ADDR, lis3dh_device.settings.motion_treshould);
        lis3dh_write_reg(LIS3DH_TIME_LIMIT_ADDR, motion_duration);

        if(lis3dh_device.settings.control.int_enabled == TRUE)
        {
            // XYZ High Event Int
            lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x15);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);

            int_enabled = TRUE;
        }
        else
        {
            lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x00);
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
        }
    }
    else
    {
        lis3dh_write_reg(LIS3DH_CLICK_CFG_ADDR, 0x00);
        lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
    }

    lis3dh_read_reg(LIS3DH_CLICK_SRC_ADDR, &val);

    // Shoke treshould and motion interrupt
    if(lis3dh_device.settings.control.shoke_treshould == TRUE)
    {
        lis3dh_write_reg(LIS3DH_INT1_THS_ADDR, lis3dh_device.settings.shoke_treshould);
        lis3dh_write_reg(LIS3DH_INT1_DURATION_ADDR, shoke_duration);

        if(lis3dh_device.settings.control.int_enabled == TRUE)
        {
            // XYZ High Event Int
            lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x2a);
            // INT1 request latched
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);

            int_enabled = TRUE;
        }
        else
        {
            lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x00);
            lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
        }
    }
    else
    {
        lis3dh_write_reg(LIS3DH_INT1_CFG_ADDR, 0x00);
        lis3dh_write_reg(LIS3DH_CTRL_REG5_ADDR, 0x08);
    }

    lis3dh_read_reg(LIS3DH_INT1_SRC_ADDR, &val);

    if(int_enabled == TRUE)
    {
        gpio_interrupt_enable(GPIO_INT_MOTION);
    }
    else
    {
        gpio_interrupt_disable(GPIO_INT_MOTION);
    }

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_write_reg(UINT8 addr, UINT8 data)
{
    return hal_i2c_write_register(LIS3DH_I2C_ADDRESS, addr, 1, &data);
}

/*--------------------------------------------------------------------------------------*/
BOOL lis3dh_read_reg(UINT8 addr, UINT8 * p_data)
{
    return hal_i2c_read_register(LIS3DH_I2C_ADDRESS, addr, 1, p_data);
}

/*--------------------------------------------------------------------------------------*/
void gpio_interrupt_enable(UINT16 gpio_int)
{
    pinMode(XINT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(XINT_PIN), gpio_interrupt_handler, FALLING);

    DBG_DEV("LIS3DH interrupt enabled\r\n");
}

/*--------------------------------------------------------------------------------------*/
void gpio_interrupt_disable(UINT16 gpio_int)
{
    detachInterrupt(digitalPinToInterrupt (XINT_PIN));

    DBG_DEV("LIS3DH interrupt disabled\r\n");
}

/*--------------------------------------------------------------------------------------*/
#if ((defined HW_PLATFORM_TYPE) && ((HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP) || (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32)))
void ICACHE_RAM_ATTR gpio_interrupt_handler(void )
#else
void gpio_interrupt_handler(void )
#endif
{
    motion_interrupt = TRUE;

    //LOG_DEV("IRQ\r\n");
}

