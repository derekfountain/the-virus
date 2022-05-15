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

#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <z80.h>
#include <im2.h>
#include <string.h>
#include "player.h"
#include "main.h"
#include "timer.h"

/*
 * Timer ticker for the 50Hz interrupt signal which fires
 * via the hardware every 20ms.
 *
 * The "getter" for this is currently a macro, so don't make
 * this static.
 */
volatile uint16_t ticker = 0;
volatile uint16_t timer_ticker = 0;

/*
 * 100ms ticker. This one increments every 5 interrupts, so it
 * ticks up every 100ms.
 */
uint8_t           ticker_100ms_int_counter;  /* This goes 0-5 */
volatile uint8_t  interrupt_service_required_100ms = 0;

IM2_DEFINE_ISR(isr)
{
  /*
   * This all happens with the interrupt still disabled so
   * nothing needs atomic protection.
   */
  ticker++;

  if( !GET_COUNTDOWNPAUSED )
  {
    if( ++timer_ticker == GAME_TIME_FRAMES_PER_BLOCK )
    {
      SET_COUNTDOWN( (GET_COUNTDOWN-1) );
      timer_ticker=0;
    }
  }

  if( ++ticker_100ms_int_counter == 5 )
  {
    ticker_100ms_int_counter = 0;
    interrupt_service_required_100ms = 1;
  }

  /* This is better done here, it's smoother on screen */
  clear_player();
  draw_player();
}

/*
 * Standard interrupt set up for now.
 */
#define TABLE_HIGH_BYTE        ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xD1)

#define UI_256                 ((unsigned int)256)
#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))

void setup_int(void)
{
  memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );
  z80_bpoke( JUMP_POINT,   195 );
  z80_wpoke( JUMP_POINT+1, (unsigned int)isr );
  im2_init( TABLE_ADDR );
  intrinsic_ei();
}
