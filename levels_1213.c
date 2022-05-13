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

typedef struct _level12_data
{
  uint8_t   state;
  uint8_t   phase_counter;
} LEVEL12_DATA;
void draw_level12_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL12_DATA *l_data = level->level_data;
    uint8_t block[][2] = { {4,7},{4,14},{4,11},{4,10},{255,255} };

    if( ++l_data->phase_counter < 25 )
      return;

    l_data->phase_counter = 0;
    l_data->state = !l_data->state;

    if( l_data->state == 0 )
      draw_cells( block, (PAPER_GREEN|BRIGHT) );
    else
      draw_cells( block, PAPER_WHITE );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL12_DATA *ld = (LEVEL12_DATA*)malloc( sizeof(LEVEL12_DATA) );
    level->level_data = ld;

    ld->state = 0;
    ld->phase_counter = 0;
#include "level12.inc"
  }
}

typedef struct _level13_data
{
  uint8_t   phase_counter;
} LEVEL13_DATA;
void draw_level13_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL13_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 30 )
      return;

    l_data->phase_counter = 0;

    swap_cells_colours( (PAPER_GREEN|INK_BLACK|BRIGHT), (PAPER_BLACK|INK_BLACK|BRIGHT) );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL13_DATA *ld = (LEVEL13_DATA*)malloc( sizeof(LEVEL13_DATA) );
    level->level_data = ld;

    ld->phase_counter = 0;
#include "level13.inc"
  }
}
