#include <stdint.h>
#include <arch/zx.h>
#include <input.h>
#include "player.h"

uint8_t  player_x_i;
uint8_t  player_y_i;
uint8_t  previous_player_x_i;
uint8_t  previous_player_y_i;

CONTROL control;

/* These are in virion.c, bit of a cheat but I can use them here */
extern uint8_t *screen_line_starts[192];
extern uint8_t  screen_line_offsets[256];
extern uint8_t  screen_byte_values[256];

// Need to add player velocity
// constant movement
// unable to enter black regions

void init_player( CONTROL c )
{
  /* Middle of screen */
  player_x_i = 128;
  player_y_i =  96;

  control = c;
}

uint8_t query_player_x( void )
{
  return player_x_i;
}

uint8_t query_player_y( void )
{
  return player_y_i;
}

uint8_t move_player( void )
{
  if( control == KEYBOARD )
  {
    if( in_key_pressed( IN_KEY_SCANCODE_q ) && player_y_i )       player_y_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_a ) && player_y_i < 190 ) player_y_i+=2;
    if( in_key_pressed( IN_KEY_SCANCODE_o ) && player_x_i )       player_x_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_p ) && player_x_i < 254 ) player_x_i+=2;
  }
  else
  {
    uint16_t kempston_input = in_stick_kempston();

    if( (kempston_input & IN_STICK_UP)    && player_y_i )       player_y_i-=2;
    if( (kempston_input & IN_STICK_DOWN)  && player_y_i < 190 ) player_y_i+=2;
    if( (kempston_input & IN_STICK_LEFT)  && player_x_i )       player_x_i-=2;
    if( (kempston_input & IN_STICK_RIGHT) && player_x_i < 254 ) player_x_i+=2;
  }

  return ( in_key_pressed( IN_KEY_SCANCODE_1 ) );
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
