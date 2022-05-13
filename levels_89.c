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

#include <stdint.h>
#include <stdlib.h>
#include <arch/zx.h>
#include "main.h"
#include "levels_primitives.h"

typedef struct _level8_data
{
  uint8_t x;
} LEVEL8_DATA;
void draw_level8_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL8_DATA *l_data = level->level_data;

    *(zx_cxy2aaddr(l_data->x,11)) = PAPER_WHITE;
    if( l_data->x++ == 28 )
      l_data->x = 2;
    *(zx_cxy2aaddr(l_data->x,11)) = (PAPER_RED|BRIGHT);
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL8_DATA *ld = (LEVEL8_DATA*)malloc( sizeof(LEVEL8_DATA) );
    level->level_data = ld;

    ld->x = 2;
    *(zx_cxy2aaddr(ld->x,11)) = (PAPER_RED|BRIGHT);
  }
}

typedef struct _level9_data
{
  uint8_t   x;
  uint8_t   y;
  DIRECTION d;
} LEVEL9_DATA;
void draw_level9_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL9_DATA *l_data = level->level_data;

    _2x2( l_data->x, l_data->y, PAPER_WHITE );

    if( l_data->d == DIRECTION_E )
    {
      if( l_data->x++ == 27 )
        l_data->d = DIRECTION_S;
    }
    else if( l_data->d == DIRECTION_S )
    {
      if( l_data->y++ == 18 )
        l_data->d = DIRECTION_W;
    }
    else if( l_data->d == DIRECTION_W )
    {
      if( l_data->x-- == 3 )
        l_data->d = DIRECTION_N;
    }
    else if( l_data->d == DIRECTION_N )
    {
      if( l_data->y-- == 3 )
        l_data->d = DIRECTION_E;
    }

    _2x2( l_data->x, l_data->y, (PAPER_RED|BRIGHT) );

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL9_DATA *ld = (LEVEL9_DATA*)malloc( sizeof(LEVEL9_DATA) );
    level->level_data = ld;

    ld->x = 2;
    ld->y = 2;
    ld->d = DIRECTION_E;
    _2x2( ld->x, ld->y, (PAPER_RED|BRIGHT) );
  }
}
