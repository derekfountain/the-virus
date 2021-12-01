#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>
#include <sys/ioctl.h>

#include "main.h"
#include "draw_swarm.h"
#include "vectors.h"
#include "swarm_algorithm.h"

Vector swarm[NUM_IN_SWARM];
Vector previous_swarm[NUM_IN_SWARM];

Vector goal;



void main(void)
{
  uint8_t i;

  // Middle of screen for now.
  goal.x_i = 128; goal.x_f = f16_i16( goal.x_i );
  goal.y_i =  96; goal.y_f = f16_i16( goal.y_i );

  const half_t f100 = f16_i16( 100 );

  init_draw_swarm();
  ioctl(1, IOCTL_OTERM_PAUSE, 0);

  test_vector_distance_f();

  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    swarm[i].x_i = rand()%256; swarm[i].x_f = f16_u16( swarm[i].x_i ); swarm[i].velocity_x = f16_f32( 0.1 );
    swarm[i].y_i = rand()%192; swarm[i].y_f = f16_u16( swarm[i].y_i ); swarm[i].velocity_y = f16_f32( 0.1 );
    //printf("Init: %d, %d == %f, %f\n", swarm[i].x_i, swarm[i].y_i,
    //                                   f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
  }

  while(1)
  {
    int i;

    for( i=0; i < NUM_IN_SWARM; i++ )
    {
//      Vector avoid_others_v;
      Vector move_to_goal_v;

      /*
       * Avoid others
       */
      //avoid_others(i, &avoid_others_v);
      //printf("Avoid others: vel_x=%f, vel_y=%f\n", f32_f16( avoid_others_v.velocity_x ), f32_f16( avoid_others_v.velocity_y ) );


      /*
       * Move to goal
       */
      move_to_goal_v.x_i = goal.x_i;
      move_to_goal_v.y_i = goal.y_i;
      //  printf("1: goalpos_x=%d, goalpos_y=%d\n", move_to_goal_v.x_i, move_to_goal_v.y_i );

      // printf("2: pos_x=%d, pos_y=%d\n", swarm[i].x_i, swarm[i].y_i );
      move_to_goal_v.x_i = move_to_goal_v.x_i - swarm[i].x_i;
      move_to_goal_v.y_i = move_to_goal_v.y_i - swarm[i].y_i;
      // printf("Move to goal %d (before div100): vel_x=%d, vel_y=%d\n", i, move_to_goal_v.x_i, move_to_goal_v.y_i );

      // Integer rounding is different between C and Tcl. Not sure how to handle it, this is closest.
      move_to_goal_v.velocity_x = ( divf16( f16_i16(move_to_goal_v.x_i), f100 ) );
      move_to_goal_v.velocity_y = ( divf16( f16_i16(move_to_goal_v.y_i), f100 ) );
      //printf("Move to goal %d: vel_x=%f, vel_y=%f\n\n", i, f32_f16( move_to_goal_v.velocity_x ), f32_f16( move_to_goal_v.velocity_y ) );


      /*
       * Add up total velocity
       */
      //swarm[i].velocity_x = addf16( swarm[i].velocity_x, avoid_others_v.velocity_x );
      //swarm[i].velocity_y = addf16( swarm[i].velocity_y, avoid_others_v.velocity_y );

      swarm[i].velocity_x = addf16( swarm[i].velocity_x, move_to_goal_v.velocity_x );
      swarm[i].velocity_y = addf16( swarm[i].velocity_y, move_to_goal_v.velocity_y );
  

      /*
       * Limit velocity
       */
#if 1
      const half_t SPEED_LIMIT = f16_i16(64);

      // This appears to be just squaring the velocity?
//      printf("Vel in %d: vel_x=%f, vel_y=%f\n\n", i, f32_f16( swarm[i].velocity_x ), f32_f16( swarm[i].velocity_y ) );
      half_t x_sq = mulf16( subf16( f16_i16(0), swarm[i].velocity_x ), subf16( f16_i16(0), swarm[i].velocity_x ) );
      half_t y_sq = mulf16( subf16( f16_i16(0), swarm[i].velocity_y ), subf16( f16_i16(0), swarm[i].velocity_y ) );
      half_t mag = addf16( x_sq, y_sq );
      if( isgreaterf16( mag, SPEED_LIMIT ) )
      {
//	printf("Mag: %f\n", f32_f16( mag ) );
	half_t t_x = divf16( swarm[i].velocity_x, mag );
	half_t t_y = divf16( swarm[i].velocity_y, mag );

	swarm[i].velocity_x = mulf16( t_x, SPEED_LIMIT );
	swarm[i].velocity_y = mulf16( t_y, SPEED_LIMIT );
      }
//      printf("Vel ou %d: vel_x=%f, vel_y=%f\n\n", i, f32_f16( swarm[i].velocity_x ), f32_f16( swarm[i].velocity_y ) );

//    set magnitude [vector_distance {0 0} $dots($index,velocity)]
//    if { $magnitude > $SPEED_LIMIT } {
//	set t [vector_div $dots($index,velocity) $magnitude]
//	set dots($index,velocity) [vector_mul $t $SPEED_LIMIT]
//    }
#endif



      /* Finally, add calculated velocity to dot position */
///      printf("Pre-update %d: x=%f, y=%f\n", i, f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
///      printf("Velocity   %d: x=%f, y=%f\n", i, f32_f16( swarm[i].velocity_x ), f32_f16( swarm[i].velocity_y ) );

      swarm[i].x_f = addf16( swarm[i].x_f, swarm[i].velocity_x );
      swarm[i].y_f = addf16( swarm[i].y_f, swarm[i].velocity_y );

///      printf("Pos-update %d: x=%f, y=%f\n", i, f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );

      /* Updated rounded version of position */
      swarm[i].x_i = i16_f16( ( swarm[i].x_f ) );
      swarm[i].y_i = i16_f16( ( swarm[i].y_f ) );
///      printf("Ints now   %d: x=%d, y=%d\n\n", i, swarm[i].x_i, swarm[i].y_i );
    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();

    /* Draw the newly computed swarm - also hard coded */
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    /* OPT I only need the int positions, so keeping those separate will make this faster */
    memcpy( previous_swarm, swarm, sizeof(swarm) );
  }
}
