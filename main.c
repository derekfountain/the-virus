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
#include "timer.h"
#include "sound.h"

#if STDIO_DEBUG
#include <stdio.h>
#endif

unsigned char version[8] = "ver1.00";

/* Statics for speed */
LEVEL   *level;

#define STARTING_VELOCITY (uint8_t)100

void main(void)
{
  /* Set up interrupts */
  setup_int();

  /* Infinite outer loop, new game */
  while(1)
  {
    zx_border( INK_WHITE );
    zx_cls( PAPER_WHITE|INK_BLACK );

    /*
     * ZX Paintbrush exports screen data in linear arrangement. Fiddle here
     * to get it looking right.
     */
    extern uint8_t tv_px[];
    uint8_t i;
    for(i=0;i<40;i++)
    {
      memcpy( zx_pxy2saddr(0,i), tv_px+(i*32), 32 );
    }
    for(i=0;i<5;i++)
    {
      memset( zx_cxy2aaddr(0,i), PAPER_BLACK|INK_WHITE, 32 );
    }

    /* Ask user for controls - keyboard or joystick */
    CONTROL selected_control = select_controls();

    uint8_t current_level = 0;
    SET_COUNTDOWN(32);

    /* Outer loop, level selection */
    while( 1 )
    {
      /* Not fair to run the timer while messages are scrolling, etc */
      PAUSE_TIMER;

      zx_cls( PAPER_WHITE );

      /* Pick up level data */
      level = get_level(current_level);

      /* Refresh the pool of random values so patterns don't appear */
      refresh_random_values();

      /* Hide dot while caption is on screen */
      hide_player();

      init_level( level );

      init_player( selected_control );

      init_swarm( level->starting_num_virions, STARTING_VELOCITY );

      RESET_HALF_SECONDS_PASSED;
      UNPAUSE_TIMER;

#define TIME_TEST 0
/*
 * Time test:
 *
 * Set max to 26/20 and use -SO3. Then:
 *
 * level 0 with "no moving cells" set TRUE should run in about 13.5 secs
 * level 9 with "no moving cells" set FALSE should run in about 13.5 secs
 */
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
        if( mp_result == 3 )
        {
          toggle_sound_sound();
          TOGGLE_SOUND;
#if STDIO_DEBUG
          printf("sound_on is %d\n",sound_on);
#endif
        }

        /* Update level displayed on screen. Swarm logic is applied to updated level.  */
        update_level( level );
        if( draw_timer(0) )
          break;

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

      /* Did we escape the inner loop because time was up? */
      if( GET_COUNTDOWN == 0 )
      {
        zx_border( INK_BLACK );
        hide_player();
        time_up();
        break;
      }

      /* We escaped the inner loop because the player completed the level */
      current_level++;

      /* Completed game? */
      if( current_level == NUM_LEVELS )
      {
        hide_player();
        winner();
        break;
      }

    }

    /* Back to top for a new game */
  }
}

