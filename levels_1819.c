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

typedef struct _level18_data
{
  uint8_t   state;
  uint8_t   phase_counter;
  uint8_t   head_x, head_y;
} LEVEL18_DATA;
void draw_level18_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL18_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 1 )
      return;
    l_data->phase_counter = 0;

    uint8_t i,x,y;
    x = l_data->head_x;
    y = l_data->head_y;
    for( i=0; i < 8; i++ )
    {
      *(zx_cxy2aaddr(x, y)) = PAPER_WHITE;

      if( y == 1 )
      {
        /* Top row */
        if( x == 1 )
          y=2;
        else
          x--;
      }
      else if( y == 21 )
      {
        /* Bottom row */
        if( x == 30 )
          y=20;
        else
          x++;
      }

      else if( x == 30 )
      {
        /* Right side */
        if( y == 1 )
          x--;
        else
          y--;
      }
      else if( x == 1 )
      {
        /* Left side */
        if( y == 21 )
          x++;
        else
          y++;
      }

    }
    *(zx_cxy2aaddr(x, y)) = PAPER_BLUE|BRIGHT;

    if( l_data->head_y == 1 )
    {
      /* Top row */
      if( l_data->head_x == 30 )
        l_data->head_y++;
      else
        l_data->head_x++;
    }
    else if( l_data->head_y == 21 )
    {
      /* Bottom row */
      if( l_data->head_x == 1 )
        l_data->head_y--;
      else
        l_data->head_x--;
    }

    else if( l_data->head_x == 30 )
    {
      /* Right side */
      if( y == 21 )
        l_data->head_x--;
      else
        l_data->head_y++;
    }
    else if( l_data->head_x == 1 )
    {
      /* Left side */
      if( y == 1 )
        l_data->head_x++;
      else
         l_data->head_y--;
    }
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL18_DATA *ld = (LEVEL18_DATA*)malloc( sizeof(LEVEL18_DATA) );
    level->level_data = ld;

    /* Set up to start in state 0 */
    ld->state = 1;
    ld->phase_counter = 200;
    ld->head_x = 10;
    ld->head_y = 1;
    draw_box( 0, 0, 31, 22, PAPER_RED|BRIGHT );
    draw_box( 1, 1, 30, 21, PAPER_BLUE|BRIGHT );
    *(zx_cxy2aaddr(15, 11)) = PAPER_GREEN|BRIGHT;
    *(zx_cxy2aaddr(16, 11)) = PAPER_GREEN|BRIGHT;
  }
}


typedef struct _level19_data
{
  uint8_t   state;
  uint8_t   phase_counter;
} LEVEL19_DATA;
void draw_level19_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL19_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 50 )
      return;
    l_data->phase_counter = 0;

    swap_cells_colours( PAPER_RED|BRIGHT, PAPER_GREEN|BRIGHT );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL19_DATA *ld = (LEVEL19_DATA*)malloc( sizeof(LEVEL19_DATA) );
    level->level_data = ld;

    /* Set up to start in state 0 */
    ld->state = 1;
    ld->phase_counter = 0;
    draw_box( 1, 1, 30, 21, PAPER_BLUE|BRIGHT );
  }
}
