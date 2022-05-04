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

    if( ++l_data->phase_counter < 30 )
      return;
    l_data->phase_counter = 0;

    uint8_t *att_addr = (uint8_t*)0x5800;
    while( att_addr < (uint8_t*)0x5B00 )
    {
      switch( *att_addr )
      {
        case PAPER_RED|BRIGHT:
          *att_addr = PAPER_GREEN|BRIGHT;
          break;
        case PAPER_BLUE|BRIGHT:
          *att_addr = PAPER_RED|BRIGHT;
          break;
        case PAPER_BLACK|BRIGHT:
          *att_addr = PAPER_BLUE|BRIGHT;
          break;
        case PAPER_GREEN|BRIGHT:
          *att_addr = PAPER_BLACK|BRIGHT;
          break;
      }
      att_addr++;
    }
    _2x2(15, 4,PAPER_GREEN|BRIGHT);
    _2x2(15,11,PAPER_GREEN|BRIGHT);
    _2x2(15,18,PAPER_GREEN|BRIGHT);

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL16_DATA *ld = (LEVEL16_DATA*)malloc( sizeof(LEVEL16_DATA) );
    level->level_data = ld;

    /* Set up to start in state 0 */
    ld->state = 1;
    ld->phase_counter = 0;
#include "level16.inc"
  }
}


typedef struct _level17_data
{
  uint8_t   state;
  uint8_t   phase_counter;
} LEVEL17_DATA;
void draw_level17_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL17_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 50 )
      return;
    l_data->phase_counter = 0;

    swap_cells_colours( PAPER_RED|BRIGHT, PAPER_GREEN|BRIGHT );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL17_DATA *ld = (LEVEL17_DATA*)malloc( sizeof(LEVEL17_DATA) );
    level->level_data = ld;

    /* Set up to start in state 0 */
    ld->state = 1;
    ld->phase_counter = 0;
#include "level17.inc"
    draw_box( 1, 1, 30, 21, PAPER_BLUE|BRIGHT );
  }
}
