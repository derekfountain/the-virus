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
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level0_frame),
    NAMED_ARG("Caption",              "Red cells kill the virus! Hurrah!"),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level1_frame),
    NAMED_ARG("Caption",              "Green cells make things worse! Boo!"),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level2_frame),
    NAMED_ARG("Caption",              "Black cells just get in the way"),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        50),
    NAMED_ARG("Draw frame handler",   draw_level3_frame),
    NAMED_ARG("Caption",              "Blue cells just confuse things"),
  },

  /* Bit harder */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_GREEN),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level4_frame),
    NAMED_ARG("Caption",              "Let's try something a bit harder"),
  },

  /* Introduce a moving block */
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

void draw_level0_frame(void)
{
  /* Starter level, single red block in corner */
#include "level0.inc"
}

void draw_level1_frame(void)
{
  /* Introduce green block */
#include "level1.inc"
}

void draw_level2_frame(void)
{
  /* Single red block in corner, 4 black blocks in a X around it */
#include "level2.inc"
}

void draw_level3_frame(void)
{
  /* Big red block surrounded by blue blocks */
#include "level3.inc"
}

void draw_level4_frame(void)
{
#include "level4.inc"
}
