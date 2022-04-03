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

#ifndef __MAIN_H
#define __MAIN_H

#define STDIO_DEBUG 0

#if STDIO_DEBUG
#pragma printf %f %d %ld
#endif

#define NAMED_ARG(N,V) (V)

/* Use 25 for the timing test, 40 for final game (as the algorithm stands) */
#define MAX_IN_SWARM ((const uint8_t)25)

#define MAX_GAME_TIME_SECS   ((const uint8_t)1200)
#define MAX_GAME_TIME_FRAMES ((const uint8_t)60000)
#define GAME_TIME_FRAMES_PER_BLOCK ((const uint16_t)1875)


typedef enum _direction
{
  DIRECTION_STATIONARY = 0,
  DIRECTION_N          = 0x01,
  DIRECTION_NE         = 0x03,
  DIRECTION_NW         = 0x05,
  DIRECTION_S          = 0x04,
  DIRECTION_SE         = 0x14,
  DIRECTION_SW         = 0x24,
  DIRECTION_E          = 0x40,
  DIRECTION_W          = 0x80,
} DIRECTION;

#endif
