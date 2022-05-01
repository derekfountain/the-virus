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
#include "levels.h"
#include "levels_primitives.h"

/* 4 starter levels */
void draw_level0_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level0.inc"
  }
}

void draw_level1_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level1.inc"
  }
}

void draw_level2_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level2.inc"
  }
}

void draw_level3_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level3.inc"
  }
}


/* 4 intermediate levels */
void draw_level4_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level4.inc"
  }
}

void draw_level5_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level5.inc"
  }
}

void draw_level6_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level6.inc"
  }
}

void draw_level7_frame( LEVEL *level, LEVEL_PHASE phase )
{
  (void)level;
  (void)phase;

  if( phase == PHASE_INIT )
  {
#include "level7.inc"
  }
}
