#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>
#include <input.h>
#include <sys/ioctl.h>

#include "main.h"
#include "draw_swarm.h"
#include "vectors.h"

Vector swarm[NUM_IN_SWARM];
Vector previous_swarm[NUM_IN_SWARM];

int16_t  goal_x_i;
int16_t  goal_y_i;

int16_t  move_to_goal_x_i;
int16_t  move_to_goal_y_i;

half_t   random_values[255];


void main(void)
{
  uint8_t i;

  zx_border( INK_BLUE );
  zx_cls( PAPER_WHITE );

  // Middle of screen for now.
  goal_x_i = 128;
  goal_y_i =  96;

  const half_t f100 = f16_i16( 100 );

  /* Create a pool of random floating point numbers for the jitter */
  for(i=0;i<255;i++)
  {
    random_values[i] = f16_f32( (rand()%100) / 1000.0 );
    if( i & 0x01 )
      random_values[i] = negf16(random_values[i]);
    //printf("%f ", f32_f16(random_values[i]));
  }

  init_draw_swarm();
  ioctl(1, IOCTL_OTERM_PAUSE, 0);

  /* Starting points */
  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    swarm[i].x_i = rand()%256; swarm[i].x_f = f16_u16( swarm[i].x_i ); swarm[i].velocity_x = f16_f32( 0.1 );
    swarm[i].y_i = rand()%192; swarm[i].y_f = f16_u16( swarm[i].y_i ); swarm[i].velocity_y = f16_f32( 0.1 );
    //printf("Init: %d, %d == %f, %f\n", swarm[i].x_i, swarm[i].y_i,
    //                                   f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
  }

#define TIME_TEST 1
#if TIME_TEST
  uint16_t countdown = 500;
#endif

  uint8_t bump=0;
  while(1
#if TIME_TEST
	&& --countdown
#endif
    )
  {
    int i;

    if( in_key_pressed( IN_KEY_SCANCODE_q ) ) goal_y_i--;
    if( in_key_pressed( IN_KEY_SCANCODE_a ) ) goal_y_i++;
    if( in_key_pressed( IN_KEY_SCANCODE_o ) ) goal_x_i--;
    if( in_key_pressed( IN_KEY_SCANCODE_p ) ) goal_x_i++;

    for( i=0; i < NUM_IN_SWARM; i++ )
    {
      /*
       * Original algorithm attempts to move dots so they don't get too close
       * to each other. That's not posssible on the Spectrum. I get the milling
       * around behaviour by introducing some random jitter.
       */
      if( bump++ & 0x04 )
      {
	swarm[i].velocity_x = addf16( swarm[i].velocity_x, random_values[rand()&0xff] );
	swarm[i].velocity_y = addf16( swarm[i].velocity_y, random_values[rand()&0xff] );
      }


      /*
       * Move to goal. Calculate distance to goal, take 1% of it. Add that to velocity.
       */
      move_to_goal_x_i = goal_x_i - swarm[i].x_i;
      move_to_goal_y_i = goal_y_i - swarm[i].y_i;

      swarm[i].velocity_x = addf16( swarm[i].velocity_x, divf16( f16_i16(move_to_goal_x_i), f100 ) );
      swarm[i].velocity_y = addf16( swarm[i].velocity_y, divf16( f16_i16(move_to_goal_y_i), f100 ) );
  

      /*
       * Limit velocity
       */
      const half_t SPEED_LIMIT = f16_f32(2.75);
      if( isgreaterf16( swarm[i].velocity_x, SPEED_LIMIT ) || isgreaterf16( swarm[i].velocity_y, SPEED_LIMIT ) )
      {
	swarm[i].velocity_x = div2f16( swarm[i].velocity_x );
	swarm[i].velocity_y = div2f16( swarm[i].velocity_y );
      }


      /* Finally, add calculated velocity to dot position */
      swarm[i].x_f = addf16( swarm[i].x_f, swarm[i].velocity_x );
      swarm[i].y_f = addf16( swarm[i].y_f, swarm[i].velocity_y );

      /* Updated rounded version of position */
      swarm[i].x_i = i16_f16( ( swarm[i].x_f ) );
      swarm[i].y_i = i16_f16( ( swarm[i].y_f ) );
    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();

    /* Draw the newly computed swarm - also hard coded */
    draw_player( goal_x_i, goal_y_i );
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    /* OPT I only need the int positions, so keeping those separate will make this faster */
    memcpy( previous_swarm, swarm, sizeof(swarm) );
  }
}
