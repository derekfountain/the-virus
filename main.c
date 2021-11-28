#include <stdlib.h>
#include <string.h>
#include <arch/zx.h>

#include "main.h"
#include "draw_swarm.h"
#include "vectors.h"

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
    swarm[i].x_i = rand()%256;
    swarm[i].y_i = rand()%192;
  }

  int j=0;
  while(j<2000)
  {
    int i;

//    memcpy( previous_swarm, swarm, sizeof(swarm) );

    for( i=0; i < NUM_IN_SWARM; i++ )
    {
//      int16_t avoid_others_v = avoid_others(i);
    
      /* Extra rule to make the boids follow the red dot. */
      /* Move 1% closer to it each iteration */

//      Vector v4 = goal;
#if 0

[vector_sub [list $::goal_x $::goal_y] $dots($i,position)]
	set v4 [vector_div $v4 100]

	set dots($i,velocity) [vector_add $dots($i,velocity) $v2]
	set dots($i,velocity) [vector_add $dots($i,velocity) $v4]

	limit_velocity $i

	set dots($i,position) [vector_add $dots($i,position) $dots($i,velocity)]
	set dots($i,position_int) "[expr {round([lindex $dots($i,position) 0])}] [expr {round([lindex $dots($i,position) 1])}]"
#endif
  
    }

//    draw_swarm( XOR_MODE, previous_swarm );
    draw_swarm( OR_MODE, swarm );
    j++;
  }

  return 0;
}
