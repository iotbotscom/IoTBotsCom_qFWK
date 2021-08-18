#ifndef IOTBOTSCOM_I2C_H
#define IOTBOTSCOM_I2C_H
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


    Filename: iotbotscom_i2c.h

    General Description: i2c driver

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
#define LIS3DH_I2C_ADDRESS          (0x32 >> 1) // Motion Sensor
#define LM75_I2C_ADDRESS            (0x90 >> 1) // Temperature Sensor
#define BME280_I2C_ADDRESS          (0xEC >> 1) // BME280
#define PCF85063A_I2C_ADDRESS       (0xA2 >> 1) // RTC Timer
#define EEPROM_I2C_ADDRESS          (0xA0 >> 1) // External EEPROM
#define UID_I2C_ADDRESS             (0xA4 >> 1) // UID EEPROM
#define PCA9538A_I2C_ADDRESS        (0xE0 >> 1) // IOE
#define PCA9685_I2C_ADDRESS         (0x80 >> 1) // PWM
#define TSL2591_I2C_ADDRESS         (0x52 >> 1) // TSL2591
#define CCS811_I2C_ADDRESS          (0xB4 >> 1) // CCS811
#define SSD1306_I2C_ADDRESS         (0x78 >> 1) // SSD1306
#define CAT5171_PIR_I2C_ADDRESS     (0x5A >> 1) // CAT5171
#define CAT5171_MIC_I2C_ADDRESS     (0x58 >> 1) // CAT5171
#define MS5611_I2C_ADDRESS          (0x52 >> 1) // MS5611
#define ICM20948_I2C_ADDRESS        (0x52 >> 1) // ICM20948
#define VEML6070_I2C_ADDRESS        (0x70 >> 1) // VEML6070
#define VCNL4040_I2C_ADDRESS        (0x60 >> 1) // VCNL4040
#define L3GD20_I2C_ADDRESS          (0xD4 >> 1) // GYRO (0xD6)
#define LIS3MDL_I2C_ADDRESS         (0x38 >> 1) // Compass (0x3C)
#define LSM9DS1_AG_I2C_ADDRESS      (0xD4 >> 1) // Compass (0xD6)
#define LSM9DS1_M_I2C_ADDRESS       (0x38 >> 1) // IMU (0x3C)
#define TT_I2C_ADDRESS              (0x77 >> 1) // I2C Test Device

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/
void hal_i2c_init(void );
void hal_i2c_on(UINT8 DevAddr);
void hal_i2c_off(UINT8 DevAddr);
void hal_i2c_suspend(UINT8 DevAddr);
void hal_i2c_resume(UINT8 DevAddr);

BOOL hal_i2c_read_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToRead, UINT8 * p_RegisterValue);
BOOL hal_i2c_write_register(UINT8 DeviceAddr, UINT8 RegisterAddr, UINT8 NumByteToWrite, UINT8 * p_RegisterValue);
BOOL hal_i2c_read_device(UINT8 DeviceAddr, UINT8 NumByteToRead, UINT8 * p_RegisterValue);
BOOL hal_i2c_write_device(UINT8 DeviceAddr, UINT8 NumByteToWrite, UINT8 * p_RegisterValue);
BOOL hal_i2c_read_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToRead, UINT8 * p_RAM_addr);
BOOL hal_i2c_write_memory(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToWrite, UINT8 * p_RAM_addr);
BOOL hal_i2c_read_uid(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToRead, UINT8 * p_RAM_addr);
BOOL hal_i2c_write_uid(UINT8 DeviceAddr, UINT16 EEPROM_addr, UINT8 NumByteToWrite, UINT8 * p_RAM_addr);
BOOL hal_i2c_wait_memory_ready(UINT8 DeviceAddr);

/*---- Function declarations------------------------------------------------------------*/

#endif

