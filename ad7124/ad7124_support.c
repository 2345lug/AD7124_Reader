/*!
 *****************************************************************************
  @file:  ad7124_support.c

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

#include <stdbool.h>
#include "ad7124_support.h"

// Public Functions
/*
 * @brief helper function get the setup setting for an ADC channel
 *
 * @param dev The device structure.
 *
 * @param channel ADC channel to get Setup for.
 *
 * @return value of setup field in channel configuration.
 */
uint8_t ad7124_get_channel_setup(struct ad7124_dev *dev, uint8_t channel)
{
	return (dev->regs[AD7124_Channel_0 + channel].value >> 12) & 0x7;
}


/*
 * @brief helper function get the PGA setting for an ADC channel
 *
 * @param dev The device structure.
 *
 * @param channel ADC channel to get Setup for.
 *
 * @return value of PGA field in the setup for an ADC channel.
 */
uint8_t ad7124_get_channel_pga(struct ad7124_dev *dev, uint8_t channel)
{
	uint8_t setup = ad7124_get_channel_setup(dev, channel);

	return (dev->regs[AD7124_Config_0 + setup].value) & 0x07;
}


/*
 * @brief helper function get the bipolar setting for an ADC channel
 *
 * @param dev The device structure.
 *
 * @param channel ADC channel to get bipolar mode for.
 *
 * @return value of bipolar field in the setup for an ADC channel.
 */
bool ad7124_get_channel_bipolar(struct ad7124_dev *dev, uint8_t channel)
{
	uint8_t setup = ad7124_get_channel_setup(dev, channel);

	return ((dev->regs[AD7124_Config_0 + setup].value >> 11) & 0x1) ? true : false;
}


/*
 * @brief converts ADC sample value to voltage based on gain setting
 *
 * @param dev The device structure.
 *
 * @param channel ADC channel to get Setup for.
 *
 * @param sample Raw ADC sample
 *
 * @return Sample ADC value converted to voltage.
 *
 * @note The conversion equation is implemented for simplicity,
 *       not for accuracy or performance
 *
 */
float ad7124_convert_sample_to_voltage(struct ad7124_dev *dev, uint8_t channel, uint32_t sample)
{
	bool isBipolar = ad7124_get_channel_bipolar(dev, channel);
	uint8_t channelPGA = ad7124_get_channel_pga(dev, channel);

	float convertedValue;

	if (isBipolar) {
        convertedValue = ( ((float)sample / (1 << (AD7124_ADC_N_BITS -1))) -1 ) * \
        		              (AD7124_REF_VOLTAGE / AD7124_PGA_GAIN(channelPGA));
	} else {
		convertedValue = ((float)sample * AD7124_REF_VOLTAGE)/(AD7124_PGA_GAIN(channelPGA) * \
								                              (1 << AD7124_ADC_N_BITS));
	}

    return (convertedValue);
}
