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


    Filename: iotbotscom_sflash.cpp

    General Description: qFWK Serial Flash driver

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
#include "iotbotscom_sflash.h"
#include "../../hal/spi/iotbotscom_spi.h"

/* Logging... */
#include "../logman/iotbotscom_logman.h"

/*---- Defines  ------------------------------------------------------------------------*/
#define SFLASH_PAGE_SIZE        256
#define SFLASH_BLOCK_SIZE       4096 // Subsector size
#define SFLASH_BLOCKS_NUMBER    (512 * 2)
#define SFLASH_PAGES_NUMBER     (8192 * 2)
#define SFLASH_SIZE             (UINT32)(SFLASH_BLOCK_SIZE * SFLASH_BLOCKS_NUMBER)
#define SFLASH_DATA_START       (UINT32)0x00000
#define SFLASH_DATA_END         (UINT32)(SFLASH_SIZE-1)

// sFlash Commands
#define sFLASH_CMD_WRITE        0x0A  /*!< Write to Memory instruction */
#define sFLASH_CMD_PROGRAM      0x02  /*!< Program to Memory instruction */
#define sFLASH_CMD_WRSR         0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN         0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ         0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR         0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID         0x9F  /*!< Read identification */
#define sFLASH_CMD_SE           0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE32         0x52  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE64         0xD8  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE           0xC7  /*!< Bulk Erase instruction */

#define sFLASH_WIP_FLAG         0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE       0xA5
#define sFLASH_SPI_PAGESIZE     0x100

#define sFLASH_CS_LOW()         digitalWrite(SFLASH_CS_PIN, LOW)
#define sFLASH_CS_HIGH()        digitalWrite(SFLASH_CS_PIN, HIGH)

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/
SFLASH_DEVICE_T sflash_device;
UINT8 checkbuf[256];

/*---- Function prototypes -------------------------------------------------------------*/
void sflash_init(void );
BOOL sflash_on(void );
BOOL sflash_off(void );
BOOL sflash_suspend(void );
BOOL sflash_resume(void );

BOOL sflash_read(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number);
BOOL sflash_write(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number);

UINT32 sflash_read_chip_id(void );
BOOL sflash_read_page(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number);
BOOL sflash_write_page(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number);
BOOL sflash_erase_bulk(void );
BOOL sflash_erase_sector(UINT32 u16_memAddr);
void sflash_write_enable(void);
void sflash_wait_for_write_end(void);

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Init(void )
{
    sflash_init();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_On(void )
{
    if(!(sflash_device.mode == MODE_INIT || sflash_device.mode == MODE_OFF ))
    {
        return FALSE;
    }

    return sflash_on();
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Off(void )
{
    if(sflash_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return sflash_off();
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Resume(void )
{
    if(sflash_device.mode != MODE_POWERSAVE)
    {
        return FALSE;
    }

    return sflash_resume();
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Suspend(void )
{
    if(sflash_device.mode != MODE_ON)
    {
        return FALSE;
    }

    return sflash_suspend();
}

/*--------------------------------------------------------------------------------------*/
MODE_T SFLASH_Get_Mode(void )
{
    return sflash_device.mode;
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Read(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    BOOL ret;

    if(sflash_device.mode != MODE_ON && sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }

    sflash_device.xMutex = TRUE;

    LOG_DEV("SFLASH_Read begin\r\n");

    ret = sflash_read(p_sFLASH_addr, p_RAM_addr, number);

    LOG_DEV("SFLASH_Read end\r\n");

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Write(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    BOOL ret;

    if(sflash_device.mode != MODE_ON && sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }

    sflash_device.xMutex = TRUE;

    LOG_DEV("SFLASH_Write begin\r\n");

    ret = sflash_write(p_sFLASH_addr, p_RAM_addr, number);

    LOG_DEV("SFLASH_Write end\r\n");

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL SFLASH_Bulk_Erasure(void )
{
    BOOL ret = TRUE;

    if(sflash_device.mode != MODE_ON && sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }

    sflash_device.xMutex = TRUE;

    LOG_DEV("SFLASH_Bulk_Erasure begin\r\n");

    sflash_erase_bulk();

    LOG_DEV("SFLASH_Bulk_Erasure end\r\n");

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
void sflash_init(void )
{
    if(sflash_device.init_check != 1234)
    {
        sflash_device.init_check = 1234;

        sflash_device.xMutex = FALSE;
#if 0
        pinMode(SPI_SCK_PIN, OUTPUT);
        digitalWrite(SPI_SCK_PIN, HIGH);

        pinMode(SPI_MOSI_PIN, OUTPUT);
        digitalWrite(SPI_MOSI_PIN, HIGH);

        pinMode(SPI_MISO_PIN, INPUT);
#endif
        pinMode(SFLASH_CS_PIN, OUTPUT);
        sFLASH_CS_HIGH();

        hal_spi_init();

        sflash_device.mode = MODE_INIT;
    }
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_on(void )
{
    UINT32 ID;
    BOOL ret = FALSE;

    if(sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        sflash_device.xMutex = TRUE;
    }

    if(hal_spi_on(SFLASH_SPI_DEVICE) == TRUE)
    {
        ID = sflash_read_chip_id();

        if(LOG_USR_CHECK(USR_LOG_SFLASH))
        {
            Serial.print("sFlash Chip ID : 0x");
            Serial.println(ID, HEX);
        }

        if(ID == 0x208014 || ID == 0x208015 || ID == 0x208016 || ID == 0xEF4018 || ID == 0xEF4017 || ID == 0xEF6017 || ID == 0xEF4016 || ID == 0xEF7018)
        {
            sflash_device.mode = MODE_ON;

            ret = TRUE;
        }
        else
        {
            hal_spi_off(SFLASH_SPI_DEVICE);
        }
    }

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_off(void )
{
    BOOL ret = FALSE;

    if(sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        sflash_device.xMutex = TRUE;
    }

    if(hal_spi_off(SFLASH_SPI_DEVICE) == TRUE)
    {
        sflash_device.mode = MODE_OFF;

        ret = TRUE;
    }

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_suspend(void )
{
    BOOL ret = FALSE;

    if(sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        sflash_device.xMutex = TRUE;
    }

    if(hal_spi_off(SFLASH_SPI_DEVICE) == TRUE)
    {
        sflash_device.mode = MODE_POWERSAVE;

        ret = TRUE;
    }

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_resume(void )
{
    BOOL ret = FALSE;

    if(sflash_device.xMutex != FALSE)
    {
        return FALSE;
    }
    else
    {
        sflash_device.xMutex = TRUE;
    }

    if(hal_spi_on(SFLASH_SPI_DEVICE) == TRUE)
    {
        sflash_device.mode = MODE_ON;

        ret = TRUE;
    }

    sflash_device.xMutex = FALSE;

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_read(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    BOOL ret = TRUE;
    UINT32 sFLASH_addr;
    UINT8 * RAM_addr;
    UINT32 page_begin, page_end;
    UINT16 bytes_number;
    BOOL is_last_transfer = FALSE;
    BOOL is_first_transfer = TRUE;
    UINT16 bytes_processed = 0;
    
    if(!(((p_sFLASH_addr + (UINT32)number) <= SFLASH_DATA_END) && number <= 256))
    {
        return FALSE;
    }

    if(LOG_USR_CHECK(USR_LOG_SFLASH))
    {
        Serial.print("RD : 0x");
        Serial.print((UINT32)p_sFLASH_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(number);
    }

    while(is_last_transfer != TRUE)
    {
        sFLASH_addr = p_sFLASH_addr + bytes_processed;
        RAM_addr = p_RAM_addr + bytes_processed;
        bytes_number = number - bytes_processed;
        
        page_begin = (sFLASH_addr / SFLASH_PAGE_SIZE);

        page_end = ((sFLASH_addr + bytes_number - 1) / SFLASH_PAGE_SIZE);

        if(page_begin == page_end)
        {
            is_last_transfer = TRUE;
        }
        else if(is_first_transfer == TRUE)
        {
            is_first_transfer = FALSE;
            bytes_number = (page_begin + 1) * SFLASH_PAGE_SIZE - sFLASH_addr;
        }
        else
        {
            bytes_number = SFLASH_PAGE_SIZE;
        }

        bytes_processed += bytes_number;

        if(sflash_read_page(sFLASH_addr, RAM_addr, bytes_number) != TRUE)
        {
            ret = FALSE;
            break;
        }
    }

    if(LOG_USR_CHECK(USR_LOG_SFLASH))
    {
        if(ret == TRUE)
        {
            Serial.println(" >> OK");
        }
        else
        {
            Serial.println(" >> Failed");
        }
    }

    return ret;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_write(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    BOOL ret = TRUE;
    UINT32 sFLASH_addr;
    UINT8 * RAM_addr;
    UINT32 page_begin, page_end;
    UINT16 bytes_number;
    BOOL is_last_transfer = FALSE;
    BOOL is_first_transfer = TRUE;
    UINT16 bytes_processed = 0;
    UINT16 i;
    BOOL block_erasure = FALSE;
    UINT32 sector_address;

    if(!(((p_sFLASH_addr + (UINT32)number) <= SFLASH_DATA_END) && number <= 256))
    {
        return FALSE;
    }

    if(LOG_USR_CHECK(USR_LOG_SFLASH))
    {
        Serial.print("WR : 0x");
        Serial.print((UINT32)p_sFLASH_addr, HEX);
        Serial.print(", ");
        Serial.print((UINT32)p_RAM_addr, HEX);
        Serial.print(", ");
        Serial.print(number);
    }

    sflash_read_page(p_sFLASH_addr, checkbuf, number);

    for(i = 0; i < number; i++)
    {
        if(checkbuf[i] != 0xff)
        {
            LOG_DEV("\r\nblock to be erasured\r\n");
            block_erasure = TRUE;
            break;
        }
    }

    if(block_erasure == TRUE)
    {
        page_begin = (p_sFLASH_addr / SFLASH_BLOCK_SIZE);
        page_end = ((p_sFLASH_addr + number - 1) / SFLASH_BLOCK_SIZE);

        if(page_begin != page_end || (p_sFLASH_addr % SFLASH_BLOCK_SIZE) == 0)
        {
            sector_address = (p_sFLASH_addr + (UINT32)number) / SFLASH_BLOCK_SIZE;
            sector_address *= SFLASH_BLOCK_SIZE;

            LOG_DEV("\r\nsflash_erase_sector begin\r\n");
            ret = sflash_erase_sector(sector_address);
            LOG_DEV("\r\nsflash_erase_sector end\r\n");
        }
    }

    if(ret == TRUE)
    {
        while(is_last_transfer != TRUE)
        {
            sFLASH_addr = p_sFLASH_addr + bytes_processed;
            RAM_addr = p_RAM_addr + bytes_processed;
            bytes_number = number - bytes_processed;
        
            page_begin = (sFLASH_addr / SFLASH_PAGE_SIZE);
        
            page_end = ((sFLASH_addr + bytes_number - 1) / SFLASH_PAGE_SIZE);
        
            if(page_begin == page_end)
            {
                is_last_transfer = TRUE;
            }
            else if(is_first_transfer == TRUE)
            {
                is_first_transfer = FALSE;
                bytes_number = (page_begin + 1) * SFLASH_PAGE_SIZE - sFLASH_addr;
            }
            else
            {
                bytes_number = SFLASH_PAGE_SIZE;
            }
        
            bytes_processed += bytes_number;

            if(sflash_write_page(sFLASH_addr, RAM_addr, bytes_number) != TRUE)
            {
                LOG_DEV("\r\nsflash_write_page failed\r\n");
                ret = FALSE;
                break;
            }
        }

        sflash_read_page(p_sFLASH_addr, checkbuf, number);

        for(i = 0; i < number; i++)
        {
            if(checkbuf[i] != *((UINT8 *)(p_RAM_addr + i)))
            {
                ret = FALSE;
                break;
            }
        }
    }

    if(LOG_USR_CHECK(USR_LOG_SFLASH))
    {
        if(ret == TRUE)
        {
            Serial.println(" >> OK");
        }
        else
        {
            Serial.println(" >> Failed");
        }
    }

    return ret;
}

/*--------------------------------------------------------------------------------------*/
UINT32 sflash_read_chip_id(void )
{
  UINT32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "RDID " instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, 0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_read_page(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    UINT8 * p = (UINT8 *)p_RAM_addr;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (p_sFLASH_addr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (p_sFLASH_addr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, p_sFLASH_addr & 0xFF);

  while (number--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *p = hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    p++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_write_page(UINT32 p_sFLASH_addr, UINT8 * p_RAM_addr, UINT16 number)
{
    UINT8 * p = (UINT8 *)p_RAM_addr;

  /*!< Enable the write access to the FLASH */
  sflash_write_enable();

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_PROGRAM);
  /*!< Send WriteAddr high nibble address byte to write to */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (p_sFLASH_addr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (p_sFLASH_addr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, p_sFLASH_addr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (number--)
  {
    /*!< Send the current byte */
    hal_spi_write_byte(SFLASH_SPI_DEVICE, *p);
    /*!< Point on the next byte to be written */
    p++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sflash_wait_for_write_end();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_erase_bulk(void )
{
  /*!< Send write enable instruction */
  sflash_write_enable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Bulk Erase instruction  */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_BE);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sflash_wait_for_write_end();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
BOOL sflash_erase_sector(UINT32 sector_address)
{
  /*!< Send write enable instruction */
  sflash_write_enable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (sector_address & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, (sector_address & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sector_address & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sflash_wait_for_write_end();

    return TRUE;
}

/*--------------------------------------------------------------------------------------*/
void sflash_write_enable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

/*--------------------------------------------------------------------------------------*/
void sflash_wait_for_write_end(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = hal_spi_write_byte(SFLASH_SPI_DEVICE, sFLASH_DUMMY_BYTE);

  }
  while (flashstatus & sFLASH_WIP_FLAG); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

