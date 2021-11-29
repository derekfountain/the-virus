#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "vectors.h"
#include "swarm_algorithm.h"

void avoid_others( uint8_t index, Vector *result )
{
  extern Vector swarm[NUM_IN_SWARM];

  uint8_t i;
  uint8_t MIN_DISTANCE=4;
  
  result->velocity_x = 0;
  result->velocity_y = 0;

  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    if( i != index )
    {
      int16_t distance = vector_distance_i( &swarm[i], &swarm[index] );
      // printf("%d: distance is %d\n", i, distance);

      if( distance < MIN_DISTANCE )
      {
	Vector diff;

	diff.x_i = swarm[i].x_i - swarm[index].x_i;
	diff.y_i = swarm[i].y_i - swarm[index].y_i;

	result->velocity_x -= diff.x_i;
	result->velocity_y -= diff.y_i;
      }
    }
  }
  return;
}

