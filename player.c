/*
 * Shake the Virus, a ZX Spectrum game.
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

#include <stdint.h>
#include <arch/zx.h>
#include <input.h>
#include "player.h"

uint8_t  player_x;
uint8_t  player_y;
uint8_t  previous_player_x;
uint8_t  previous_player_y;

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
  player_x = 128;
  player_y =  96;

  control = c;

  direction = DIRECTION_STATIONARY;
}

uint8_t query_player_x( void )
{
  return player_x;
}

uint8_t query_player_y( void )
{
  return player_y;
}

/*
 * Read selected controls and move player screen x,y accordingly.
 *
 * It feels like this can be optimised, but I can't quite see how.
 *
 * This design allows the player to hold the dot against a screen.
 * I'm not sure I like that, but I'm leaving it for now.
 */
uint8_t move_player( void )
{
  /* This is ignored for keyboard control */
  uint16_t kempston_input = in_stick_kempston();

  DIRECTION input = DIRECTION_STATIONARY;

  /* Read input */
  if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_q )) 
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

  if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_o )) 
      ||
      ((control == JOYSTICK) && (kempston_input & IN_STICK_LEFT)) )
  {
    if( input == DIRECTION_STATIONARY )
      input = DIRECTION_W;
    else if( input == DIRECTION_N )
      input = DIRECTION_NW;
    else if( input == DIRECTION_S )
      input = DIRECTION_SW;
  }
  else if( ((control == KEYBOARD) && in_key_pressed( IN_KEY_SCANCODE_p )) 
	   ||
	   ((control == JOYSTICK) && (kempston_input & IN_STICK_RIGHT)) )
  {
    if( input == DIRECTION_STATIONARY )
      input = DIRECTION_E;
    else if( input == DIRECTION_N )
      input = DIRECTION_NE;
    else if( input == DIRECTION_S )
      input = DIRECTION_SE;
  }


  /* Deal with up, down, left and right */
  if( ((input == DIRECTION_N) || (input == DIRECTION_STATIONARY && direction == DIRECTION_N)) )
  {
    direction = DIRECTION_N;
    if( player_y )
    {
      player_y-=2;
    }
    else
    {
      direction = DIRECTION_S;
    }
  }
  else if( ((input == DIRECTION_S) || (input == DIRECTION_STATIONARY && direction == DIRECTION_S)) )
  {
    direction = DIRECTION_S;
    if( player_y < 190 )
    {
      player_y+=2;
    }
    else
    {
      direction = DIRECTION_N;
    }
  }
  else if( ((input == DIRECTION_W) || (input == DIRECTION_STATIONARY && direction == DIRECTION_W)) )
  {
    direction = DIRECTION_W;
    if( player_x )
    {
      player_x-=2;
    }
    else
    {
      direction = DIRECTION_E;
    }
  }
  else if( ((input == DIRECTION_E) || (input == DIRECTION_STATIONARY && direction == DIRECTION_E)) )
  {
    direction = DIRECTION_E;
    if( player_x < 254 )
    {
      player_x+=2;
    }
    else
    {
      direction = DIRECTION_W;
    }
  }


  /* Deal with diagonals */
  else if( ((input == DIRECTION_NW) || (input == DIRECTION_STATIONARY && direction == DIRECTION_NW)) )
  {
    direction = DIRECTION_NW;
    if( player_y && player_x )
    {
      /* Room to move as required */
      player_y-=2;
      player_x-=2;
    }
    else if( player_y == 0 && player_x == 0 )
    {
      /* Hit the very corner */
      direction = DIRECTION_SE;
    }
    else
    {
      if( player_y )
      {
	/* Hit the left side wall */
        player_y-=2;
        player_x+=2;
	direction = DIRECTION_NE;
      }
      else
      {
	/* Hit the roof */
        player_y+=2;
        player_x-=2;
	direction = DIRECTION_SW;
      }
    }
  }
  else if( ((input == DIRECTION_NE) || (input == DIRECTION_STATIONARY && direction == DIRECTION_NE)) )
  {
    direction = DIRECTION_NE;
    if( player_y && player_x < 254 )
    {
      /* Room to move as required */
      player_y-=2;
      player_x+=2;
    }
    else if( player_y == 0 && player_x == 254 )
    {
      /* Hit the very corner */
      direction = DIRECTION_SW;
    }
    else
    {
      if( player_y )
      {
	/* Hit the right side wall */
        player_y-=2;
        player_x-=2;
	direction = DIRECTION_NW;
      }
      else
      {
	/* Hit the roof */
        player_y+=2;
        player_x+=2;
	direction = DIRECTION_SE;
      }
    }
  }
  else if( ((input == DIRECTION_SW) || (input == DIRECTION_STATIONARY && direction == DIRECTION_SW)) )
  {
    direction = DIRECTION_SW;
    if( player_y < 190 && player_x )
    {
      /* Room to move as required */
      player_y+=2;
      player_x-=2;
    }
    else if( player_y == 190 && player_x == 0 )
    {
      /* Hit the very corner */
      direction = DIRECTION_NE;
    }
    else
    {
      if( player_y == 190 )
      {
	/* Hit the floor */
        player_y+=2;
        player_x-=2;
	direction = DIRECTION_NW;
      }
      else
      {
	/* Hit left side wall */
        player_y+=2;
        player_x+=2;
	direction = DIRECTION_SE;
      }
    }
  }
  else if( ((input == DIRECTION_SE) || (input == DIRECTION_STATIONARY && direction == DIRECTION_SE)) )
  {
    direction = DIRECTION_SE;
    if( player_y < 190 && player_x < 254 )
    {
      /* Room to move as required */
      player_y+=2;
      player_x+=2;
    }
    else if( player_y == 190 && player_x == 254 )
    {
      /* Hit the very corner */
      direction = DIRECTION_NW;
    }
    else
    {
      if( player_y == 190 )
      {
	/* Hit the floor */
        player_y-=2;
        player_x+=2;
	direction = DIRECTION_NE;
      }
      else
      {
	/* Hit the right side wall */
        player_y+=2;
        player_x-=2;
	direction = DIRECTION_SW;
      }
    }
  }

  /* Return the level bump magic key */
  return ( in_key_pressed( IN_KEY_SCANCODE_1 ) );
}


/*
 * This clear-and-redraw code is called form interrupt, so 50 times a
 * second. Depending on how long it takes to reposition and redraw the
 * swarm the player's key scanning and repositioning might not have
 * happened. (This isn't a 50fps game.) So only clear and redraw when
 * we know the player has actually moved.
 */
uint8_t redraw_required = 0;

void draw_player( void )
{
  uint8_t *scr_byte = screen_line_starts[player_y];

  *(scr_byte + screen_line_offsets[player_x])   |= screen_byte_values[player_x];
  *(scr_byte + screen_line_offsets[player_x+1]) |= screen_byte_values[player_x+1];

  scr_byte = screen_line_starts[player_y+1];

  *(scr_byte + screen_line_offsets[player_x])   |= screen_byte_values[player_x];
  *(scr_byte + screen_line_offsets[player_x+1]) |= screen_byte_values[player_x+1];

  previous_player_x = player_x;
  previous_player_y = player_y;

  redraw_required = 0;
}


void clear_player( void )
{
  if( (player_x == previous_player_x) && (player_y == previous_player_y) )
    return;

  uint8_t *scr_byte = screen_line_starts[previous_player_y];

  *(scr_byte + screen_line_offsets[previous_player_x])   = 0;
  *(scr_byte + screen_line_offsets[previous_player_x+1]) = 0;

  scr_byte = screen_line_starts[previous_player_y+1];

  *(scr_byte + screen_line_offsets[previous_player_x])   = 0;
  *(scr_byte + screen_line_offsets[previous_player_x+1]) = 0;

  redraw_required = 1;
}
