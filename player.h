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

void init_player( void );
int16_t query_player_x( void );
int16_t query_player_y( void );
void move_player( void );
void clear_player( void );
void draw_player( void );

#endif
