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

#ifndef __VIRION_H
#define __VIRION_H

#include <stdint.h>

#define MAKE_IMMUNE     ((uint8_t)1)
#define MAKE_NON_IMMUNE ((uint8_t)0)
/*
 * A virion is a single particle of a virus. We all knew that, right?
 */
typedef struct _virion
{
  int16_t   x_i;
  int16_t   y_i;

  int16_t   velocity_x;
  int16_t   velocity_y;

  uint8_t  active;
  uint16_t immunity_start;

  int16_t  previous_x_i;
  int16_t  previous_y_i;

} VIRION;

void init_draw_virion_tables(void);
void draw_virion( VIRION* );
void clear_virion( VIRION* );
void activate_virion( VIRION* );
void deactivate_virion( VIRION* );
void change_immunity( VIRION *, uint8_t );

#endif
