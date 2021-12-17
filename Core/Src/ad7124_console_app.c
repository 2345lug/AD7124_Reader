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


// Last Sampled values for All ADC channels
static uint32_t channel_samples[AD7124_CHANNEL_COUNT] = {0};
// How many times a given channel is sampled in total for one sample run
static uint32_t channel_samples_count[AD7124_CHANNEL_COUNT] = {0};

static float convertResistance (float inputSample);
static void toggleChannel(uint8_t lastChannel, struct ad7124_dev * inDev);
// Public Functions

/*!
 * @brief      Initialize the AD7124 device and the SPI port as required
 *
 * @details    This resets and then writes the default register map value to
 *  		   the device.  A call to init the SPI port is made, but may not
 *  		   actually do very much, depending on the platform
 */
int32_t ad7124_app_initialize(uint8_t configID, struct ad7124_dev * pAd7124_dev, uint8_t inPort, uint8_t inPin, uint32_t* pointerOutput)
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
			inPort,
			inPin,
			NULL
  		},
  		ad7124_register_map,

  		10000				// Retry count for polling
  	};

  int32_t setupResult = ad7124_setup(&pAd7124_dev, sAd7124_init);
  *pointerOutput = (pAd7124_dev);
  return(setupResult);



}

// Private Functions



/*!
 * @brief      reads and displays the status register on the AD7124
 *
 * @details

static void read_status_register(struct ad7124_dev * pAd7124_dev)
{
	if (ad7124_read_register(pAd7124_dev, &ad7124_register_map[AD7124_Status]) < 0) {
	   printf("\r\nError Encountered reading Status register\r\n");
	} else {
	    uint32_t status_value = (uint32_t)ad7124_register_map[AD7124_Status].value;
        printf("\r\nRead Status Register = 0x%02lx\r\n", status_value);
	}
}
*/
/*!
 * @brief      Continuously acquires samples in Continuous Conversion mode
 *
 * @details   The ADC is run in continuous mode, and all samples are acquired
 *            and assigned to the channel they come from. Escape key an be used
 *            to exit the loop
 */
int32_t do_continuous_conversion(uint8_t display_mode, struct ad7124_dev * pAd7124_dev, float* resultArrayPointer, uint8_t startPoint)
{
	int32_t error_code;
	int32_t sample_data;
	static uint32_t initialized[10] = { 0 };
	if (initialized[startPoint] == 0)
	{
	// Clear the ADC CTRL MODE bits, has the effect of selecting continuous mode
    ad7124_register_map[AD7124_ADC_Control].value &= ~(AD7124_ADC_CTRL_REG_MODE(0xf));
	if ( (error_code = ad7124_write_register(pAd7124_dev, ad7124_register_map[AD7124_ADC_Control]) ) < 0) {
		//printf("Error (%ld) setting AD7124 Continuous conversion mode.\r\n", error_code);
		//adi_press_any_key_to_continue();
		return(MENU_CONTINUE);
	}

	/*
	 *  If displaying data in stream form, want to output a channel header
	 */

	initialized[startPoint] = 1;
	}
	// Continuously read the channels, and store sample values


		/*
		 *  this polls the status register READY/ bit to determine when conversion is done
		 *  this also ensures the STATUS register value is up to date and contains the
		 *  channel that was sampled as well.
		 *  Generally, no need to read STATUS separately, but for faster sampling
		 *  enabling the DATA_STATUS bit means that status is appended to ADC data read
		 *  so the channel being sampled is read back (and updated) as part of the same frame
		 */

    	if ( (error_code = ad7124_wait_for_conv_ready(pAd7124_dev, 10000)) < 0) {
    		//printf("Error/Timeout waiting for conversion ready %ld\r\n", error_code);
    		//continue;
    	}

    	if ( (error_code = ad7124_read_data(pAd7124_dev, &sample_data)) < 0) {
			//printf("Error reading ADC Data (%ld).\r\n", error_code);
			//continue;
		}

		/*
		 * No error, need to process the sample, what channel has been read? update that channelSample
		 for (uint8_t i = 0; i < AD7124_CHANNEL_COUNT; i++) {
			channel_read = ad7124_register_map[AD7124_Status].value & 0x0000000F;
		}
		 */
		uint8_t channel_read = ad7124_register_map[AD7124_Status].value & 0x0000000F;

		if (channel_read < AD7124_CHANNEL_COUNT) {
			channel_samples[channel_read] = sample_data;
			channel_samples_count[channel_read]++;
		} else {
			printf("Channel Read was %d, which is not < AD7124_CHANNEL_COUNT\r\n", channel_read);
		}
		float convertedSample = 0;
	    float temperatureValue = 0;

		convertedSample = convertResistance(channel_samples[channel_read]);
	    *(resultArrayPointer + startPoint + channel_read) = convertedSample;
	    toggleChannel(channel_read, pAd7124_dev);


	return(MENU_CONTINUE);
}

static void toggleChannel(uint8_t lastChannel, struct ad7124_dev * inDev)
{
	uint32_t error_code = 0;

	if (lastChannel == 0)
	{
      ad7124_register_map[AD7124_IOCon1].value = 0b000000000010000010110100;
	  ad7124_register_map[AD7124_Channel_0].value = 0b0000000001100100;
	  ad7124_register_map[AD7124_Channel_1].value = 0b1000000011000111;
	}
	else
	{
      ad7124_register_map[AD7124_IOCon1].value = 0b000000000000010010110100;
	  ad7124_register_map[AD7124_Channel_0].value = 0b1000000001100100;
	  ad7124_register_map[AD7124_Channel_1].value = 0b0000000011000111;
	}


	if ( (error_code = ad7124_write_register(inDev, ad7124_register_map[AD7124_IOCon1])) < 0)
	{
      printf("Error (%ld) toggle channel.\r\n", error_code);
	}

	if ( (error_code = ad7124_write_register(inDev, ad7124_register_map[AD7124_Channel_0])) < 0)
	{
	  printf("Error (%ld) toggle channel.\r\n", error_code);
	}

	if ( (error_code = ad7124_write_register(inDev, ad7124_register_map[AD7124_Channel_1])) < 0)
	{
	  printf("Error (%ld) toggle channel.\r\n", error_code);
	}
}

static float convertResistance (float inputSample)
{
  static long zero = 1L << 23;
  static double referenceResistor = 2.49E3;
  static double gain = 1;
  double resistance = 0;
  double temperature = 0;
  resistance = ((inputSample - zero) * referenceResistor) / (zero * gain);
  //temperature = (resistance - 1000) / 3.85;
  temperature = -1 *((sqrt(-0.00232*resistance + 17.59246) - 3.908) / 0.00116);
  return (float)temperature;
}
