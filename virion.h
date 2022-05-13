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

#ifndef __VIRION_H
#define __VIRION_H

#include <stdint.h>

/*
 * A virion is a single particle of a virus. We all knew that, right?
 */
typedef struct _virion
{
  uint8_t  active;

  int16_t  x;
  int16_t  y;

  int16_t  velocity_x;
  int16_t  velocity_y;

  int16_t  previous_x;
  int16_t  previous_y;

} VIRION;

void draw_virion( VIRION* );
void clear_virion( VIRION* );
void activate_virion( VIRION* );
void deactivate_virion( VIRION* );
void random_reappear_virion( VIRION* );

#endif
