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

#define L14_TRAIL_LEN 6
typedef struct _level14_data
{
  uint8_t phase_counter;
  uint8_t block_is_green;
  uint8_t i;
} LEVEL14_DATA;
uint8_t l14_trail[][2] = { {12,9},{13,9},{14,9},{15,9},{16,9},{17,9},{18,9},{19,9},
                           {19,10},{19,11},{19,12},
                           {19,13},{18,13},{17,13},{16,13},{15,13},{14,13},{13,13},{12,13},
                           {12,12},{12,11},{12,10},
                         };
void get_trail_entry( int8_t i, uint8_t **result )
{
  if( i < 0 )
    i = 22+i;
  *result = &(l14_trail[i][0]);
}
void draw_level14_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL14_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter == 100 )
    {
      l_data->phase_counter = 0;
      l_data->block_is_green = !l_data->block_is_green;
      swap_cells_colours( (PAPER_GREEN|INK_BLACK|BRIGHT), (PAPER_RED|INK_BLACK|BRIGHT) );
    }
    if( l_data->phase_counter & 1 )
      return;

    uint8_t i;
    uint8_t *entry;
    int8_t plot = (int8_t)l_data->i;
    for(i=0;i<L14_TRAIL_LEN;i++)
    {
      get_trail_entry( plot, &entry );
      plot--;
      *(zx_cxy2aaddr(*entry,*(entry+1))) = ( (( l_data->block_is_green ) ? PAPER_RED : PAPER_GREEN)|BRIGHT);
    }

    /* Add a white one to remove the end of the previous trail */
    get_trail_entry( plot, &entry );
    *(zx_cxy2aaddr(*entry,*(entry+1))) = PAPER_WHITE;

    if( ++l_data->i == 22 )
      l_data->i = 0;
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL14_DATA *ld = (LEVEL14_DATA*)malloc( sizeof(LEVEL14_DATA) );
    level->level_data = ld;

    ld->phase_counter =  0;
    ld->block_is_green = 1;
    ld->i              = 0;
#include "level14.inc"
  }
}

typedef struct _level15_data
{
  uint8_t   state;
  uint8_t   instep;
  uint8_t   colour;
  uint8_t   phase_counter;
} LEVEL15_DATA;
void draw_level15_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL15_DATA *l_data = level->level_data;

    if( ++l_data->phase_counter < 15 )
      return;
    l_data->phase_counter = 0;

    draw_box(0+l_data->instep,0+l_data->instep,31-l_data->instep,22-l_data->instep,PAPER_WHITE);

    if( l_data->instep++ == 6 )
      l_data->instep = 0;

    if( l_data->colour == (PAPER_RED|BRIGHT) )
      l_data->colour = PAPER_BLACK|BRIGHT;
    else if( l_data->colour == (PAPER_BLACK|BRIGHT) )
      l_data->colour = PAPER_BLUE|BRIGHT;
    else if( l_data->colour == (PAPER_BLUE|BRIGHT) )
      l_data->colour = PAPER_GREEN|BRIGHT;
    else if( l_data->colour == (PAPER_GREEN|BRIGHT) )
      l_data->colour = PAPER_RED|BRIGHT;

    draw_box(0+l_data->instep,0+l_data->instep,31-l_data->instep,22-l_data->instep,l_data->colour);

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL15_DATA *ld = (LEVEL15_DATA*)malloc( sizeof(LEVEL15_DATA) );
    level->level_data = ld;

    ld->state = 0;
    ld->instep = 0;
    ld->colour = PAPER_RED|BRIGHT;
    ld->phase_counter = 0;

    draw_box(0,0,31,22,PAPER_RED|BRIGHT);
  }
}
