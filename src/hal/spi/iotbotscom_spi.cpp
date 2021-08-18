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


    Filename: iotbotscom_spi.cpp

    General Description: spi driver

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
#include <SPI.h>
#include "iotbotscom_spi.h"

/* Logging*/
#include "../../drv/logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/

/*---- Typedefs ------------------------------------------------------------------------*/
typedef struct _SPI_DEVICE_T
{
    UINT16 init_check;
    UINT8 state;
    BOOL xMutex;
}SPI_DEVICE_T;

/*---- Variables -----------------------------------------------------------------------*/
SPI_DEVICE_T spi_device = {0};

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
void hal_spi_init(void )
{
    if(spi_device.init_check != 1234)
    {
        spi_device.init_check = 1234;

        spi_device.xMutex = FALSE;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_spi_on(UINT8 Dev)
{
    if(spi_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        spi_device.xMutex = TRUE;
    }

    if(Dev == MS5611_SPI_DEVICE || Dev == SFLASH_SPI_DEVICE)
    {
        if(spi_device.state == 0)
        {
            // Some Common things to do
#if ((defined PLATFORM_STM32) && (PLATFORM_STM32 == TRUE))
#if 1
            SPI.begin();

            SPI.setMISO(SYSTEM_SPI_MISO_PIN);
            SPI.setMOSI(SYSTEM_SPI_MOSI_PIN);
            SPI.setSCLK(SYSTEM_SPI_SCK_PIN);

            SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
#else
            mySPI.begin();

            mySPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
#endif

#else
            SPI.begin();

            SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
#endif
        }

        if(Dev == MS5611_SPI_DEVICE && (!(spi_device.state & MS5611_SPI_DEVICE)))
        {
            spi_device.state |= MS5611_SPI_DEVICE;
        }

        if(Dev == SFLASH_SPI_DEVICE && (!(spi_device.state & SFLASH_SPI_DEVICE)))
        {
            spi_device.state |= SFLASH_SPI_DEVICE;
        }
    }

    spi_device.xMutex = FALSE;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_spi_off(UINT8 Dev)
{
    if(spi_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        spi_device.xMutex = TRUE;
    }

    if(Dev == MS5611_SPI_DEVICE || Dev == SFLASH_SPI_DEVICE)
    {
        if(Dev == MS5611_SPI_DEVICE)
        {
            spi_device.state &= ~MS5611_SPI_DEVICE;
        }

        if(Dev == SFLASH_SPI_DEVICE)
        {
            spi_device.state &= ~SFLASH_SPI_DEVICE;
        }

        if(spi_device.state == 0)
        {
#if 0//((defined PLATFORM_STM32) && (PLATFORM_STM32 == TRUE))
            mySPI.endTransaction();

            mySPI.end();
#else
            SPI.endTransaction();

            SPI.end();
#endif
        }
    }

    spi_device.xMutex = FALSE;

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_spi_suspend(UINT8 Dev)
{
    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL hal_spi_resume(UINT8 Dev)
{
    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
UINT8 hal_spi_read_byte(UINT8 Dev, UINT8 dummy_byte)
{
    if(spi_device.state & Dev)
    {
#if 0//((defined PLATFORM_STM32) && (PLATFORM_STM32 == TRUE))
        return mySPI.transfer(dummy_byte);
#else
        return SPI.transfer(dummy_byte);
#endif
    }
    else
    {
        return 0xcc;
    }
}

/*--------------------------------------------------------------------------------------*/
UINT8 hal_spi_write_byte(UINT8 Dev, UINT8 byte)
{
    if(spi_device.state & Dev)
    {
#if 0//((defined PLATFORM_STM32) && (PLATFORM_STM32 == TRUE))
        return mySPI.transfer(byte);
#else
        return SPI.transfer(byte);
#endif
    }
    else
    {
        return 0xcc;
    }
}

