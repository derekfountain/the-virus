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
    NAMED_ARG("Caption",              "Red cells kill the virus! Hurrah!"),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level1_frame),
    NAMED_ARG("Caption",              "Green cells make things worse! Boo!"),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level2_frame),
    NAMED_ARG("Caption",              "Black cells just get in the way"),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level3_frame),
    NAMED_ARG("Caption",              "Blue cells just confuse things"),
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
    NAMED_ARG("Caption",              "Let's try something a bit harder"),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level5_frame),
    NAMED_ARG("Caption",              ""),
    NAMED_ARG("Level data",           NULL),
  },

  /* Introduce a moving block */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        0),
    NAMED_ARG("Level handler",        draw_level6_frame),
    NAMED_ARG("Caption",              ""),
    NAMED_ARG("Level data",           NULL),
  },
};

uint8_t select_level(void)
{
  return 0;
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
    level->level_data = (LEVEL6_DATA*)malloc( sizeof(LEVEL6_DATA) );
  }
  else if( phase == PHASE_INIT )
  {
#include "level6.inc"
  }
  else if( phase == PHASE_FINALISE )
  {
    free( level->level_data );
  }
}
