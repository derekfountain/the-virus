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


typedef struct _bouncing_line
{
  int8_t x;
  int8_t y;
  int8_t length;
  int8_t colour;
  int8_t direction;
} BOUNCING_LINE;

void draw_bouncing_line( BOUNCING_LINE *bl )
{
  draw_h_line( 0, 31, bl->y, PAPER_WHITE|INK_BLACK );

  uint8_t xpos = (bl->x-bl->length) > 0 ? bl->x-bl->length : 0;
  draw_h_line( xpos,
               xpos+bl->length,
               bl->y,
               bl->colour );
}

typedef struct _level19_data
{
  BOUNCING_LINE blue_line;
  BOUNCING_LINE green_line;
} LEVEL19_DATA;
void draw_level19_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL19_DATA *l_data = level->level_data;

    if( l_data->blue_line.x == 31 )
    {
      l_data->blue_line.x         = 30;
      l_data->blue_line.direction = 1;
    }
    else if( l_data->blue_line.x == l_data->blue_line.length )
    {
      l_data->blue_line.x         = l_data->blue_line.length + 1;
      l_data->blue_line.direction = 0;
    }
    draw_bouncing_line( &(l_data->blue_line) );
    if( l_data->blue_line.direction == 0 )
      l_data->blue_line.x++;
    else
      l_data->blue_line.x--;


    if( l_data->green_line.x == 31 )
    {
      l_data->green_line.x         = 30;
      l_data->green_line.direction = 1;
    }
    else if( l_data->green_line.x == l_data->green_line.length )
    {
      l_data->green_line.x         = l_data->green_line.length + 1;
      l_data->green_line.direction = 0;
    }
    draw_bouncing_line( &(l_data->green_line) );
    if( l_data->green_line.direction == 0 )
      l_data->green_line.x++;
    else
      l_data->green_line.x--;
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL19_DATA *ld = (LEVEL19_DATA*)malloc( sizeof(LEVEL19_DATA) );
    level->level_data = ld;

    ld->blue_line.x         = 0;
    ld->blue_line.y         = 15;
    ld->blue_line.length    = 7;
    ld->blue_line.colour    = PAPER_BLUE|BRIGHT;
    ld->blue_line.direction = 0;
    draw_bouncing_line( &(ld->blue_line) );

    ld->green_line.x         = 12;
    ld->green_line.y         = 11;
    ld->green_line.length    = 3;
    ld->green_line.colour    = PAPER_GREEN|BRIGHT;
    ld->green_line.direction = 0;
    draw_bouncing_line( &(ld->green_line) );

    draw_box( 7, 19, 24, 19, PAPER_RED|BRIGHT );
    draw_box( 6, 18, 25, 20, PAPER_BLUE|BRIGHT );
    draw_h_line( 9, 22, 18, PAPER_WHITE );
  }
}
