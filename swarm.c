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
    swarm[p].x = rand()&255;
    swarm[p].y = rand()&183;

    swarm[p].velocity_x = vel;
    swarm[p].velocity_y = vel;

    swarm[p].active = 0;
    
    swarm[p].previous_x = -1;
    swarm[p].previous_y = -1;
  }

  /* Activate those used in this level */
  for( p=0; p<size; p++ )
  {
    swarm[p].active = 1;
  }
}

/*
 * The velocity values are used throughout this algorithm, try to get the
 * compiler to keep them in registers. Having these as locals in the loop
 * they're used in sees the generated code end up with them on the top of
 * the stack (i.e. not in registers) so they're accessed via index registers
 * anyway. :( Keeping them here as globals has them accessed directly
 * via their memory address, which is faster than via the index registers.
 */
register int16_t vx;
register int16_t vy;

void update_swarm( LEVEL *level )
{
  register uint8_t i;

  /*
   * Update each dot's velocity and then location based on empirically derived rules. This actually
   * updates every other dot each pass, every other-other dot being left in the same place as it was
   * in the previous iteration. This allows twice as many dots to be managed, at the expense of
   * more accurate behaviour. The virion logic needs to be applied to every dot each time round
   * though, because on screens with moving blocks the logic needs to be applied for each block
   * change.
   *
   * The loop is always over the maximum number, regardless of how many are in play. This keeps the
   * speed reasonably consistent.
   */
  every_other_dot ^= 1;
  for( i=0; i < level->max_virions; i++ )
  {
    if( (i & 0x01) == every_other_dot )
    {
      /*
       * The velocity values are used throughout this algorithm, make them
       * easier to access.
       */
      vx = swarm[i].velocity_x;
      vy = swarm[i].velocity_y;

      /*
       * Move towards player. Calculate distance to player, take 1/256th of it. Add that to velocity.
       * This has to be a signed 32bit calculation. If the player is a x=255 then 255*256=65280. If
       * the player is at x=0 then 0*0=0, and if the swarm dot is at x=255 then 0-65280=-65280. The
       * result will be in the range 255 to -255.
       */
      vx += (((int32_t)QUERY_PLAYER_X*(int32_t)256) - (int32_t)swarm[i].x*(int32_t)256) / (int16_t)256;
      vy += (((int32_t)QUERY_PLAYER_Y*(int32_t)256) - (int32_t)swarm[i].y*(int32_t)256) / (int16_t)256;


      /*
       * Limit velocity. This is ugly and causes stuttering sort of behaviour, but it's the best
       * I could get without a square root.
       */
      const int16_t SPEED_LIMIT = 280;
      if( vx > SPEED_LIMIT || vx < -SPEED_LIMIT )
      {
        int16_t div2 = vx / 2;
        if( div2 != 0 )
          vx = div2;
      }
      if( vy > SPEED_LIMIT || vy < -SPEED_LIMIT )
      {
        int16_t div2 = vy / 2;
        if( div2 != 0 )
          vy = div2;
      }

      /*
       * Original algorithm attempts to move dots so they don't get too close to each other.
       * That's not possible on the Spectrum, there's nowhere near enough CPU power to
       * iterate all of the swarm each iteration of this loop. I get the milling around behaviour
       * by introducing some random jitter. However, this only happens to dots which are moving
       * slowly. This means they crowd the player, but when moving at speed they tend to clump
       * together, which makes guiding the swarm easier.
       */
      const int16_t JITTER_LIMIT = 35;
      if( (vx < JITTER_LIMIT && vx > -JITTER_LIMIT)
          ||
          (vy < JITTER_LIMIT && vy > -JITTER_LIMIT) )
      {
        /*
         * This used to pick from the random pool at random, but that was a bit expensive CPUwise.
         * This implementation cycles the pool of random numbers, but you can't really tell from
         * the swarm's on screen behaviour.
         * This is buggy. Because the ++ is missing off the second use of 'r', the same random value
         * will be used for vy on this iteration of the loop and for vx on the next iteration. But
         * fixing it makes a pattern appear in the swarm's behaviour. I don't know why, so I left
         * it like this. The first instance of "that's not a bug it's a feature" I've ever come across!
         */
        static uint8_t r=0;
        vx += random_values[r++];
        vy += random_values[r];
      }


      /*
       * Finally, add calculated velocity to dot position. The velocity calculation is done at high
       * scale to maintain accuracy in the integers. (Doing maths with numbers in the range 0-255
       * leads to rounding to zeroes way too much.) This was designed to take 1% of the velocity as
       * calculated but the div 100 was very expensive. div 64 has a similar enough effect and is much
       * faster.
       */
      swarm[i].x += vx/64;
      swarm[i].y += vy/64;

      /* Restore calculated values back into the structure */
      swarm[i].velocity_x = vx;
      swarm[i].velocity_y = vy;

      /*
       * The virion logic call was always in here until I spotted the every-other-one bug.
       * I can only do these checks here - i.e. for every other virion - if there are
       * no changing or moving colour cells. When there are moving cells this has to be
       * done for all virions, so I skip it here and do it below, outside the every-other-one
       * condition.
       */
      if( ! level->has_moving_cells )
      {
      /* Remove it from its old screen position */
        clear_virion( &swarm[i] );

        apply_virion_logic( level, &swarm[i] );

        /* Put it in its new place */
        draw_virion( &swarm[i] );
      }
    }

    /*
     * This has to be outside the every-other-one condition because the screens with
     * moving blocks need all virions processed for each iteration. Otherwise a block
     * can move onto a virion and there's a 50/50 chance it doesn't get recognised.
     *
     * This slows things down, which is a shame, but I can't see a way around it. The
     * has-moving-cells flag allows for fewer virions on levels with moving cells,
     * which at least keeps the speed consistent.
     */
    if( level->has_moving_cells )
    {
      /* Remove it from its old screen position */
      clear_virion( &swarm[i] );

      apply_virion_logic( level, &swarm[i] );

      /* Put it in its new place */
      draw_virion( &swarm[i] );
    }

    /* Note the new place ready for removing it next time round */
    swarm[i].previous_x = swarm[i].x;
    swarm[i].previous_y = swarm[i].y;
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


VIRION *activate_virion_in_swarm( void )
{
  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( ! swarm[i].active )
    {
      swarm[i].active = 1;
      current_num_virions++;

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
    printf("Virion %d, active %d, x=%d, y=%d\n",i,swarm[i].active,swarm[i].x,swarm[i].y);
}

#endif
