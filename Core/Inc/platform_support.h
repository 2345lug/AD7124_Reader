/*!
 *****************************************************************************
  @file:  platform_support.h

  @brief:  support functions and declarations for STM32 in general, and
           targeting a configuration of L476 device in particular

  @details:
 -----------------------------------------------------------------------------
Copyright (c) 2019 Analog Devices, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors/
    products manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
	which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its contributors
    may be used to endorse or promote products derived from this software without
    specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
	requirement that you obtain separate licenses from these patent holders
	to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

20180927-7CBSD SLA

*****************************************************************************/

#ifndef PLATFORM_SUPPORT_H_
#define PLATFORM_SUPPORT_H_

#include <stdio.h>

/*
 * the main.h file is required to either #include or contain the following
 * 	 Pin #define's for SPI, I2C, etc...
 * 	     - Typically these are generated using STM32CubeIDE or MX tools
 * 	 hspi1 SPI Port Handle variable definition
 * 	 huart2 for serial uart for printf/getchar() functions
 * 	 #include for the relevant stm32ynxx_hal.h
 * 	     - required to link with relevant HAL functions for chosen device
 */
#include "main.h"

// Function Prototypes
int __io_putchar(int ch);
int __io_getchar(void);

int16_t getchar_nonblocking();

void toggle_activity_led(void);

#endif /* PLATFORM_SUPPORT_H_ */
