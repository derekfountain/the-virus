#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>
#include <input.h>
#include <sys/ioctl.h>
#include <intrinsic.h>
#include <z80.h>
#include <im2.h>

#include "main.h"
#include "virion.h"
#include "player.h"
#include "levels.h"
#include "controls.h"
#include "int.h"

unsigned char version[8] = "ver0.01";

VIRION   swarm[MAX_IN_SWARM];

int16_t  move_to_player_x_i;
int16_t  move_to_player_y_i;

int8_t   random_values[255];

/* General purpose counter, kept static for speed */
uint8_t i;

void main(void)
{
  zx_cls( PAPER_WHITE );

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
    /*
     * Create a pool of random numbers for the jitter. These are added to the velocity.
     * Every other one is negative.
     * Refreshed each new level in case patterns appear, which I think is unlikely.
     */
    uint16_t r;
    for(r=0; r<256; r++)
    {
      random_values[r] = rand()%50;
      if( r & 0x01 )
	random_values[r] = -(random_values[r]);
    }

    LEVEL *level = get_level(current_level);

    /* ioctl(1, IOCTL_OTERM_PAUSE, 0); */

    init_player( selected_control );

    draw_level( level );

    /* Starting points */
    for( i=0; i<level->starting_num_virions; i++ )
    {
      swarm[i].x_i = rand()%256; swarm[i].velocity_x = 100;
      swarm[i].y_i = rand()%192; swarm[i].velocity_y = 100;
      swarm[i].active = 1;
    }

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
      move_player();

      /*
       * Update each dot's velocity and then location based on empirically derived rules. This actually
       * updates every other dot each pass, every other-other dot being left to do the same as it did
       * in the previous iteration. This allows twice as many dots to be managed, at the expense of
       * more accurate behaviour.
       *
       * The loop is always over the maximum number, regardless of how many are in play. This keeps the
       * speed reasonably consistent.
       */
      static uint8_t every_other_dot=0;
      every_other_dot ^= 1;
      for( i=every_other_dot; i < MAX_IN_SWARM; i+=2 )
      {
	/*
	 * Move towards player. Calculate distance to player, take 1/256th of it. Add that to velocity.
	 * This has to be a signed 32bit calculation. If the player is a x=255 then 255*256=65280. If
	 * the player is at x=0 then 0*0=0, and if the swarm dot is at x=255 then 0-65280=-65280. The
	 * result will be in the range 255 to -255.
	 */
	swarm[i].velocity_x += (((int32_t)query_player_x()*(int32_t)256) - (int32_t)swarm[i].x_i*(int32_t)256) / (int16_t)256;
	swarm[i].velocity_y += (((int32_t)query_player_y()*(int32_t)256) - (int32_t)swarm[i].y_i*(int32_t)256) / (int16_t)256;


	/*
	 * Limit velocity. This is ugly and causes stuttering sort of behaviour, but it's the best
	 * I could get without a square root.
	 */
	const int16_t SPEED_LIMIT = 350;
	if( swarm[i].velocity_x > SPEED_LIMIT || swarm[i].velocity_x < -SPEED_LIMIT )
	{
	  int16_t div2 = swarm[i].velocity_x / 2;
	  if( div2 != 0 )
	    swarm[i].velocity_x = div2;
	}
	if( swarm[i].velocity_y > SPEED_LIMIT || swarm[i].velocity_y < -SPEED_LIMIT )
	{
	  int16_t div2 = swarm[i].velocity_y / 2;
	  if( div2 != 0 )
	    swarm[i].velocity_y = div2;
	}

	/*
	 * Original algorithm attempts to move dots so they don't get too close to each other.
	 * That's not posssible on the Spectrum, there's nowhere near enough CPU power to
	 * iterate all of the swarm each iteration of this loop. I get the milling around behaviour
	 * by introducing some random jitter. However, this only happens to dots which are moving
	 * slowly. This means they crowd the player, but when moving at speed they tend to clump
	 * together, which makes guding the swarm easier.
	 */
	const int16_t JITTER_LIMIT = 35;
	if( swarm[i].velocity_x < JITTER_LIMIT && swarm[i].velocity_x > -JITTER_LIMIT
	    ||
	    swarm[i].velocity_y < JITTER_LIMIT && swarm[i].velocity_y > -JITTER_LIMIT )
	{
	  /*
	   * This used to pick from the random pool at random, but that was a bit expensive CPUwise.
	   * This implementation cycles the pool of random numbers, but you can't really tell from
	   * the swarm's on screen behaviour.
	   */
	  static uint8_t r=0;
	  swarm[i].velocity_x += *(random_values+r++);
	  swarm[i].velocity_y += *(random_values+r);
	}


	/*
	 * Finally, add calculated velocity to dot position. The velocity calculation is done at high
	 * scale to maintain accuracy in the integers. (Doing maths with numbers in the range 0-255
	 * leads to rounding to zeroes way too much.) So this takes 1% of the velocity as calculated.
	 */
	swarm[i].x_i += swarm[i].velocity_x/100;
	swarm[i].y_i += swarm[i].velocity_y/100;
	
	/* Remove it from its old screen position */
	clear_virion( &swarm[i] );

	if( swarm[i].active )
	  apply_virion_logic( level, &swarm[i] );

	/* Put it in its new place */
	draw_virion( &swarm[i] );

	/* Note the new place ready for removing it next time round */
	swarm[i].previous_x_i = swarm[i].x_i;
	swarm[i].previous_y_i = swarm[i].y_i;
      }

      clear_player();
      draw_player();

      /* If number in swarm == 0, level is now cleared so break */
      if( level->current_num_virions == 0 )
	break;
    }

    current_level++;
  }

}

