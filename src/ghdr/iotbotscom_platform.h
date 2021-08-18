#ifndef IOTBOTSCOM_PLATFORM_H
#define IOTBOTSCOM_PLATFORM_H
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


    Filename: iotbotscom_platform.h

    General Description: qFWK global defs : HW Platform Defines

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/

/*---- Defines  ------------------------------------------------------------------------*/
// HW Platform definition
#define HW_PLATFORM_TYPE_NONE       0
#define HW_PLATFORM_TYPE_SAMD_MKR   1
#define HW_PLATFORM_TYPE_ESP        2
#define HW_PLATFORM_TYPE_ESP32      3
#define HW_PLATFORM_TYPE_STM32      4

#if defined(ARDUINO_SAMD_MKRZERO)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKR1000)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRZERO)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRFox1200)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRWAN1300)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRWAN1310)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRGSM1400)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_SAMD_MKRNB1500)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_SAMD_MKR
#elif defined(ARDUINO_ESP8266_ESP12)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_ESP
#elif defined(ARDUINO_FEATHER_ESP32)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_ESP32
#elif defined(ARDUINO_QBOARDA_ESP32)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_ESP32
#elif defined(ARDUINO_QBOARDB_ESP32)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_ESP32
#elif defined(ARDUINO_QBOARDX_ESP32)
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_ESP32
#else
 #define HW_PLATFORM_TYPE           HW_PLATFORM_TYPE_NONE
 #error "Plaform : NO Platform defined!!!"
#endif

/*---- Typedefs ------------------------------------------------------------------------*/

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

#endif

