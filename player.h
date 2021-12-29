#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdint.h>
#include "controls.h"

typedef enum _direction
{
  DIRECTION_N,
  DIRECTION_NE,
  DIRECTION_E,
  DIRECTION_SE,
  DIRECTION_S,
  DIRECTION_SW,
  DIRECTION_W,
  DIRECTION_NW,
} DIRECTION;

void init_player( CONTROL );
uint8_t query_player_x( void );
uint8_t query_player_y( void );
uint8_t move_player( void );
void clear_player( void );
void draw_player( void );

#endif
