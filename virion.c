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
#include <arch/zx.h>
#include "virion.h"
#include "int.h"
#include "swarm.h"
#include "rtunes.h"

/*
 * Line starts is the display file address. column 0.
 * Line offsets is the byte (0-31) along the display file line.
 * Byte values is the value to write into the byte to set the bit
 */
uint8_t *screen_line_starts[192];
uint8_t  screen_line_offsets[256];
uint8_t  screen_byte_values[256];

/*
 * Initialise the lookup tables. Called once, at the start.
 */
void init_draw_virion_tables(void)
{
  uint16_t i;
  for(i=0;i<192;i++)
    screen_line_starts[i] = zx_pxy2saddr( 0, i );

  for(i=0;i<256;i++)
  {
    /* Tried merging these into one table, but the multiply-by-2 on lookup cost too much */
    screen_line_offsets[i] = i >> 3;
    screen_byte_values[i]  = 0x80 >> (i & 0x07);
  }
}


void clear_virion( VIRION *v )
{
  if( ! v->active )
    return;

  if( v->previous_x_i < 0 || v->previous_x_i > 255
      ||
      v->previous_y_i < 0 || v->previous_y_i > 191 )
    return;

  uint8_t x = v->previous_x_i;
  uint8_t y = v->previous_y_i;

  /* I could cache this screen address... */
  uint8_t *scr_byte = screen_line_starts[y];
  scr_byte += screen_line_offsets[x];

  /*
   * I can't XOR this because if two virions are in the same place
   * they go off and back on again and I end up with trails of dots.
   */
  *scr_byte &= ~screen_byte_values[x];
}


void activate_virion( VIRION *v )
{
  if( !v->active )
  {
    v->active = 1;
    SET_SWARM_SIZE( GET_ACTIVE_SWARM_SIZE + 1 );
    change_immunity( v, MAKE_IMMUNE );
  }
}

void deactivate_virion( VIRION *v )
{
  if( v->active )
  {
    v->active = 0;
    SET_SWARM_SIZE( GET_ACTIVE_SWARM_SIZE - 1 );
  }
}


void draw_virion( VIRION *v )
{
  if( ! v->active )
    return;

  if( v->x_i < 0 || v->x_i > 255
      ||
      v->y_i < 0 || v->y_i > 191 )
    return;

#if 1
  uint8_t x = v->x_i;
  uint8_t y = v->y_i;
  rtunes_pixel( x, y, 1 );
#else
  uint8_t x = v->x_i;
  uint8_t y = v->y_i;

  uint8_t *scr_byte = screen_line_starts[y];
  scr_byte += screen_line_offsets[x];

  *scr_byte |= screen_byte_values[x];
#endif
}


/*
 * Random reappear code, called on blue blocks and when a virion is reactivated
 * via a green block. Put it somewhere other than a black block.
 */
void random_reappear_virion( VIRION *v )
{
  v->velocity_x = 0;
  v->velocity_y = 0;

  v->previous_x_i = -1;
  v->previous_y_i = -1;

  /* If the virion transports onto a black block it gets stuck in it, so don't allow that */
  do
  {
    v->x_i = rand()&255;
    v->y_i = rand()&191;
  }
  while( *(zx_pxy2aaddr(v->x_i,v->y_i)) == PAPER_BLACK );

  change_immunity( v, MAKE_IMMUNE );

  return;
}


void change_immunity( VIRION *v, uint8_t new_state )
{
  if( new_state == MAKE_IMMUNE )
    v->immunity_start = GET_TICKER;
  else
    v->immunity_start = 0;
}

