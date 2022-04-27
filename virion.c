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
#include "snow.h"

void clear_virion( VIRION *v )
{
  if( ! v->active )
    return;

  if( v->previous_x < 0 || v->previous_x > 255
      ||
      v->previous_y < 0 || v->previous_y > 183 )
    return;

  register uint8_t x = v->previous_x;
  register uint8_t y = v->previous_y;

  snow_unplot( x, y );
}


void activate_virion( VIRION *v )
{
  if( !v->active )
  {
    v->active = 1;
    SET_SWARM_SIZE( GET_ACTIVE_SWARM_SIZE + 1 );
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

  if( v->x < 0 || v->x > 255
      ||
      v->y < 0 || v->y > 183 )
    return;

  register uint8_t x = v->x;
  register uint8_t y = v->y;

  snow_plot( x, y );
}


/*
 * Random reappear code, called on blue blocks and when a virion is reactivated
 * via a green block. Put it somewhere other than a black block.
 */
void random_reappear_virion( VIRION *v )
{
  v->velocity_x = 0;
  v->velocity_y = 0;

  v->previous_x = -1;
  v->previous_y = -1;

  /* If the virion transports onto a black block it gets stuck in it, so don't allow that */
  do
  {
    v->x = rand()&255;
    v->y = rand()&183;
  }
  while( *(zx_pxy2aaddr(v->x,v->y)) == (PAPER_BLACK|BRIGHT) );

  return;
}
