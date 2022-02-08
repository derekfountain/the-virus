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
#include "level.h"
#include "levels.h"
#include "main.h"
#include "int.h"
#include "swarm.h"

uint8_t  current_frame;
uint16_t frames_before_change;

void init_level( LEVEL *level )
{
  zx_border( level->border_colour );

  SET_SWARM_SIZE( level->starting_num_virions );
  current_frame        = 0;
  frames_before_change = 0;
 
  (level->draw_frame)();

  return;
}

void apply_virion_logic( LEVEL *level, VIRION *v )
{
  if( ! v->active )
    return;

  if( v->x_i < 0 || v->x_i > 255
      ||
      v->y_i < 0 || v->y_i > 191 )
    return;

  uint8_t x = v->x_i;
  uint8_t y = v->y_i;

  uint8_t attribute = *(zx_pxy2aaddr(x,y));

  if( attribute == PAPER_RED )
  {
    /* If virion isn't immune deactivate it */
    if( !v->immunity_start )
    {
      /* Deactivate this one */
      deactivate_virion( v );

      /* For when/if it's reactivated */
      change_immunity( v, MAKE_NON_IMMUNE );
    }
  }
  else if( attribute == PAPER_GREEN )
  {
    /* If virion isn't immune, trigger an inactive one back to life */
    if( !v->immunity_start )
    {
      if( GET_ACTIVE_SWARM_SIZE < level->max_virions )
      {
	/* Active any currently inactive virion in the swarm, it starts off immune */
	activate_virion_in_swarm( START_IMMUNE );

	/*
	 * Virion which has triggered the new one becomes immune so it doesn't create lots
	 * while it's in the activation area. If it happens to wander into to a red zone
	 * then it'll be immune, which might be an issue. Depends if this is noticable
	 * during gameplay.
	 */
	change_immunity( v, MAKE_IMMUNE );
      }
    }
  }
  else if( attribute == PAPER_BLUE )
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
  }
  else if( attribute == PAPER_BLACK )
  {
    /*
     * I'd prefer a better "bounce" dynamic, but it's expensive to work out
     * where the virion has come from, how it's hit the black block (i.e.
     * which side of the cell) and hence which direction it should bounce
     * off in. This produces a reasonable approximation.
     */
    v->velocity_x = -(v->velocity_x);	
    v->velocity_y = -(v->velocity_y);	
    v->x_i        = v->previous_x_i;
    v->y_i        = v->previous_y_i;
  }
}
