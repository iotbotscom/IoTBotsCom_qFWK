#ifndef IOTBOTSCOM_HW_CONFIG_ESP32_FEATHER_H
#define IOTBOTSCOM_HW_CONFIG_ESP32_FEATHER_H
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


    Filename: iotbotscom_hw_config_esp32_feather.h

    General Description: ESP32 Adafruit Feather Board HW config file

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*---- Defines  ------------------------------------------------------------------------*/
#define HW_PRODUCT_VERSION  "00.00.01"
#define HW_PRODUCT_TYPE     "qFWK-ESP32-FEATHER"

// I2C
#define I2C_SDA_PIN         SDA
#define I2C_SCL_PIN         SCL

// UART
#define UART_RXD_PIN        16
#define UART_TXD_PIN        17
#define UART_RTS_PIN        27
#define UART_CTS_PIN        33

// SPI
//#define SPI_SCK_PIN         5
//#define SPI_MOSI_PIN        18
//#define SPI_MISO_PIN        19

// SFLASH SPI CS
#define SFLASH_CS_PIN       15

// Motion Sensor Interrupt Pin
#define XINT_PIN            33

// RTC Interrupt Pin
#define RTC_INT_PIN         36

// RED LED
#define RED_LED_PIN         13

// GREEN LED
#define GREEN_LED_PIN       12

// BLUE LED
#define BLUE_LED_PIN        4

// VEXT_ADC
#define VEXT_ADC_PIN        34

// VINT_ADC
#define VINT_ADC_PIN        35

// IN0_ADC
#define IN0_ADC_PIN         39

// IN1_ADC
#define IN1_ADC_PIN         32

// OUT0
#define OUT0_PIN            25

// OUT0
#define OUT1_PIN            26

// RESET CONTROL
#define RESET_CONTROL_PIN   26

// OWD PIN
#define OWD_PIN             14

// RS485 CONTROL PIN
#define RS485_CNTL_PIN      27

// RS485 RX PIN
#define RS485_RX_PIN        UART_RXD_PIN

// RS485 TX PIN
#define RS485_TX_PIN        UART_TXD_PIN

// IND LED
#define IND_PIN             BLUE_LED_PIN

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

