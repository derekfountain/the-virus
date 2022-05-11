/*
 * Shake the Virus, a ZX Spectrum game.
 * Copyright (C) 2022 Derek Fountain
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdint.h>
#include <input.h>
#include "controls.h"

uint16_t up_scancode    = IN_KEY_SCANCODE_q;
uint16_t down_scancode  = IN_KEY_SCANCODE_a;
uint16_t left_scancode  = IN_KEY_SCANCODE_o;
uint16_t right_scancode = IN_KEY_SCANCODE_p;

CONTROL select_controls( void )
{
  // Print Keyboard or Joystick
  // Wait for K or J
  // If J, return JOYSTICK
  // Show current keys
  // R to redefine or Enter to accept
  // If enter, return KEYBOARD
  // Key for Up? Wait and accept keypress. Then down, left, right
  // Return to "SHow current keys"

  return KEYBOARD;
  //return JOYSTICK;
}

