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
  uint8_t *att_addr = (uint8_t*)0x5AE0; //zx_cxy2aaddr(0, 23);

  /* Draw the timer with the new countdown value */
  uint8_t i=0;
  for( i=0; i<countdown; i++ )
  {
    *att_addr++ = (PAPER_CYAN|BRIGHT);
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

void time_up( void )
{
  zx_cls( PAPER_WHITE );

  roll_str(11,"   Time up! The virus wins!     ");
}

