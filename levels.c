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
#include "level.h"
#include "levels.h"

LEVEL levels[] = 
{
  /* Training levels */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level0_frame),
    NAMED_ARG("Caption",              "  Red cells kill the virus!     "),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level1_frame),
    NAMED_ARG("Caption",              "Green cells make things worse!  "),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level2_frame),
    NAMED_ARG("Caption",              " Black cells get in the way!    "),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level3_frame),
    NAMED_ARG("Caption",              "  Blue cells confuse things!    "),
    NAMED_ARG("Level data",           NULL),
  },

  /* Bit harder */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level4_frame),
    NAMED_ARG("Caption",              "         Got it? OK!            "),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level5_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Level data",           NULL),
  },

  /* Introduce moving blocks */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_RED),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level6_frame),
    NAMED_ARG("Caption",              "Oh, and sometimes things move!  "),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_RED),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level7_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_RED),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level8_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_GREEN),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level9_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Level data",           NULL),
  },

};

uint8_t select_level(void)
{
  return 9;
}


LEVEL *get_level( uint8_t lev )
{
  return &levels[lev];
}


void draw_cells( uint8_t cells[][2], uint8_t colour )
{
  uint8_t i=0;

  while( cells[i][0] != 0xFF )
  {
    *(zx_cxy2aaddr(cells[i][0],cells[i][1])) = colour;

    i++;
  }
}

void _2x2( uint8_t x, uint8_t y, uint8_t colour )
{
  uint8_t cells[4][2] = { {0,0},{1,0},{1,1},{0,1} };

  uint8_t i;
  for(i=0;i<4;i++)
    *(zx_cxy2aaddr(cells[i][0]+x,cells[i][1]+y)) = colour;
}

void _5x1( uint8_t x, uint8_t y, uint8_t colour )
{
  uint8_t i;

  uint8_t level_red[][2] = { {0,1},{1,1},{2,1},{3,1},{4,1} };
  for(i=0;i<5;i++)
    *(zx_cxy2aaddr(level_red[i][0]+x,level_red[i][1]+y)) = colour;
}


void draw_level0_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level0.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

void draw_level1_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level1.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

void draw_level2_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level2.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

void draw_level3_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level3.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

void draw_level4_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level4.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

void draw_level5_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;

  if( phase == PHASE_UPDATE )
  {
  }
  else if( phase == PHASE_INIT )
  {
#include "level5.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
  }
}

typedef struct _level6_data
{
  uint8_t x;
} LEVEL6_DATA;
void draw_level6_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL6_DATA *l6_data = level->level_data;

    *(zx_cxy2aaddr(l6_data->x,11)) = PAPER_WHITE;
    if( l6_data->x++ == 28 )
      l6_data->x = 2;
    *(zx_cxy2aaddr(l6_data->x,11)) = PAPER_RED;
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL6_DATA *l6d = (LEVEL6_DATA*)malloc( sizeof(LEVEL6_DATA) );
    level->level_data = l6d;

    l6d->x = 2;
    *(zx_cxy2aaddr(l6d->x,11)) = PAPER_RED;
  }
  else if( phase == PHASE_FINALISE )
  {
    free( level->level_data );
  }
}

typedef struct _level7_data
{
  uint8_t   x;
  uint8_t   y;
  DIRECTION d;
} LEVEL7_DATA;
void draw_level7_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL7_DATA *l7_data = level->level_data;

    _2x2( l7_data->x, l7_data->y, PAPER_WHITE );

    if( l7_data->d == DIRECTION_E )
    {
      if( l7_data->x++ == 27 )
        l7_data->d = DIRECTION_S;
    }
    else if( l7_data->d == DIRECTION_S )
    {
      if( l7_data->y++ == 19 )
        l7_data->d = DIRECTION_W;
    }
    else if( l7_data->d == DIRECTION_W )
    {
      if( l7_data->x-- == 3 )
        l7_data->d = DIRECTION_N;
    }
    else if( l7_data->d == DIRECTION_N )
    {
      if( l7_data->y-- == 3 )
        l7_data->d = DIRECTION_E;
    }

    _2x2( l7_data->x, l7_data->y, PAPER_RED );

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL7_DATA *l7d = (LEVEL7_DATA*)malloc( sizeof(LEVEL7_DATA) );
    level->level_data = l7d;

    l7d->x = 2;
    l7d->y = 2;
    l7d->d = DIRECTION_E;
    _2x2( l7d->x, l7d->y, PAPER_RED );
  }
  else if( phase == PHASE_FINALISE )
  {
    free( level->level_data );
  }
}


typedef struct _level8_data
{
  uint8_t   x;
  DIRECTION d;
} LEVEL8_DATA;
void draw_level8_frame( LEVEL *level, LEVEL_PHASE phase )
{
  if( phase == PHASE_UPDATE )
  {
    LEVEL8_DATA *l8_data = level->level_data;

    _5x1( l8_data->x, 10, PAPER_WHITE );

    if( l8_data->d == DIRECTION_E )
      l8_data->x++;
    else
      l8_data->x--;

    if( l8_data->x == 27 )
    {
      l8_data->d = DIRECTION_W;
    }
    else if( l8_data->x == 0 )
    {
      l8_data->d = DIRECTION_E;
    }

    _5x1( l8_data->x, 10, PAPER_RED );
  }
  else if( phase == PHASE_INIT )
  {
    LEVEL8_DATA *l8d = (LEVEL8_DATA*)malloc( sizeof(LEVEL8_DATA) );
    level->level_data = l8d;

    l8d->x = 0;
    l8d->d = DIRECTION_E;
    _5x1( l8d->x, 10, PAPER_RED );

#include "level8.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
    free( level->level_data );
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
    LEVEL9_DATA *l9_data = level->level_data;

//    _2x2( l9_data->x, l9_data->y, PAPER_WHITE );

    if( l9_data->d == DIRECTION_E )
    {
      if( l9_data->x++ == 27 )
        l9_data->d = DIRECTION_S;
    }
    else if( l9_data->d == DIRECTION_S )
    {
      if( l9_data->y++ == 19 )
        l9_data->d = DIRECTION_W;
    }
    else if( l9_data->d == DIRECTION_W )
    {
      if( l9_data->x-- == 3 )
        l9_data->d = DIRECTION_N;
    }
    else if( l9_data->d == DIRECTION_N )
    {
      if( l9_data->y-- == 3 )
        l9_data->d = DIRECTION_E;
    }

//    _2x2( l9_data->x, l9_data->y, PAPER_RED );

  }
  else if( phase == PHASE_INIT )
  {
    LEVEL9_DATA *l9d = (LEVEL9_DATA*)malloc( sizeof(LEVEL9_DATA) );
    level->level_data = l9d;

    l9d->x = 2;
    l9d->y = 2;
    l9d->d = DIRECTION_E;

//    not sure what to do with this. it's ok as it is. flash a blue box around the red one, maybe?
//    _2x2( l9d->x, l9d->y, PAPER_RED );

#include "level9.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
    free( level->level_data );
  }
}
