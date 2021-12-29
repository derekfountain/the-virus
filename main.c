#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

    init_player( selected_control );

    init_level( level );

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
      if( move_player() )
	break;

      update_swarm( level );
      
      clear_player();
      draw_player();

      /* If number in swarm == 0, level is now cleared so break */
      if( get_active_swarm_size() == 0 )
	break;
    }

    if( ++current_level == NUM_LEVELS )
    {
      game_over();
    }
  }

}

