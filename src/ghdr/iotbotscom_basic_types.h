#ifndef IOTBOTSCOM_BASIC_TYPES_H
#define IOTBOTSCOM_BASIC_TYPES_H
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


    Filename: iotbotscom_basic_types.h

    General Description: qFWK global defs : Basic Types

------------------------------------------------------------------------------------------
Revision History:
                    Modification     Tracking
Author                  Date          Number           Description of Changes
----------------    ------------    ----------   -----------------------------------------
iotbotscom           08/15/2021                  QWARKS FrameWork Initial Public Release

========================================================================================*/

/*---- Include files -------------------------------------------------------------------*/

/*---- Defines  ------------------------------------------------------------------------*/
#define SIZE(array) (sizeof(array) / sizeof((array)[0]))

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE (!FALSE)
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef MSB
    #define MSB 0x80
#endif

/*---- Typedefs ------------------------------------------------------------------------*/
typedef unsigned char       BOOL;

// Data
typedef unsigned char       BYTE;
typedef unsigned char       CHAR;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

// Unsigned numbers
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned long       UINT32;
typedef unsigned long long  UINT64;

// Signed numbers
typedef signed char         SINT8;
typedef signed short        SINT16;
typedef signed long         SINT32;
typedef signed long long    SINT64;

typedef struct _PBUF_T
{
    UINT16 len;
    UINT8 * buf;
}PBUF_T;

typedef enum _MODE_T
{
    MODE_NONE = 0,
    MODE_INIT,
    MODE_ON,
    MODE_POWERSAVE,
    MODE_OFF
}MODE_T;

typedef struct _TIME_T
{
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
}TIME_T;

typedef struct _DATE_T
{
    UINT8 day;
    UINT8 month;
    UINT8 year;
    UINT8 wday;
    BOOL dst;
}DATE_T;

typedef struct _TD_T
{
    DATE_T date;
    TIME_T time;
}TD_T;

/*---- Variables -----------------------------------------------------------------------*/

/*---- Function prototypes -------------------------------------------------------------*/

/*---- Function declarations------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/

#endif

