#include <sdCard.h>
#include "app_fatfs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "platform_support.h"
#include <rtcProcess.h>

FATFS FatFs; 	//Fatfs handle
FIL fil = { 0 }; 		//File handle
FRESULT fres; //Result after operations
int8_t initState = 0;

void fileCreate(void)
{
  uint8_t fileName[23] = { 0 };
  printFilenameString(fileName);
  f_open(&fil, fileName, FA_CREATE_ALWAYS | FA_WRITE);
}

void writeString(uint8_t* inputString, uint8_t bytesToWrite)
{
  uint8_t bytesWritten;

  f_write(&fil, inputString, bytesToWrite, &bytesWritten);
}

void fileClose(void)
{
  f_close(&fil);
}

void sdCardInit (void)
{


  //Open the file system
  HAL_Delay(1000);
  fres = f_mount(&FatFs, "", 1); //1=mount now
  if (fres != FR_OK)
  {
   printf("f_mount error (%i)\r\n", fres);
		//while(1);
  }

  //Let's get some statistics from the SD card
    DWORD free_clusters, free_sectors, total_sectors;

    FATFS* getFreeFs;

    fres = f_getfree("", &free_clusters, &getFreeFs);
    if (fres != FR_OK) {
  	printf("f_getfree error (%i)\r\n", fres);
  	//while(1);
    }

    //Formula comes from ChaN's documentation
    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors = free_clusters * getFreeFs->csize;

    printf("\r\n SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

}

