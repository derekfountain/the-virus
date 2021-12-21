#ifndef __SWARM_H
#define __SWARM_H

#include <stdint.h>
#include "level.h"

/* Invalid index in swarm */
#define INVALID_VIRION   ((uint8_t)255)

#define START_NON_IMMUNE ((uint8_t)0)
#define START_IMMUNE     ((uint8_t)1)

void init_swarm( uint8_t, int16_t );
void update_swarm( LEVEL * );
void refresh_random_values( void );

/*
 * Find any deactivated virion in the swarm and reactivate it.
 * Arg is whether to start it immune or not.
 * Answers the swarm index of the activated virion.
 */
uint8_t activate_virion_in_swarm( uint8_t );

/*
 * Set the number of virions in the swarm. Probably a bad idea, this.
 */
void set_swarm_size( uint8_t );

/*
 * Answers the number of virions currently active in the swarm
 */
uint8_t get_active_swarm_size( void );

#endif
