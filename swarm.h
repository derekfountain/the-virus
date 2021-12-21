#ifndef __SWARM_H
#define __SWARM_H

#include <stdint.h>
#include "level.h"

void init_swarm( uint8_t, int16_t );
void update_swarm( LEVEL * );
void refresh_random_values( void );

#endif
