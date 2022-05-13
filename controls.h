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

#ifndef __CONTROLS_H
#define __CONTROLS_H

#include <stdint.h>

typedef enum _control
{
  KEYBOARD,
  JOYSTICK
} CONTROL;

extern uint16_t up_scancode;
extern uint16_t down_scancode;
extern uint16_t left_scancode;
extern uint16_t right_scancode;
extern uint16_t sound_scancode;

#define UP_SCANCODE    ((uint16_t)up_scancode)
#define DOWN_SCANCODE  ((uint16_t)down_scancode)
#define LEFT_SCANCODE  ((uint16_t)left_scancode)
#define RIGHT_SCANCODE ((uint16_t)right_scancode)
#define SOUND_SCANCODE ((uint16_t)sound_scancode)

CONTROL select_controls(void);

#endif
