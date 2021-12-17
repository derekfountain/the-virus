#include <stdint.h>
#include <arch/zx.h>
#include <input.h>
#include "player.h"

int16_t  player_x_i;
int16_t  player_y_i;
int16_t  previous_player_x_i;
int16_t  previous_player_y_i;

/* These are in virion.c, bit of a cheat but I can use them here */
extern uint8_t *screen_line_starts[192];
extern uint8_t  screen_line_offsets[256];
extern uint8_t  screen_byte_values[256];

void init_player( void )
{
  /* Middle of screen */
  player_x_i = 128;
  player_y_i =  96;
}

int16_t query_player_x( void )
{
  return player_x_i;
}

int16_t query_player_y( void )
{
  return player_y_i;
}

void move_player( void )
{
  if( in_key_pressed( IN_KEY_SCANCODE_q ) && player_y_i )       player_y_i-=2;
  if( in_key_pressed( IN_KEY_SCANCODE_a ) && player_y_i < 190 ) player_y_i+=2;
  if( in_key_pressed( IN_KEY_SCANCODE_o ) && player_x_i )       player_x_i-=2;
  if( in_key_pressed( IN_KEY_SCANCODE_p ) && player_x_i < 254 ) player_x_i+=2;
}

void draw_player( void )
{
  uint8_t *scr_byte = screen_line_starts[player_y_i];

  *(scr_byte + screen_line_offsets[player_x_i])   |= screen_byte_values[player_x_i];
  *(scr_byte + screen_line_offsets[player_x_i+1]) |= screen_byte_values[player_x_i+1];

  scr_byte = screen_line_starts[player_y_i+1];

  *(scr_byte + screen_line_offsets[player_x_i])   |= screen_byte_values[player_x_i];
  *(scr_byte + screen_line_offsets[player_x_i+1]) |= screen_byte_values[player_x_i+1];

  previous_player_x_i = player_x_i;
  previous_player_y_i = player_y_i;
}


void clear_player( void )
{
  uint8_t *scr_byte = screen_line_starts[previous_player_y_i];

  *(scr_byte + screen_line_offsets[previous_player_x_i])   = 0;
  *(scr_byte + screen_line_offsets[previous_player_x_i+1]) = 0;

  scr_byte = screen_line_starts[previous_player_y_i+1];

  *(scr_byte + screen_line_offsets[previous_player_x_i])   = 0;
  *(scr_byte + screen_line_offsets[previous_player_x_i+1]) = 0;
}
