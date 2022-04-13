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

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
typedef uint8_t boolean_t;

/* Countdown goes from 31 to 0 */
extern uint8_t countdown;
extern uint8_t paused;

boolean_t draw_timer( boolean_t );
void time_up( void );
void winner( void );

#define GET_COUNTDOWN       ((uint8_t)(countdown))
#define SET_COUNTDOWN(t)    countdown = (uint8_t)(t)
#define GET_COUNTDOWNPAUSED ((uint8_t)(paused))

#define PAUSE_TIMER   paused=1
#define UNPAUSE_TIMER paused=0

#endif
