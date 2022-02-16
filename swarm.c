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
#include <stdlib.h>
#include "swarm.h"
#include "main.h"
#include "player.h"
#include "virion.h"
#include "level.h"
#include "int.h"

VIRION   swarm[MAX_IN_SWARM];
uint8_t  current_num_virions;

int8_t   random_values[255];

uint8_t  every_other_dot=1;

void init_swarm( uint8_t size, int16_t vel )
{
  uint8_t p;

  /* Starting points */
  for( p=0; p<MAX_IN_SWARM; p++ )
  {
    swarm[p].x_i = rand()&255;
    swarm[p].y_i = rand()&191;

    swarm[p].velocity_x = vel;
    swarm[p].velocity_y = vel;

    swarm[p].active = 0;
    swarm[p].immunity_start = GET_TICKER;   /* Start immune */
    
    swarm[p].previous_x_i = -1;
    swarm[p].previous_y_i = -1;
  }

  /* Activate those used in this level */
  for( p=0; p<size; p++ )
  {
    swarm[p].active = 1;
  }
}

void update_swarm( LEVEL *level )
{
  uint8_t i;

  /*
   * Update each dot's velocity and then location based on empirically derived rules. This actually
   * updates every other dot each pass, every other-other dot being left to do the same as it did
   * in the previous iteration. This allows twice as many dots to be managed, at the expense of
   * more accurate behaviour.
   *
   * The loop is always over the maximum number, regardless of how many are in play. This keeps the
   * speed reasonably consistent.
   */
  every_other_dot ^= 1;
  for( i=every_other_dot; i < MAX_IN_SWARM; i+=2 )
  {
    /*
     * Move towards player. Calculate distance to player, take 1/256th of it. Add that to velocity.
     * This has to be a signed 32bit calculation. If the player is a x=255 then 255*256=65280. If
     * the player is at x=0 then 0*0=0, and if the swarm dot is at x=255 then 0-65280=-65280. The
     * result will be in the range 255 to -255.
     */
    swarm[i].velocity_x += (((int32_t)QUERY_PLAYER_X*(int32_t)256) - (int32_t)swarm[i].x_i*(int32_t)256) / (int16_t)256;
    swarm[i].velocity_y += (((int32_t)QUERY_PLAYER_Y*(int32_t)256) - (int32_t)swarm[i].y_i*(int32_t)256) / (int16_t)256;


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
    if( (swarm[i].velocity_x < JITTER_LIMIT && swarm[i].velocity_x > -JITTER_LIMIT)
	||
	(swarm[i].velocity_y < JITTER_LIMIT && swarm[i].velocity_y > -JITTER_LIMIT) )
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

    apply_virion_logic( level, &swarm[i] );

    /* Put it in its new place */
    draw_virion( &swarm[i] );

    /* Note the new place ready for removing it next time round */
    swarm[i].previous_x_i = swarm[i].x_i;
    swarm[i].previous_y_i = swarm[i].y_i;

    /* If this virion is immune, check if its immunity has run out */
    if( swarm[i].immunity_start )
    {
      if( GET_TICKER > (swarm[i].immunity_start+level->immune_frames) )
      {
	swarm[i].immunity_start = 0;
      }
    }
  }

  return;
}


void refresh_random_values( void )
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
  
  return;
}


VIRION *activate_virion_in_swarm( uint8_t start )
{
  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( ! swarm[i].active )
    {
      swarm[i].active = 1;
      current_num_virions++;

      if( start == START_IMMUNE )
	change_immunity( &swarm[i], MAKE_IMMUNE );
      else
	change_immunity( &swarm[i], MAKE_NON_IMMUNE );
	
      return &swarm[i];
    }
  }

  return INVALID_VIRION_PTR;
}

#if STDIO_DEBUG

#include <stdio.h>
void printf_swarm_details(void)
{
  printf("Virions left: %d\n",GET_ACTIVE_SWARM_SIZE);
  uint8_t i;
  for(i=0;i<MAX_IN_SWARM;i++)
    printf("Virion %d, active %d, x=%d, y=%d\n",i,swarm[i].active,swarm[i].x_i,swarm[i].y_i);
}

#endif
