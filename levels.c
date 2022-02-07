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
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level1_frame),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level2_frame),
  },
  {
    NAMED_ARG("Starting num virions", 12),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level3_frame),
  },

  /* Bit harder */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_GREEN),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level4_frame),
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
  /* Single red block in corner, 4 black blocks in a X around it */
  *(zx_cxy2aaddr(5,5))  = PAPER_RED;

  *(zx_cxy2aaddr(2,8))  = PAPER_BLACK;
  *(zx_cxy2aaddr(8,8))  = PAPER_BLACK;
  *(zx_cxy2aaddr(8,2))  = PAPER_BLACK;
  *(zx_cxy2aaddr(2,2))  = PAPER_BLACK;
}

void draw_level2_frame(void)
{
  /* Introduce green block */
  *(zx_cxy2aaddr(5,5))   = PAPER_RED;

  *(zx_cxy2aaddr(25,15)) = PAPER_GREEN;
  *(zx_cxy2aaddr(26,15)) = PAPER_GREEN;
  *(zx_cxy2aaddr(25,16)) = PAPER_GREEN;
  *(zx_cxy2aaddr(26,16)) = PAPER_GREEN;
}

void draw_level3_frame(void)
{
  /* Big red block surrounded by green blocks */
  *(zx_cxy2aaddr(5,5))   = PAPER_RED;
  *(zx_cxy2aaddr(5,6))   = PAPER_RED;
  *(zx_cxy2aaddr(6,5))   = PAPER_RED;
  *(zx_cxy2aaddr(6,6))   = PAPER_RED;

  *(zx_cxy2aaddr(2,9))   = PAPER_GREEN;
  *(zx_cxy2aaddr(9,9))   = PAPER_GREEN;
  *(zx_cxy2aaddr(9,2))   = PAPER_GREEN;
  *(zx_cxy2aaddr(2,2))   = PAPER_GREEN;
}

void draw_level4_frame(void)
{
  *(zx_cxy2aaddr(5,20))   = PAPER_RED;
  *(zx_cxy2aaddr(6,20))   = PAPER_RED;
  *(zx_cxy2aaddr(5,21))   = PAPER_RED;
  *(zx_cxy2aaddr(6,21))   = PAPER_RED;

  uint8_t trail[][2] =
    {
      {4,20}, {4,21},
      {4,19}, {5,19}, {6,19}, {7,19}, {8,19}, {9,19}, {10,19}, {11,19}, {12,19}, {13,19}, {14,19}, {15,19},
      {4,22}, {5,22}, {6,22}, {7,22}, {8,22}, {9,22}, {10,22}, {11,22}, {12,22}, {13,22}, {14,22}, {15,22}, {16,22}, {17,22},{18,22},
      {18,21},{18,20},{18,19},
      {15,18},{18,18},
      {15,17},{18,17},
      {15,16},{18,16},
      {15,15},{18,15},
      {15,14},{18,14},
    };

  uint8_t i;
  for( i=0; i<sizeof(trail)/2; i++ )
  {
    *(zx_cxy2aaddr(trail[i][0],trail[i][1]))  = PAPER_BLACK;
  }
  
}
