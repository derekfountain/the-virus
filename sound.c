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
#include <sound.h>
#include "int.h"
#include "sound.h"

uint8_t  sound_on   = 1;
uint16_t last_sound = 0;

void kill_virion_sound( void )
{
  if( sound_on )
    bit_beepfx(BEEPFX_PICK);
}


void reactivate_virion_sound( void )
{
  if( !sound_on )
    return;

  if( (GET_TICKER - last_sound) > SOUND_FRAMES )
  {
    bit_beepfx(BEEPFX_HIT_4);
    last_sound = GET_TICKER;
  }
}

void relocate_virion_sound( void )
{
  if( !sound_on )
    return;

  if( (GET_TICKER - last_sound) > SOUND_FRAMES )
  {
    if( sound_on )
      bit_beepfx(BEEPFX_SHOT_1);
    last_sound = GET_TICKER;
  }
}

