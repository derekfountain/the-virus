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

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdint.h>
#include "controls.h"

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

void init_player( CONTROL );
uint8_t query_player_x( void );
uint8_t query_player_y( void );
uint8_t move_player( void );
void clear_player( void );
void draw_player( void );

#endif
