#ifndef __LEVELS_H
#define __LEVELS_H

#include <stdint.h>
#include "level.h"

#define NUM_LEVELS 25

void init_level( LEVEL* );
uint8_t select_level(void);
LEVEL *get_level( uint8_t );

#endif
