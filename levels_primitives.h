/*
 * The Virus, a ZX Spectrum game.
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

#ifndef __LEVELS_PRIMITIVES_H
#define __LEVELS_PRIMITIVES_H

#include <stdint.h>
#include "level.h"
#include "levels.h"

void draw_cells( uint8_t cells[][2], uint8_t );
void _2x2( uint8_t, uint8_t, uint8_t );
void _5x1( uint8_t, uint8_t, uint8_t );
void draw_box( uint8_t, uint8_t, uint8_t, uint8_t, uint8_t );
void draw_h_line( uint8_t, uint8_t, uint8_t, uint8_t );
void swap_cells_colours( uint8_t, uint8_t );

#endif
