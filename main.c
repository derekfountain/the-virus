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

#include <stdlib.h>
#include <string.h>
#include <arch/zx.h>
#include <input.h>
#include <sys/ioctl.h>
#include <intrinsic.h>
#include <z80.h>

#include "main.h"
#include "virion.h"
#include "player.h"
#include "levels.h"
#include "controls.h"
#include "int.h"
#include "swarm.h"
#include "game_over.h"

unsigned char version[8] = "ver0.01";

/* Statics for speed */
LEVEL   *level;

void main(void)
{
  /* Set up interrupts */
  setup_int();

  /* Initialise the tables which optimise screen drawing */
  init_draw_virion_tables();

  /* Ask user for controls - keyboard or joystick */
  CONTROL selected_control = select_controls();

  uint8_t current_level = select_level();

  /* Outer infinite loop, level selection */
  while( 1 )
  {
    zx_cls( PAPER_WHITE );

    /* Pick up level data */
    level = get_level(current_level);

    /* Refresh the pool of random values so patterns don't appear */
    refresh_random_values();

    /* Hide dot while caption is on screen */
    hide_player();

    init_level( level );

    init_player( selected_control );

    init_swarm( level->starting_num_virions,
                level->starting_velocity );

#define TIME_TEST 0
/* Keep this time test at less than 20 seconds with MAX_SWARM=25 */
#if TIME_TEST
    uint16_t countdown = 500;
#endif

    while(1
#if TIME_TEST
	  && --countdown
#endif
      )
    {
      /* Small hack here to allow bumping through the levels */
      uint8_t mp_result = move_player();
      if( mp_result == 1 )
	break;
#if STDIO_DEBUG
      if( mp_result == 2 )
        printf_swarm_details();
#endif
      /* Update level displayed on screen. Swarm logic is applied to updated level.  */
      update_level( level );

      update_swarm( level );

      /*
       * Player was cleared and redrawn here, but it suffered a bit of
       * flicker. I moved it to the ISR, so it happens in top border
       * time. See int.c.
       */

      /* If number in swarm == 0, level is now cleared so break */
      if( GET_ACTIVE_SWARM_SIZE == 0 )
	break;
    }

    /* Reclaim memory, etc */
    finalise_level( level );

    if( ++current_level == NUM_LEVELS )
    {
      zx_border( INK_MAGENTA );
      game_over();
    }
  }

}

