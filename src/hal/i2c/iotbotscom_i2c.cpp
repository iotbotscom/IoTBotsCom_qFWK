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


    Filename: iotbotscom_i2c.cpp

    General Description: i2c driver

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
#include <Wire.h>
#include "iotbotscom_i2c.h"

/* Logging*/
#include "../../drv/logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define DEVICE_I2C_LIS3DH       (1 << 0)
#define DEVICE_I2C_LM75         (1 << 1)
#define DEVICE_I2C_BME280       (1 << 2)
#define DEVICE_I2C_PCF85063A    (1 << 3)
#define DEVICE_I2C_EEPROM       (1 << 4)
#define DEVICE_I2C_PCA9538A     (1 << 5)
#define DEVICE_I2C_PCA9685      (1 << 6)
#define DEVICE_I2C_TSL2591      (1 << 7)
#define DEVICE_I2C_CCS811       (1 << 8)
#define DEVICE_I2C_MS5611       (1 << 9)
#define DEVICE_I2C_ICM20948     (1 << 10)
#define DEVICE_I2C_VEML6070     (1 << 11)
#define DEVICE_I2C_UID          (1 << 12)
#define DEVICE_I2C_VCNL4040     (1 << 13)
#define DEVICE_I2C_TT           (1 << 14)

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _I2C_DEVICE_I2C_T
{
    BOOL is_init;
    UINT16 state;
    BOOL xMutex;
}I2C_DEVICE_I2C_T;

/*---- Variables -----------------------------------------------------------------------*/
I2C_DEVICE_I2C_T i2c_device = {0};

/*---- Function prototypes -------------------------------------------------------------*/
UINT8 i2c_write_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToWrite, UINT8 * RegisterValue);
UINT8 i2c_read_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToRead, UINT8 * RegisterValue);
BOOL i2c_get_status(UINT8 DeviceAddr);
UINT8 i2c_read_status(UINT8 DeviceAddr, UINT8 * RegisterValue);
UINT8 i2c_read_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToRead, UINT8 * p_RAM_addr);
UINT8 i2c_write_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToWrite, UINT8 * p_RAM_addr);
BOOL i2c_wait_memoryready(UINT8 DeviceAddr);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
void hal_i2c_init(void )
{
    if(i2c_device.is_init == FALSE)
    {
        i2c_device.xMutex = FALSE;

        i2c_device.is_init = TRUE;
    }
}

/*--------------------------------------------------------------------------------------*/
void hal_i2c_on(UINT8 DevAddr)
{
    if(i2c_device.xMutex != FALSE)
    {
        return;
    }
    else
    {
        i2c_device.xMutex = TRUE;
    }

    if(DevAddr == UID_I2C_ADDRESS || DevAddr == LIS3DH_I2C_ADDRESS || DevAddr == LM75_I2C_ADDRESS || DevAddr == BME280_I2C_ADDRESS || DevAddr == PCF85063A_I2C_ADDRESS || DevAddr == EEPROM_I2C_ADDRESS
        || DevAddr == TSL2591_I2C_ADDRESS || DevAddr == PCA9538A_I2C_ADDRESS || DevAddr == PCA9685_I2C_ADDRESS || DevAddr == CCS811_I2C_ADDRESS
        || DevAddr == MS5611_I2C_ADDRESS || DevAddr == ICM20948_I2C_ADDRESS || DevAddr == VEML6070_I2C_ADDRESS || DevAddr == UID_I2C_ADDRESS || DevAddr == VCNL4040_I2C_ADDRESS || DevAddr == TT_I2C_ADDRESS)
    {
        if(i2c_device.state == 0)
        {
#if ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP))
            Wire.pins(I2C_SDA_PIN, I2C_SCL_PIN);
            Wire.begin();
            Wire.setClock(400000);
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_ESP32))
            Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_SAMD_MKR))
            Wire.begin();
#elif ((defined HW_PLATFORM_TYPE) && (HW_PLATFORM_TYPE == HW_PLATFORM_TYPE_STM32))
            Wire.setSDA(I2C_SDA_PIN);
            Wire.setSCL(I2C_SCL_PIN);
            Wire.begin();
#else
            Wire.begin();
#endif
        }

        if(DevAddr == UID_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_UID)))
        {
            i2c_device.state |= DEVICE_I2C_UID;
        }

        if(DevAddr == TT_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_TT)))
        {
            i2c_device.state |= DEVICE_I2C_TT;
        }

        if(DevAddr == VCNL4040_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_VCNL4040)))
        {
            i2c_device.state |= DEVICE_I2C_VCNL4040;
        }

        if(DevAddr == LIS3DH_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_LIS3DH)))
        {
            i2c_device.state |= DEVICE_I2C_LIS3DH;
        }

        if(DevAddr == LM75_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_LM75)))
        {
            i2c_device.state |= DEVICE_I2C_LM75;
        }

        if(DevAddr == BME280_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_BME280)))
        {
            i2c_device.state |= DEVICE_I2C_BME280;
        }

        if(DevAddr == PCF85063A_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_PCF85063A)))
        {
            i2c_device.state |= DEVICE_I2C_PCF85063A;
        }

        if(DevAddr == EEPROM_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_EEPROM)))
        {
            i2c_device.state |= DEVICE_I2C_EEPROM;
        }

        if(DevAddr == PCA9538A_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_PCA9538A)))
        {
            i2c_device.state |= DEVICE_I2C_PCA9538A;
        }

        if(DevAddr == PCA9685_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_PCA9685)))
        {
            i2c_device.state |= DEVICE_I2C_PCA9685;
        }

        if(DevAddr == TSL2591_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_TSL2591)))
        {
            i2c_device.state |= DEVICE_I2C_TSL2591;
        }

        if(DevAddr == CCS811_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_CCS811)))
        {
            i2c_device.state |= DEVICE_I2C_CCS811;
        }

        if(DevAddr == MS5611_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_MS5611)))
        {
            i2c_device.state |= DEVICE_I2C_MS5611;
        }

        if(DevAddr == ICM20948_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_ICM20948)))
        {
            i2c_device.state |= DEVICE_I2C_ICM20948;
        }

        if(DevAddr == VEML6070_I2C_ADDRESS && (!(i2c_device.state & DEVICE_I2C_VEML6070)))
        {
            i2c_device.state |= DEVICE_I2C_VEML6070;
        }
    }

    i2c_device.xMutex = FALSE;
}

/*--------------------------------------------------------------------------------------*/
void hal_i2c_off(UINT8 DevAddr)
{
    if(i2c_device.xMutex != FALSE)
    {
        return;
    }
    else
    {
        i2c_device.xMutex = TRUE;
    }

    if(DevAddr == UID_I2C_ADDRESS || DevAddr == LIS3DH_I2C_ADDRESS || DevAddr == LM75_I2C_ADDRESS || DevAddr == BME280_I2C_ADDRESS || DevAddr == PCF85063A_I2C_ADDRESS || DevAddr == EEPROM_I2C_ADDRESS
        || DevAddr == TSL2591_I2C_ADDRESS || DevAddr == PCA9538A_I2C_ADDRESS || DevAddr == PCA9685_I2C_ADDRESS || DevAddr == CCS811_I2C_ADDRESS
        || DevAddr == MS5611_I2C_ADDRESS || DevAddr == ICM20948_I2C_ADDRESS || DevAddr == VEML6070_I2C_ADDRESS || DevAddr == UID_I2C_ADDRESS || DevAddr == VCNL4040_I2C_ADDRESS || DevAddr == TT_I2C_ADDRESS)
    {
        if(DevAddr == UID_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_UID;
        }

        if(DevAddr == TT_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_TT;
        }

        if(DevAddr == VCNL4040_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_VCNL4040;
        }

        if(DevAddr == LIS3DH_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_LIS3DH;
        }

        if(DevAddr == LM75_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_LM75;
        }

        if(DevAddr == BME280_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_BME280;
        }

        if(DevAddr == PCF85063A_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_PCF85063A;
        }

        if(DevAddr == EEPROM_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_EEPROM;
        }

        if(DevAddr == PCA9538A_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_PCA9538A;
        }

        if(DevAddr == PCA9685_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_PCA9685;
        }

        if(DevAddr == TSL2591_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_TSL2591;
        }

        if(DevAddr == CCS811_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_CCS811;
        }

        if(DevAddr == MS5611_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_MS5611;
        }

        if(DevAddr == ICM20948_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_ICM20948;
        }

        if(DevAddr == VEML6070_I2C_ADDRESS)
        {
            i2c_device.state &= ~DEVICE_I2C_VEML6070;
        }

        if(i2c_device.state == 0)
        {
            pinMode(I2C_SDA_PIN, OUTPUT);
            digitalWrite(I2C_SDA_PIN, HIGH);
            pinMode(I2C_SCL_PIN, OUTPUT);
            digitalWrite(I2C_SCL_PIN, HIGH);
        }
    }

    i2c_device.xMutex = FALSE;
}

/*--------------------------------------------------------------------------------------*/
void hal_i2c_suspend(UINT8 DevAddr)
{
}

/*--------------------------------------------------------------------------------------*/
void hal_i2c_resume(UINT8 DevAddr)
{
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_read_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToRead, UINT8 * p_RegisterValue)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("RD : 0x");
        Serial.print(RegisterAddr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToRead);
    }

    Wire.beginTransmission(DeviceAddr);
    Wire.write((uint8_t)RegisterAddr);
    ret = Wire.endTransmission(false);
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }

    Wire.requestFrom(DeviceAddr, NumByteToRead);

    if(Wire.available() == NumByteToRead)
    {
        for(i = 0; i < NumByteToRead; i++)
        {
            *(p_RegisterValue + i) = (UINT8)Wire.read();

            if(LOG_DBG_CHECK(DBG_LOG_I2C))
            {
                Serial.print(", 0x");
                Serial.print(*(p_RegisterValue + i), HEX);
            }
        }

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_write_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToWrite, UINT8 * p_RegisterValue)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("WR : 0x");
        Serial.print(RegisterAddr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToWrite);
    }

    Wire.beginTransmission(DeviceAddr);
    Wire.write((uint8_t)RegisterAddr);

    for(i = 0; i < NumByteToWrite; i++)
    {
        Wire.write((uint8_t)*(p_RegisterValue + i));

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.print(", 0x");
            Serial.print(*(p_RegisterValue + i), HEX);
        }
    }

    ret = Wire.endTransmission();
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_read_device(UINT8 DeviceAddr, UINT8 NumByteToRead, UINT8 * p_RegisterValue)
{
    UINT8 i;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("RD : 0x");
        Serial.print(DeviceAddr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToRead);
    }

    Wire.requestFrom(DeviceAddr, NumByteToRead);

    if(Wire.available() == NumByteToRead)
    {
        for(i = 0; i < NumByteToRead; i++)
        {
            *(p_RegisterValue + i) = (UINT8)Wire.read();

            if(LOG_DBG_CHECK(DBG_LOG_I2C))
            {
                Serial.print(", 0x");
                Serial.print(*(p_RegisterValue + i), HEX);
            }
        }

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_write_device(UINT8 DeviceAddr, UINT8 NumByteToWrite, UINT8 * p_RegisterValue)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("WR : 0x");
        Serial.print(DeviceAddr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToWrite);
    }

    Wire.beginTransmission(DeviceAddr);

    for(i = 0; i < NumByteToWrite; i++)
    {
        Wire.write((uint8_t)*(p_RegisterValue + i));

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.print(", 0x");
            Serial.print(*(p_RegisterValue + i), HEX);
        }
    }

    ret = Wire.endTransmission();
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_read_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToRead, UINT8 * p_RAM_addr)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("RD : 0x");
        Serial.print(EEPROM_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToRead);
    }

    Wire.beginTransmission(DeviceAddr);
    //Wire.write((uint8_t)((EEPROM_addr & 0xFF00) >> 8));
    Wire.write((uint8_t)(EEPROM_addr & 0x00FF));
    ret = Wire.endTransmission(false);
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }

    Wire.requestFrom(DeviceAddr, NumByteToRead);

    if(Wire.available() == NumByteToRead)
    {
        for(i = 0; i < NumByteToRead; i++)
        {
            *(p_RAM_addr + i) = (UINT8)Wire.read();

            if(LOG_DBG_CHECK(DBG_LOG_I2C))
            {
                Serial.print(", 0x");
                Serial.print(*(p_RAM_addr + i), HEX);
            }
        }

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_write_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToWrite, UINT8 * p_RAM_addr)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("WR : 0x");
        Serial.print(EEPROM_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToWrite);
    }

    Wire.beginTransmission(DeviceAddr);
    //Wire.write((uint8_t)((EEPROM_addr & 0xFF00) >> 8));
    Wire.write((uint8_t)(EEPROM_addr & 0x00FF));

    for(i = 0; i < NumByteToWrite; i++)
    {
        Wire.write((uint8_t)*(p_RAM_addr + i));

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.print(", 0x");
            Serial.print(*(p_RAM_addr + i), HEX);
        }
    }

    ret = Wire.endTransmission();
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_read_uid(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToRead, UINT8 * p_RAM_addr)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("RD : 0x");
        Serial.print(EEPROM_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToRead);
    }

    Wire.beginTransmission(DeviceAddr);
    Wire.write((uint8_t)(EEPROM_addr & 0x00FF));
    ret = Wire.endTransmission(false);
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }

    Wire.requestFrom(DeviceAddr, NumByteToRead);

    if(Wire.available() == NumByteToRead)
    {
        for(i = 0; i < NumByteToRead; i++)
        {
            *(p_RAM_addr + i) = (UINT8)Wire.read();

            if(LOG_DBG_CHECK(DBG_LOG_I2C))
            {
                Serial.print(", 0x");
                Serial.print(*(p_RAM_addr + i), HEX);
            }
        }

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_write_uid(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToWrite, UINT8 * p_RAM_addr)
{
    UINT8 i;
    uint8_t ret;

    if(LOG_DBG_CHECK(DBG_LOG_I2C))
    {
        Serial.print("WR : 0x");
        Serial.print(EEPROM_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(NumByteToWrite);
    }

    Wire.beginTransmission(DeviceAddr);
    Wire.write((uint8_t)(EEPROM_addr & 0x00FF));

    for(i = 0; i < NumByteToWrite; i++)
    {
        Wire.write((uint8_t)*(p_RAM_addr + i));

        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.print(", 0x");
            Serial.print(*(p_RAM_addr + i), HEX);
        }
    }

    ret = Wire.endTransmission();
    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Failed");
        }

        return FALSE;
    }
    else
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> OK");
        }

        return TRUE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_i2c_wait_memory_ready(UINT8 DeviceAddr)
{
    uint32_t entry_time;
    uint8_t ret = 1;

    entry_time = micros();

    while(micros() - entry_time <= 5000)
    {
        Wire.beginTransmission(DeviceAddr);

        ret = Wire.endTransmission();
        if(ret == 0)
        {
            break;
        }
    }

    if(ret != 0)
    {
        if(LOG_DBG_CHECK(DBG_LOG_I2C))
        {
            Serial.println(" >> Waiting Failed");
        }

        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

