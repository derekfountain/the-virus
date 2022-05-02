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
#include "main.h"
#include "levels_primitives.h"

typedef struct _level16_data
{
  uint8_t   state;
  uint8_t   phase_counter;
} LEVEL16_DATA;
void draw_level16_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL16_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 100 )
      return;

    l_data->phase_counter = 0;
    l_data->state = !l_data->state;

    if( l_data->state == 0 )
    {
      memset( 0x5800, PAPER_WHITE, (32*24) );
#include "level16_0.inc"
    }
    else
    {
#include "level16_1.inc"
    }

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL16_DATA *ld = (LEVEL16_DATA*)malloc( sizeof(LEVEL16_DATA) );
    level->level_data = ld;

    /* Set up to start in state 0 */
    ld->state = 1;
    ld->phase_counter = 100;
  }
}
