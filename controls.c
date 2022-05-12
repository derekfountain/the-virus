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
#include <arch/zx.h>
#include <input.h>
#include <intrinsic.h>
#include "controls.h"
#include "print_str.h"

int16_t  up_key    = 'q';
int16_t  down_key  = 'a';
int16_t  left_key  = 'o';
int16_t  right_key = 'p';
int16_t  sound_key = 's';

uint16_t up_scancode    = IN_KEY_SCANCODE_q;
uint16_t down_scancode  = IN_KEY_SCANCODE_a;
uint16_t left_scancode  = IN_KEY_SCANCODE_o;
uint16_t right_scancode = IN_KEY_SCANCODE_p;
uint16_t sound_scancode = IN_KEY_SCANCODE_s;

uint8_t kp;

CONTROL select_controls( void )
{
  /* The keyboard responses are a bit sluggish with the ROM int running */
  intrinsic_di();

  zx_cls(PAPER_WHITE|INK_BLACK);

  print_str(3,10,"(K)eyboard or (J)oystick?");
  while(1)
  {
    int k_or_j = in_inkey();

    if( k_or_j == 'j' )
    {
      intrinsic_ei();
      return JOYSTICK;
    }
    else if( k_or_j == 'k' )
    {
      break;
    }
  }

  zx_cls(PAPER_WHITE|INK_BLACK);

  while( 1 )
  {
    uint8_t key_char[2];

    // Show current keys
    key_char[1] = 0;
    print_str(12, 6,"Up    "); key_char[0] = up_key-0x20;    print_str(19, 6,key_char);
    print_str(12, 8,"Down  "); key_char[0] = down_key-0x20;  print_str(19, 8,key_char);
    print_str(12,10,"Left  "); key_char[0] = left_key-0x20;  print_str(19,10,key_char);
    print_str(12,12,"Right "); key_char[0] = right_key-0x20; print_str(19,12,key_char);
    print_str(12,14,"Sound "); key_char[0] = sound_key-0x20; print_str(19,14,key_char);

    print_str(2,17,"(R)edefine, or ENTER to play");

    while( 1 )
    {
      int r_or_enter;
      r_or_enter = in_inkey();
      if( r_or_enter == 0x0D )
      {
        intrinsic_ei();
        return KEYBOARD;
      }
      else if( r_or_enter == 'r' )
      {
        in_wait_nokey();
        break;
      }
    }

    zx_cls(PAPER_WHITE|INK_BLACK);

    print_str(12, 6,"Up?   ");
    up_key = 0;
    in_wait_key(); up_key = in_inkey(); in_wait_nokey();
    key_char[0] = up_key-0x20;
    print_str(19, 6,key_char);

    print_str(12, 8,"Down? ");
    down_key = 0;
    while(1)
    {
      in_wait_key(); down_key = in_inkey(); in_wait_nokey();
      if( down_key != up_key )
        break;
    }
    key_char[0] = down_key-0x20;
    print_str(19, 8,key_char);

    print_str(12,10,"Left? ");
    left_key = 0;
    while(1)
    {
      in_wait_key(); left_key = in_inkey(); in_wait_nokey();
      if( left_key != up_key && left_key != down_key )
        break;
    }
    key_char[0] = left_key-0x20;
    print_str(19,10,key_char);

    print_str(12,12,"Right? ");
    right_key = 0;
    while(1)
    {
      in_wait_key(); right_key = in_inkey(); in_wait_nokey();
      if( right_key != up_key && right_key != down_key && right_key != left_key )
        break;
    }
    key_char[0] = right_key-0x20;
    print_str(19,12,key_char);

    print_str(12,14,"Sound? ");
    sound_key = 0;
    while(1)
    {
      in_wait_key(); sound_key = in_inkey(); in_wait_nokey();
      if( sound_key != up_key && sound_key != down_key && sound_key != left_key && sound_key != right_key )
        break;
    }
    key_char[0] = sound_key-0x20;
    print_str(19,14,key_char);

    up_scancode    = in_key_scancode(up_key);
    down_scancode  = in_key_scancode(down_key);
    left_scancode  = in_key_scancode(left_key);
    right_scancode = in_key_scancode(right_key);
    sound_scancode = in_key_scancode(sound_key);
  }
}

