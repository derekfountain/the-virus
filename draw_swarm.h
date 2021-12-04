#ifndef __DRAW_SWARM_H
#define __DRAW_SWARM_H

#include "main.h"

typedef enum _draw_mode
{
  OR_MODE,
  XOR_MODE
} Draw_Mode;


void init_draw_swarm(void);
void draw_swarm( Draw_Mode mod );
void draw_swarm_or( void );
void clear_swarm( void );

void clear_player( uint8_t, uint8_t );
void draw_player( uint8_t, uint8_t );

#endif
