/*!
 *****************************************************************************
  @file:  ad7124_support.h

  @brief: Provides useful support functions for the AD7124 NoOS driver

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
    may be used to endorse or promote products derived from this software
without specific prior written permission.
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

#ifndef AD7124_SUPPORT_H_
#define AD7124_SUPPORT_H_

#include "ad7124.h"

/* PGA Gain Value */
#define AD7124_PGA_GAIN(x) (1 << (x))

#define AD7124_REF_VOLTAGE 2.5
#define AD7124_ADC_N_BITS 24

uint8_t ad7124_get_channel_setup(struct ad7124_dev *dev, uint8_t channel);
uint8_t ad7124_get_channel_pga(struct ad7124_dev *dev, uint8_t channel);
bool ad7124_get_channel_bipolar(struct ad7124_dev *dev, uint8_t channel);
float ad7124_convert_sample_to_voltage(struct ad7124_dev *dev, uint8_t channel,
                                       uint32_t sample);

#endif /* AD7124_SUPPORT_H_ */
