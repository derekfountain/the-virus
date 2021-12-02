#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>
#include <input.h>
#include <math.h>
#include <sys/ioctl.h>

#include "main.h"
#include "draw_swarm.h"

/* Keep these together so the copy into 'previous' can be done in one go */
int16_t swarm_x_i[NUM_IN_SWARM];
int16_t swarm_y_i[NUM_IN_SWARM];
int16_t previous_swarm_x_i[NUM_IN_SWARM];
int16_t previous_swarm_y_i[NUM_IN_SWARM];

half_t swarm_x_f[NUM_IN_SWARM];
half_t swarm_y_f[NUM_IN_SWARM];

half_t swarm_velocity_x[NUM_IN_SWARM];
half_t swarm_velocity_y[NUM_IN_SWARM];

int16_t  goal_x_i;
int16_t  goal_y_i;
int16_t  previous_goal_x_i;
int16_t  previous_goal_y_i;

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
  // ioctl(1, IOCTL_OTERM_PAUSE, 0);

  /* Starting points */
  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    swarm_x_i[i] = rand()%256; swarm_x_f[i] = f16_u16( swarm_x_i[i] ); swarm_velocity_x[i] = f16_f32( 0.1 );
    swarm_y_i[i] = rand()%192; swarm_y_f[i] = f16_u16( swarm_y_i[i] ); swarm_velocity_y[i] = f16_f32( 0.1 );
  }

#define TIME_TEST 0
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

    if( in_key_pressed( IN_KEY_SCANCODE_q ) && goal_y_i )       goal_y_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_a ) && goal_y_i < 192 ) goal_y_i+=2;
    if( in_key_pressed( IN_KEY_SCANCODE_o ) && goal_x_i )       goal_x_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_p ) && goal_x_i < 256 ) goal_x_i+=2;

    for( i=0; i < NUM_IN_SWARM; i++ )
    {
      /*
       * Original algorithm attempts to move dots so they don't get too close
       * to each other. That's not posssible on the Spectrum. I get the milling
       * around behaviour by introducing some random jitter.
       */
      if( bump++ & 0x04 )
      {
	swarm_velocity_x[i] = addf16( swarm_velocity_x[i], random_values[rand()&0xff] );
	swarm_velocity_y[i] = addf16( swarm_velocity_y[i], random_values[rand()&0xff] );
      }


      /*
       * Move to goal. Calculate distance to goal, take 1% of it. Add that to velocity.
       */
      move_to_goal_x_i = goal_x_i - swarm_x_i[i];
      move_to_goal_y_i = goal_y_i - swarm_y_i[i];

      swarm_velocity_x[i] = addf16( swarm_velocity_x[i], divf16( f16_i16(move_to_goal_x_i), f100 ) );
      swarm_velocity_y[i] = addf16( swarm_velocity_y[i], divf16( f16_i16(move_to_goal_y_i), f100 ) );
  

      /*
       * Limit velocity
       */
      const half_t SPEED_LIMIT = f16_f32(2.75);
      if( isgreaterf16( swarm_velocity_x[i], SPEED_LIMIT ) || isgreaterf16( swarm_velocity_y[i], SPEED_LIMIT ) )
      {
	swarm_velocity_x[i] = div2f16( swarm_velocity_x[i] );
	swarm_velocity_y[i] = div2f16( swarm_velocity_y[i] );
      }


      /* Finally, add calculated velocity to dot position */
      swarm_x_f[i] = addf16( swarm_x_f[i], swarm_velocity_x[i] );
      swarm_y_f[i] = addf16( swarm_y_f[i], swarm_velocity_y[i] );

      /* Updated rounded version of position */
      swarm_x_i[i] = i16_f16( ( swarm_x_f[i] ) );
      swarm_y_i[i] = i16_f16( ( swarm_y_f[i] ) );
    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();
    clear_player( previous_goal_x_i, previous_goal_y_i );

    /* Draw the newly computed swarm - also hard coded */
    draw_player( goal_x_i, goal_y_i );
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    memcpy( previous_swarm_x_i, swarm_x_i, sizeof(swarm_x_i)+sizeof(swarm_y_i) );
    previous_goal_x_i = goal_x_i;
    previous_goal_y_i = goal_y_i;
  }
}
