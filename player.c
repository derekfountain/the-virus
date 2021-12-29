#include <stdint.h>
#include <arch/zx.h>
#include <input.h>
#include "player.h"

uint8_t  player_x_i;
uint8_t  player_y_i;
uint8_t  previous_player_x_i;
uint8_t  previous_player_y_i;

CONTROL   control;
DIRECTION direction;

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

  direction = DIRECTION_STATIONARY;
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
  /* This is ignored for keyboard control */
  uint16_t kempston_input = in_stick_kempston();

  DIRECTION input;

  /* Read input */
  if(      ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_q )) 
           ||
           ((control == JOYSTICK) && (kempston_input & IN_STICK_UP)) )
  {
    input = DIRECTION_N;
  }
  else if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_a )) 
	   ||
	   ((control == JOYSTICK) && (kempston_input & IN_STICK_DOWN)) )
  {
    input = DIRECTION_S;
  }
  else if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_o )) 
	   ||
	   ((control == JOYSTICK) && (kempston_input & IN_STICK_LEFT)) )
  {
    input = DIRECTION_W;
  }
  else if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_p )) 
	   ||
	   ((control == JOYSTICK) && (kempston_input & IN_STICK_RIGHT)) )
  {
    input = DIRECTION_E;
  }
  else
  {
    input = DIRECTION_STATIONARY;
  }

  /* If there's room to move in the direction of travel, do so */
  if(      ((input == DIRECTION_N) || (input == DIRECTION_STATIONARY && direction == DIRECTION_N)) && player_y_i )
  {
    player_y_i-=2;
    direction = DIRECTION_N;
  }
  else if( ((input == DIRECTION_S) || (input == DIRECTION_STATIONARY && direction == DIRECTION_S)) && player_y_i < 190 )
  {
    player_y_i+=2;
    direction = DIRECTION_S;
  }
  else if( ((input == DIRECTION_W) || (input == DIRECTION_STATIONARY && direction == DIRECTION_W)) && player_x_i )
  {
    player_x_i-=2;
    direction = DIRECTION_W;
  }
  else if( ((input == DIRECTION_E) || (input == DIRECTION_STATIONARY && direction == DIRECTION_E)) && player_x_i < 254 )
  {
    player_x_i+=2;
    direction = DIRECTION_E;
  }
  else
  {
    /* No room to move in direction of travel, so bounce */
    if( direction == DIRECTION_N )
      direction = DIRECTION_S;
    else if( direction == DIRECTION_S )
      direction = DIRECTION_N;
    else if( direction == DIRECTION_W )
      direction = DIRECTION_E;
    else if( direction == DIRECTION_E )
      direction = DIRECTION_W;
  }

  /* Return the level bump magic key */
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
