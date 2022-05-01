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

void hide_player( void );
void init_player( CONTROL );
uint8_t move_player( void );
void clear_player( void );
void draw_player( void );

/* Player x,y, see player.c for definitions. Client code can use these macros */
extern uint8_t player_x;
extern uint8_t player_y;
#define QUERY_PLAYER_X ((uint8_t)(player_x))
#define QUERY_PLAYER_Y ((uint8_t)(player_y))

#endif
