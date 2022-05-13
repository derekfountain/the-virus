/*
 * The Virus, a ZX Spectrum game.
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

#ifndef __SWARM_H
#define __SWARM_H

#include <stdint.h>
#include "level.h"

/* Invalid index in swarm */
#define INVALID_VIRION     ((uint8_t)255)
#define INVALID_VIRION_PTR ((VIRION*)NULL)

void init_swarm( uint8_t, int16_t );
void update_swarm( LEVEL * );
void refresh_random_values( void );
void printf_swarm_details(void);

/*
 * Find any deactivated virion in the swarm and reactivate it.
 * Answers the swarm index of the activated virion.
 */
VIRION *activate_virion_in_swarm( void );

extern uint8_t current_num_virions;

/*
 * Set the number of virions in the swarm.
 */
#define SET_SWARM_SIZE(N) current_num_virions = ((uint8_t)(N))

/*
 * Macro to provide the number of virions currently active in the swarm
 */
#define GET_ACTIVE_SWARM_SIZE ((uint8_t)(current_num_virions))

#endif
