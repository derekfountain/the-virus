/*
 * The Virus, a ZX Spectrum game.
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

#include <arch/zx.h>
#include <intrinsic.h>
#include <string.h>
#include "print_str.h"

extern uint8_t font[];

void print_char( uint8_t *screen_addr, uint8_t c )
{
  uint8_t i;
  for(i=0;i<8;i++)
  {
    *screen_addr = font[((c-' ')*8)+i];
    screen_addr += 256;
  }
}

void print_str( uint8_t x, uint8_t y, uint8_t *str )
{
  uint8_t i;

  for( i=0; i<strlen(str); i++ )
  {
    uint8_t *addr = zx_cxy2saddr(x+i, y);
    print_char( addr, *(str+i) );
  }
}

void roll_str( uint8_t y, uint8_t *str )
{
  uint8_t scr_pos;
  uint8_t j;

  for( scr_pos=31; scr_pos>0; scr_pos-- )
  {
    uint8_t *iter_str = str;

    for(j=0;j<(31-scr_pos)+1;j++)
    {
      print_char( zx_cxy2saddr(scr_pos+j, y), *iter_str++ );
    }
  }

  for( j=0; j<75; j++ )
    intrinsic_halt();

  for( scr_pos=0; scr_pos<32; scr_pos++ )
  {
    print_char( zx_cxy2saddr(scr_pos, y), ' ' );
    intrinsic_halt();
  }
}

