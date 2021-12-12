/*!
 *****************************************************************************
  @file:  adi_console_menu.h

  @brief:	A simple console menu manager handler

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

#ifndef ADI_CONSOLE_MENU_H_
#define ADI_CONSOLE_MENU_H_

#include <stdbool.h>
#include <stdint.h>

#define MENU_ESCAPED			-1
#define MENU_CONTINUE           0
#define MENU_DONE               1

#define ESCAPE_KEY_CODE			(char)0x1B

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) ((sizeof (x)) / (sizeof ((x)[0])))
#endif

/* Type Definitions */
// Each menu item is defined by this struct
typedef struct {
	// String displayed for menu item
	char * displayText;
	// character that can be pressed to select menu item
	char  shortcutKey;
	// Function to be called when menu item is selected, if NULL, no function is called
	int32_t (*menuAction)(void);
} console_menu_item;

// This defines a complete menu with items
typedef struct{
	// String to be displayed as the menu title
	char * title;
	// Array of all the menu items
	console_menu_item * items;
	// Number of menuItems
	uint8_t itemCount;
	// Should the escape key to exit the menu be enabled?
	bool enableEscapeKey;
} console_menu;

/* Function Declarations */
/* Display a console menu, and handle user interactions */
int32_t adi_do_console_menu(const console_menu * menu);
void adi_clear_console(void);
void adi_press_any_key_to_continue(void);

#endif /* ADI_CONSOLE_MENU_H_ */
