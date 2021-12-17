#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdint.h>

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

void clear_player( uint8_t x, uint8_t y );
void draw_player( uint8_t x, uint8_t y );

#endif
