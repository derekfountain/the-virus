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

#ifndef __SOUND_H
#define __SOUND_H

/* Don't play a sound every bounce or reactiviation, it slows things down */
#define SOUND_FRAMES 4

extern uint8_t sound_on;
#define TOGGLE_SOUND  sound_on=!sound_on

void kill_virion_sound(void);
void reactivate_virion_sound(void);
void relocate_virion_sound(void);
void toggle_sound_sound(void);

#endif
