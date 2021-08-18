#ifndef IOTBOTSCOM_HW_CONFIG_H
#define IOTBOTSCOM_HW_CONFIG_H
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


    Filename: iotbotscom_hw_config.h

    General Description: qFWK global defs : Global HW Config File

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/
/* FrameWork includes. */
#include "ghdr/iotbotscom_product.h"
#include "ghdr/iotbotscom_platform.h"


// HW Common part


// FW Common part


// Product dependent part
#if defined(ARDUINO_SAMD_MKRZERO)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKR1000)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRZERO)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRFox1200)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRWAN1300)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRWAN1310)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRGSM1400)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_SAMD_MKRNB1500)
 #include "avr/iotbotscom_hw_config_avr.h"
#elif defined(ARDUINO_ESP8266_ESP12)
 #include "esp/iotbotscom_hw_config_esp.h"
#elif defined(ARDUINO_FEATHER_ESP32)
 #include "esp32/iotbotscom_hw_config_esp32_feather.h"
#elif defined(ARDUINO_QBOARDA_ESP32)
 #include "esp32/iotbotscom_hw_config_esp32_qboarda.h"
#elif defined(ARDUINO_QBOARDB_ESP32)
 #include "esp32/iotbotscom_hw_config_esp32_qboardb.h"
#elif defined(ARDUINO_QBOARDX_ESP32)
 #include "esp32/iotbotscom_hw_config_esp32_qboardx.h"
#else
 #error "HW config : NO BOARD_ID defined!!!"
#endif

/*---- Defines  ------------------------------------------------------------------------*/

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

