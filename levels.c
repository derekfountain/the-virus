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
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        250),
    NAMED_ARG("Draw frame handler",   draw_level4_frame),
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


void draw_level0_frame(void)
{
  /* Starter level, single red block in corner */
  *(zx_cxy2aaddr(5,5))  = PAPER_RED;
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
/* This one doesn't work, replace it. */
  /* Big red blocks */
  *(zx_cxy2aaddr(5,20))   = PAPER_RED;
  *(zx_cxy2aaddr(6,20))   = PAPER_RED;
  *(zx_cxy2aaddr(5,21))   = PAPER_RED;
  *(zx_cxy2aaddr(6,21))   = PAPER_RED;

  *(zx_cxy2aaddr(26,20))  = PAPER_RED;
  *(zx_cxy2aaddr(27,20))  = PAPER_RED;
  *(zx_cxy2aaddr(26,21))  = PAPER_RED;
  *(zx_cxy2aaddr(27,21))  = PAPER_RED;

  uint8_t i;
  for( i=0; i<32; i+=2 )
  {
    *(zx_cxy2aaddr(i,0))  = PAPER_BLACK;
    *(zx_cxy2aaddr(i,12))  = PAPER_BLACK;
    *(zx_cxy2aaddr(i,23))  = PAPER_BLACK;
  }
  for( i=0; i<24; i+=2 )
  {
    *(zx_cxy2aaddr(16,i))  = PAPER_BLACK;
    *(zx_cxy2aaddr(16,i))  = PAPER_BLACK;
  }
  
}
