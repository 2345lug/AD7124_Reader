/*!
 *****************************************************************************
  @file:  ad7124_console_app.c

  @brief: Implementation for the menu functions that handle the AD7124

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

/* includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "platform_support.h"
#include "spi.h"
#include "error.h"

#include "ad7124.h"
#include "ad7124_regs.h"
#include "ad7124_support.h"
#include "ad7124_regs_configs.h"

#include "ad7124_console_app.h"


/*  defines */
#define AD7124_CHANNEL_COUNT 16

#define SHOW_ALL_CHANNELS     false
#define SHOW_ENABLED_CHANNELS  true

#define DISPLAY_DATA_TABULAR    0
#define DISPLAY_DATA_STREAM     1

/* Private Variables */
/*
 * This is the 'live' AD7124 register map that is used by the driver
 * the other 'default' configs are used to populate this at init time
 */
static struct ad7124_st_reg ad7124_register_map[AD7124_REG_NO];

// Pointer to the struct representing the AD7124 device
static struct ad7124_dev * pAd7124_dev = NULL;

// Last Sampled values for All ADC channels
static uint32_t channel_samples[AD7124_CHANNEL_COUNT] = {0};
// How many times a given channel is sampled in total for one sample run
static uint32_t channel_samples_count[AD7124_CHANNEL_COUNT] = {0};


// Public Functions

/*!
 * @brief      Initialize the AD7124 device and the SPI port as required
 *
 * @details    This resets and then writes the default register map value to
 *  		   the device.  A call to init the SPI port is made, but may not
 *  		   actually do very much, depending on the platform
 */
int32_t ad7124_app_initialize(uint8_t configID)
{
	/*
	 * Copy one of the default/user configs to the live register memory map
	 * Requirement, not checked here, is that all the configs are the same size
	 */
	switch(configID) {
		case AD7124_CONFIG_A:
		{
			memcpy(ad7124_register_map, ad7124_regs_config_a, sizeof(ad7124_register_map));
			break;
		}
		case AD7124_CONFIG_B:
		{
			memcpy(ad7124_register_map, ad7124_regs_config_b, sizeof(ad7124_register_map));
			break;
		}
		default:
			// Not a defined configID
			return(FAILURE);
	}

	// Used to create the ad7124 device
    struct	ad7124_init_param sAd7124_init =
  	{
  		// spi_init_param type
  		{
  			2500000, 		// Max SPI Speed
  			0,				// Chip Select
			SPI_MODE_3,		// CPOL = 1, CPHA =1
			NULL
  		},
  		ad7124_register_map,

  		10000				// Retry count for polling
  	};

  return(ad7124_setup(&pAd7124_dev, sAd7124_init));
}

// Private Functions

/*!
 * @brief      determines if the Escape key was pressed
 *
 * @details
 */
static bool was_escape_key_pressed(void)
{
	char rxChar;
	bool wasPressed = false;

	// Check for Escape key pressed
	if ((rxChar = getchar_nonblocking()) > 0) {
		if (rxChar == ESCAPE_KEY_CODE) {
			wasPressed = true;
		}
	}
	return (wasPressed);
}


/*!
 * @brief      reads and displays the status register on the AD7124
 *
 * @details
 */
static void read_status_register(void)
{
	if (ad7124_read_register(pAd7124_dev, &ad7124_register_map[AD7124_Status]) < 0) {
	   printf("\r\nError Encountered reading Status register\r\n");
	} else {
	    uint32_t status_value = (uint32_t)ad7124_register_map[AD7124_Status].value;
        printf("\r\nRead Status Register = 0x%02lx\r\n", status_value);
	}
}


/*!
 * @brief      displays the current sample value for a ADC channels
 *
 * @details
 *
 * @param showOnlyEnabledChannels  only channels that are enabled are displayed
 *
 */
static void dislay_channel_samples(bool showOnlyEnabledChannels, uint8_t console_mode)
{
	switch(console_mode) {
		case DISPLAY_DATA_TABULAR:
		{
			printf("\tCh\tValue\t\tCount\t\tVoltage\r\n");
			for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
				// if showing all channels, or channel is enabled
				if ((showOnlyEnabledChannels == false) || (ad7124_register_map[AD7124_Channel_0 + i].value & AD7124_CH_MAP_REG_CH_ENABLE) ) {
				   printf("\t%-2d\t%-10ld\t%ld\t\t% .6f\r\n", \
							i, channel_samples[i], channel_samples_count[i],
							ad7124_convert_sample_to_voltage(pAd7124_dev, i, channel_samples[i]) );
				}
			}
			break;
		}
		case DISPLAY_DATA_STREAM:
		{
			// Output a CSV list of the sampled channels as voltages on a single line
			bool channel_printed = false;

			for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
				// if showing all channels, or channel is enabled
				if ((showOnlyEnabledChannels == false) || (ad7124_register_map[AD7124_Channel_0 + i].value & AD7124_CH_MAP_REG_CH_ENABLE) ) {
					/*
					 *  add the comma before we output the next channel but
					 *  only if at least one channel has been printed
					 */
				   if (channel_printed) {
					   printf(", ");
				   }
					printf("%.6f", \
							ad7124_convert_sample_to_voltage(pAd7124_dev, i, channel_samples[i]) );
					channel_printed = true;
				}
			}
			printf("\r\n");
			break;
		}
		default:
		{
			// ASSERT(false);
		}
	}
}


/*!
 * @brief      resets the channelSampleCounts to zero
 *
 * @details
 */
static void clear_channel_samples(void)
{
    for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
    	channel_samples[i] = 0;
    	channel_samples_count[i] = 0;
    }
}


/*!
 * @brief      Continuously acquires samples in Continuous Conversion mode
 *
 * @details   The ADC is run in continuous mode, and all samples are acquired
 *            and assigned to the channel they come from. Escape key an be used
 *            to exit the loop
 */
static int32_t do_continuous_conversion(uint8_t display_mode)
{
	int32_t error_code;
	int32_t sample_data;

	// Clear the ADC CTRL MODE bits, has the effect of selecting continuous mode
    ad7124_register_map[AD7124_ADC_Control].value &= ~(AD7124_ADC_CTRL_REG_MODE(0xf));
	if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_ADC_Control]) ) < 0) {
		printf("Error (%ld) setting AD7124 Continuous conversion mode.\r\n", error_code);
		adi_press_any_key_to_continue();
		return(MENU_CONTINUE);
	}

	clear_channel_samples();

	/*
	 *  If displaying data in stream form, want to output a channel header
	 */
	if (display_mode == DISPLAY_DATA_STREAM)
	{
		bool channel_printed = false;

		for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
		   // if showing all channels, or channel is enabled
			if (ad7124_register_map[AD7124_Channel_0 + i].value & AD7124_CH_MAP_REG_CH_ENABLE) {
				/*
				 *  add the comma before we output the next channel but
				 *  only if at least one channel has been printed
				 */
			   if (channel_printed) {
				   printf(", ");
			   }
				printf("%d", i);
			}
			channel_printed = true;
		}
		printf("\r\n");
	}

	// Continuously read the channels, and store sample values
    while (was_escape_key_pressed() != true) {
    	toggle_activity_led();

    	if (display_mode == DISPLAY_DATA_TABULAR) {
    		adi_clear_console();
    		printf("Running continuous conversion mode...\r\nPress Escape to stop\r\n\r\n");
    	}

		/*
		 *  this polls the status register READY/ bit to determine when conversion is done
		 *  this also ensures the STATUS register value is up to date and contains the
		 *  channel that was sampled as well.
		 *  Generally, no need to read STATUS separately, but for faster sampling
		 *  enabling the DATA_STATUS bit means that status is appended to ADC data read
		 *  so the channel being sampled is read back (and updated) as part of the same frame
		 */

    	if ( (error_code = ad7124_wait_for_conv_ready(pAd7124_dev, 10000)) < 0) {
    		printf("Error/Timeout waiting for conversion ready %ld\r\n", error_code);
    		continue;
    	}

    	if ( (error_code = ad7124_read_data(pAd7124_dev, &sample_data)) < 0) {
			printf("Error reading ADC Data (%ld).\r\n", error_code);
			continue;
		}

		/*
		 * No error, need to process the sample, what channel has been read? update that channelSample
		 */
		uint8_t channel_read = ad7124_register_map[AD7124_Status].value & 0x0000000F;

		if (channel_read < AD7124_CHANNEL_COUNT) {
			channel_samples[channel_read] = sample_data;
			channel_samples_count[channel_read]++;
		} else {
			printf("Channel Read was %d, which is not < AD7124_CHANNEL_COUNT\r\n", channel_read);
		}

		dislay_channel_samples(SHOW_ENABLED_CHANNELS, display_mode);
    }

    // All done, ADC put into standby mode
    ad7124_register_map[AD7124_ADC_Control].value &= ~(AD7124_ADC_CTRL_REG_MODE(0xf));
    // 2 = sleep/standby mode
    ad7124_register_map[AD7124_ADC_Control].value |= AD7124_ADC_CTRL_REG_MODE(2);

	if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_ADC_Control]) ) < 0) {
		printf("Error (%ld) setting AD7124 ADC into standby mode.\r\n", error_code);
		adi_press_any_key_to_continue();
	}

	return(MENU_CONTINUE);
}


/*!
 * @brief      Samples all enabled channels and displays in tabular form
 *
 * @details
 */
static int32_t menu_continuous_conversion_tabular(void)
{
	do_continuous_conversion(DISPLAY_DATA_TABULAR);

	adi_clear_console();
	printf("Continuous Conversion completed...\r\n\r\n");
	dislay_channel_samples(SHOW_ALL_CHANNELS, DISPLAY_DATA_TABULAR);
	adi_press_any_key_to_continue();

	return(MENU_CONTINUE);
}


/*!
 * @brief      Samples all enabled channels and displays on the console
 *
 * @details
 */
static int32_t menu_continuous_conversion_stream(void)
{
	do_continuous_conversion(DISPLAY_DATA_STREAM);
	printf("Continuous Conversion completed...\r\n\r\n");
	adi_press_any_key_to_continue();
	return(MENU_CONTINUE);
}


/*!
 * @brief      Samples all enabled channels once in Single Conversion mode
 *
 * @details    This stores all channels that are enabled in a bitmask, and then
 *             runs the ADC in single conversion mode, which acquires one channel
 *             of data at a time. After capture, that channel is disabled, and
 *             single conversion run again, until no channels are enabled.
 *             The original enable state of each channel is then restored.
 */
static int32_t menu_single_conversion(void)
{
	int32_t    error_code;
	uint16_t   channel_enable_mask = 0;
	uint8_t    channel_count = 0;
	int32_t    sample_data;

	// Need to store which channels are enabled in this config so it can be restored
	for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
		if (ad7124_register_map[AD7124_Channel_0 + i].value & AD7124_CH_MAP_REG_CH_ENABLE) {
			channel_enable_mask |= (1 << i);
			channel_count++;
		}
	}

	clear_channel_samples();
	adi_clear_console();
	printf("Running Single conversion mode...\r\nPress Escape to stop\r\n\r\n");

	// Clear the ADC CTRL MODE bits, selecting continuous mode
    ad7124_register_map[AD7124_ADC_Control].value &= ~(AD7124_ADC_CTRL_REG_MODE(0xf));

	// read the channels, and store sample values
    for (uint8_t loopCount = 0; ((was_escape_key_pressed()!= true ) && (loopCount < channel_count)) ; loopCount++) {
    	toggle_activity_led();

    	// 1 = single conversion mode
        ad7124_register_map[AD7124_ADC_Control].value |= AD7124_ADC_CTRL_REG_MODE(1);

    	if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_ADC_Control]) ) < 0) {
    		printf("Error (%ld) setting AD7124 Single conversion mode.\r\n", error_code);
    		adi_press_any_key_to_continue();
    		continue;
    	}

        /*
         *  this polls the status register READY/ bit to determine when conversion is done
         *  this also ensures the STATUS register value is up to date and contains the
         *  channel that was sampled as well. No need to read STATUS separately
         */
    	if ( (error_code = ad7124_wait_for_conv_ready(pAd7124_dev, 10000)) < 0) {
    		printf("Error/Timeout waiting for conversion ready %ld\r\n", error_code);
    		continue;
    	}

    	if ( (error_code = ad7124_read_data(pAd7124_dev, &sample_data)) < 0) {
			printf("Error reading ADC Data (%ld).\r\n", error_code);
			continue;
		}
    	/*
		 * No error, need to process the sample, what channel has been read? update that channelSample
		 */
		uint8_t channelRead = ad7124_register_map[AD7124_Status].value & 0x0000000F;

		if (channelRead < AD7124_CHANNEL_COUNT) {
			channel_samples[channelRead] = sample_data;
			channel_samples_count[channelRead]++;

			/* also need to clear the channel enable bit so the next single conversion cycle will sample the next channel */
			ad7124_register_map[AD7124_Channel_0 + channelRead].value &= ~AD7124_CH_MAP_REG_CH_ENABLE;
			if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_Channel_0 + channelRead]) ) < 0) {
				printf("Error (%ld) Clearing channel %d Enable bit.\r\n", error_code, channelRead);
				adi_press_any_key_to_continue();
				continue;
			}
		} else {
			printf("Channel Read was %d, which is not < AD7124_CHANNEL_COUNT\r\n", channelRead);
		}
    }

    // All done, ADC put into standby mode
    ad7124_register_map[AD7124_ADC_Control].value &= ~(AD7124_ADC_CTRL_REG_MODE(0xf));
    // 2 = sleep/standby mode
    ad7124_register_map[AD7124_ADC_Control].value |= AD7124_ADC_CTRL_REG_MODE(2);

	// Need to restore the channels that were disabled during acquisition
	for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
		if (channel_enable_mask & (1 << i)) {
			ad7124_register_map[AD7124_Channel_0 + i].value |= AD7124_CH_MAP_REG_CH_ENABLE;
	    	if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_Channel_0 + i]) ) < 0) {
	    		printf("Error (%ld) Setting channel %d Enable bit.\r\r\n", error_code, i);
	    		adi_press_any_key_to_continue();
	    		return(MENU_CONTINUE);
	    	}
		}
	}

	printf("Single Conversion completed...\r\n\r\n");
	dislay_channel_samples(SHOW_ENABLED_CHANNELS, DISPLAY_DATA_TABULAR);

	adi_press_any_key_to_continue();
	return(MENU_CONTINUE);
}


/*!
 * @brief      menu item that reads the status register the AD7124
 *
 * @details
 */
static int32_t menu_read_status(void)
{
	read_status_register();
	adi_press_any_key_to_continue();
	return(MENU_CONTINUE);
}


/*!
 * @brief      reads the ID register on the AD7124
 *
 * @details
 */
static int32_t menu_read_id(void)
{
	if (ad7124_read_register(pAd7124_dev, &ad7124_register_map[AD7124_ID]) < 0) {
	   printf("\r\nError Encountered reading ID register\r\n");
	} else {
	   printf("\r\nRead ID Register = 0x%02lx\r\n",
			   (uint32_t)ad7124_register_map[AD7124_ID].value );
	}
	adi_press_any_key_to_continue();
	return(MENU_CONTINUE);
}


/*!
 * @brief      Initialize the part with a specific configuration
 *
 * @details
 */
static void init_with_configuration(uint8_t configID)
{
	int32_t status = 0;

	do {
		if ((status = ad7124_remove(pAd7124_dev)) < 0) {
			break;
		}

		if ((status = ad7124_app_initialize(configID)) < 0) {
			break;
		}
	} while(0);

	if (status < 0) {
		printf("\r\n\r\n Error setting Configuration %c \r\n\r\n", (char)(configID + 'A'));
	} else {
		printf("\r\n\r\n Configuration %c Set\r\n\r\n", (char)(configID + 'A'));
	}
	adi_press_any_key_to_continue();
}


/*
 * @brief      Sends a reset command on the SPI to reset the AD7124
 *
 * @details
 */
static int32_t menu_reset(void)
{
	if (ad7124_reset(pAd7124_dev)  < 0)
	{
		printf("\r\n\r\n Error performing Reset\r\n\r\n");
	} else
	{
		// Need to set the live register map to defaults as well
		memcpy(ad7124_register_map, ad7124_regs, sizeof(ad7124_register_map));
		printf("\r\n\r\n Reset Complete\r\n\r\n");
	}
	adi_press_any_key_to_continue();
	return(MENU_CONTINUE);
}


/*!
 * @brief      Reset and set the ad7124 with configuration A
 *
 * @details
 */
static int32_t menu_reset_to_configuration_a(void)
{
	init_with_configuration(AD7124_CONFIG_A);
	return(MENU_CONTINUE);
}


/*!
 * @brief      Reset and the ad7124 with configuration B
 *
 * @details
 */
static int32_t menu_reset_to_configuration_b(void)
{
	init_with_configuration(AD7124_CONFIG_B);
	return(MENU_CONTINUE);
}


/*
 * Definition of the Sampling Menu Items and menu itself
 */
static console_menu_item acquisition_menu_items[] = {
		{"Single Conversion Mode",	                 	'S', menu_single_conversion},
	    {"Continuous Conversion Mode - Table View",	    'T', menu_continuous_conversion_tabular},
		{"Continuous Conversion Mode - Stream Data",	'C', menu_continuous_conversion_stream},
};

static console_menu acquisition_menu = {
    "Data Acquisition Menu",
	acquisition_menu_items,
	ARRAY_SIZE(acquisition_menu_items),
	true
};


/*!
 * @brief      displays and handles the Sample Channel menu
 *
 * @details
 */
static int32_t menu_sample_channels(void)
{
	return(adi_do_console_menu(&acquisition_menu));
}


/*
 * Definition of the Main Menu Items and menu itself
 */
console_menu_item main_menu_items[] = {
	{"Reset to Default Configuration",	'R', menu_reset},
	{"Reset to Configuration A",		'A', menu_reset_to_configuration_a},
    {"Reset to Configuration B",		'B', menu_reset_to_configuration_b},
	{"", 								'\00', NULL},
	{"Read ID Register",				'I', menu_read_id},
	{"Read Status Register",			'T', menu_read_status},
	{"", 								'\00', NULL},
    {"Sample Channels...",				'S', menu_sample_channels},
};

console_menu ad7124_main_menu = {
    "AD7124 Main Menu",
    main_menu_items,
	ARRAY_SIZE(main_menu_items),
	false
};
