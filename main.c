#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>

#include "main.h"
#include "draw_swarm.h"
#include "vectors.h"
#include "swarm_algorithm.h"

Vector swarm[NUM_IN_SWARM];
Vector previous_swarm[NUM_IN_SWARM];

Vector goal;



int main(void)
{
  uint8_t i;

  // Middle of screen for now.
  goal.x_i = 128; goal.x_f = f16_i16( goal.x_i );
  goal.y_i =  96; goal.y_f = f16_i16( goal.y_i );

  init_draw_swarm();

  test_vector_distance_f();

  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    swarm[i].x_i = rand()%256; swarm[i].x_f = f16_i16( swarm[i].x_i );
    swarm[i].y_i = rand()%192; swarm[i].y_f = f16_i16( swarm[i].y_i );
    //printf("Init: %d, %d == %f, %f\n", swarm[i].x_i, swarm[i].y_i,
    //                                   f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
  }

  int j=0;
  while(j<2000)
  {
    int i;

    for( i=0; i < NUM_IN_SWARM; i++ )
    {
      Vector avoid_others_v;
      Vector move_to_goal_v;

      avoid_others(i, &avoid_others_v);
      //printf("Avoid others: vel_x=%f, vel_y=%f\n", f32_f16( avoid_others_v.velocity_x ), f32_f16( avoid_others_v.velocity_y ) );

      move_to_goal_v.x_f = goal.x_f;
      move_to_goal_v.y_f = goal.y_f;
      //printf("1: goalpos_x=%f, goalpos_y=%f\n", f32_f16( move_to_goal_v.x_f ), f32_f16( move_to_goal_v.y_f ) );

      //printf("2: pos_x=%f, pos_y=%f\n", f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
      vector_sub_f( &move_to_goal_v, &swarm[i] );
      //printf("3: pos_x=%f, pos_y=%f\n", f32_f16( move_to_goal_v.x_f ), f32_f16( move_to_goal_v.y_f ) );
      //printf("4: vel_x=%f, vel_y=%f\n", f32_f16( swarm[i].x_f ), f32_f16( swarm[i].y_f ) );
      vector_div_f( &move_to_goal_v, f16_i32(100) );
      //printf("Move to goal: vel_x=%f, vel_y=%f\n", f32_f16( move_to_goal_v.x_f ), f32_f16( move_to_goal_v.y_f ) );
    move_to_goal_v.velocity_x = move_to_goal_v.x_f;
    move_to_goal_v.velocity_y = move_to_goal_v.y_f;

      swarm[i].velocity_x = addf16( swarm[i].velocity_x, avoid_others_v.velocity_x );
      swarm[i].velocity_y = addf16( swarm[i].velocity_y, avoid_others_v.velocity_y );

      swarm[i].velocity_x = addf16( swarm[i].velocity_x, move_to_goal_v.velocity_x );
      swarm[i].velocity_y = addf16( swarm[i].velocity_y, move_to_goal_v.velocity_y );
  
      // Limit velocity here

      /* Finally, add calculated velocity to dot position */
      swarm[i].x_f = addf16( swarm[i].x_f, swarm[i].velocity_x );
      swarm[i].y_f = addf16( swarm[i].y_f, swarm[i].velocity_y );

      /* Updated rounded version of position */
      swarm[i].x_i = i16_f16( roundf16( swarm[i].x_f ) );
      swarm[i].y_i = i16_f16( roundf16( swarm[i].y_f ) );
    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();

    /* Draw the newly computed swarm - also hard coded */
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    /* OPT I only need the int positions, so keeping those separate will make this faster */
    memcpy( previous_swarm, swarm, sizeof(swarm) );

    j++;
  }

  return 0;
}
