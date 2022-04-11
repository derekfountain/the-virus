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
#include <stdlib.h>
#include <string.h>
#include <arch/zx.h>
#include <z80.h>
#include "timer.h"
#include "print_str.h"

uint8_t paused;
uint8_t countdown;
uint8_t shown_countdown;

/* Return 1 when time's up */
boolean_t draw_timer( boolean_t force )
{
  if( countdown == shown_countdown && !force )
    return 0;

  /* Black line across the bottom */
  // uint8_t *line_addr = zx_cxy2saddr(0,23);
  memset( (void*)0x50E0, 255, 32 );

  /* First attribute of bottom line */
  register uint8_t *att_addr = (uint8_t*)0x5AE0; //zx_cxy2aaddr(0, 23);

  /* Draw the timer with the new countdown value */
  register uint8_t i=0;
  for( i=0; i<countdown; i++ )
  {
    uint8_t colour = PAPER_GREEN;
    if( i < 10 )
      colour = PAPER_RED;
    else if( i < 22 )
      colour = PAPER_YELLOW;

    *att_addr++ = colour;
  }
  for( ; i<32; i++ )
  {
    *att_addr = PAPER_WHITE;
  }

  shown_countdown = countdown;

  if( countdown == 0 )
    return 1;
  else
    return 0;
}

uint8_t tombstone[] = { 0b00011111, 0b11111000,
                        0b00100100, 0b00000100,
                        0b01001000, 0b00000010,
                        0b10010000, 0b00000001,
                        0b10010000, 0b00000001,
                        0b10010110, 0b01011001,
                        0b10010101, 0b01010101,
                        0b10010110, 0b01011001,
                        0b10010101, 0b01010001,
                        0b10010000, 0b00000001,
                        0b10010000, 0b00000001,
                        0b10010000, 0b00000001,
                        0b10010000, 0b00000001,
                        0b11111111, 0b11111111,
                        0b10010000, 0b00000001,
                        0b11111111, 0b11111111,
};

void print_graphic( uint8_t x, uint8_t y, uint16_t *graphic )
{
  uint8_t i;
  uint8_t *addr = zx_cxy2saddr(x, y);
  for(i=0; i<16; i++)
  {
    z80_wpoke( addr, *graphic );
    addr = zx_saddrpdown(addr);
    graphic++;
  }
}

void time_up( void )
{
  zx_cls( PAPER_WHITE );

  roll_str(6,"           Time up!             ");

  print_str(1, 7, "The virus takes over the world");
  z80_delay_ms(3000);
  print_str(5, 10, "Humanity is destroyed");
  z80_delay_ms(2000);
  print_str(9, 12, "and it's all");
  z80_delay_ms(1500);
  print_str(10, 14, "your");
  z80_delay_ms(1500);
  print_str(15, 14, "fault");
  z80_delay_ms(1500);

  uint16_t i;
  for(i=0; i<3000; i++)
  {
    print_graphic( (rand()%31), rand()%23, tombstone);
  }

  z80_delay_ms(3000);
}

