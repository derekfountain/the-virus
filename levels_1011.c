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
#include "main.h"
#include "levels_primitives.h"

typedef struct _level10_data
{
  uint8_t   x;
  DIRECTION d;
} LEVEL10_DATA;
void draw_level10_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL10_DATA *l_data = level->level_data;

    _5x1( l_data->x, 11, PAPER_WHITE );

    if( l_data->d == DIRECTION_E )
      l_data->x++;
    else
      l_data->x--;

    if( l_data->x == 27 )
    {
      l_data->d = DIRECTION_W;
    }
    else if( l_data->x == 0 )
    {
      l_data->d = DIRECTION_E;
    }

    _5x1( l_data->x, 11, (PAPER_RED|BRIGHT) );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL10_DATA *ld = (LEVEL10_DATA*)malloc( sizeof(LEVEL10_DATA) );
    level->level_data = ld;

    ld->x = 0;
    ld->d = DIRECTION_E;
    _5x1( ld->x, 11, (PAPER_RED|BRIGHT) );

#include "level10.inc"
  }
}


typedef struct _level11_data
{
  uint8_t   state;
  uint8_t   phase_counter;
} LEVEL11_DATA;
void draw_level11_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL11_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 25 )
      return;

    l_data->phase_counter = 0;
    if( ++l_data->state == 3 )
      l_data->state = 0;

    if( l_data->state == 0 )
      _2x2( 15, 10, (PAPER_RED|BRIGHT) );
    else if( l_data->state == 1 )
      _2x2( 15, 10, (PAPER_GREEN|BRIGHT) );
    else
      _2x2( 15, 10, (PAPER_BLUE|BRIGHT) );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL11_DATA *ld = (LEVEL11_DATA*)malloc( sizeof(LEVEL11_DATA) );
    level->level_data = ld;

    ld->state = 0;
    ld->phase_counter = 0;
    _2x2( 15, 10, (PAPER_RED|BRIGHT) );
  }
}

