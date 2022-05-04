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
#include "level.h"
#include "levels.h"
#include "levels_01234567.h"
#include "levels_89.h"
#include "levels_1011.h"
#include "levels_1213.h"
#include "levels_1415.h"
#include "levels_1617.h"
#include "levels_1819.h"

LEVEL levels[] = 
{
  /* Training levels */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level0_frame),
    NAMED_ARG("Caption",              "  Red cells kill the virus!     "),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level1_frame),
    NAMED_ARG("Caption",              "Green cells make things worse!  "),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level2_frame),
    NAMED_ARG("Caption",              " Black cells get in the way!    "),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level3_frame),
    NAMED_ARG("Caption",              "  Blue cells confuse things!    "),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },

  /* Bit harder */
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level4_frame),
    NAMED_ARG("Caption",              "         Got it? OK!            "),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level5_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level6_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },

    {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level7_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     FALSE),
    NAMED_ARG("Level data",           NULL),
  },


  /* Introduce moving blocks */
  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level8_frame),
    NAMED_ARG("Caption",              "Oh, and sometimes things move!  "),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level9_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level10_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level11_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level12_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level13_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

    {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level14_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },
  /* Play testing with 40 virions and a joystick, I got here in 17mins */

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level15_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level16_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level17_frame),
    NAMED_ARG("Caption",              NULL),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Level handler",        draw_level18_frame),
    NAMED_ARG("Caption",              "  Two to go! How's the time?    "),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },

  {
    NAMED_ARG("Starting num virions", MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Max num virions",      MAX_WITH_MOVING_CELLS),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Level handler",        draw_level19_frame),
    NAMED_ARG("Caption",              "      Last one! Hurry up!       "),
    NAMED_ARG("Has moving cells",     TRUE),
    NAMED_ARG("Level data",           NULL),
  },
};

LEVEL *get_level( uint8_t lev )
{
  return &levels[lev];
}
