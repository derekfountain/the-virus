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

#ifndef __LEVEL_H
#define __LEVEL_H

#include <stdint.h>
#include "virion.h"

typedef struct __level
{
  uint8_t  starting_num_virions;
  uint8_t  max_virions;

  int16_t  starting_velocity;

  uint8_t  border_colour;

  uint8_t immune_frames;

  void   (*draw_frame)(void);
} LEVEL;


void apply_virion_logic( LEVEL*, VIRION* );

#endif
