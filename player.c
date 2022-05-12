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
#include <z80.h>
#include "main.h"
#include "player.h"
#include "snow.h"
#include "controls.h"

uint8_t  player_x=255;
uint8_t  player_y=255;
uint8_t  previous_player_x=255;
uint8_t  previous_player_y=255;

CONTROL   control;
DIRECTION direction;

void hide_player( void )
{
  player_x = 255;
  player_y = 255;
  clear_player();
}

void init_player( CONTROL c )
{
  /* Middle of screen */
  player_x = 128;
  player_y =  96;

  control = c;

  direction = DIRECTION_N;
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
  if( ((control == KEYBOARD) && in_key_pressed( UP_SCANCODE ))
      ||
      ((control == JOYSTICK) && (kempston_input & IN_STICK_UP)) )
  {
    input = DIRECTION_N;
  }
  else if( ((control == KEYBOARD) && in_key_pressed( DOWN_SCANCODE ))
      ||
      ((control == JOYSTICK) && (kempston_input & IN_STICK_DOWN)) )
  {
    input = DIRECTION_S;
  }

  if( ((control == KEYBOARD) && in_key_pressed( LEFT_SCANCODE ))
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
  else if( ((control == KEYBOARD) && in_key_pressed( RIGHT_SCANCODE ))
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
    if( player_y < 182 )
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
    if( player_y < 182 && player_x )
    {
      /* Room to move as required */
      player_y+=2;
      player_x-=2;
    }
    else if( player_y == 182 && player_x == 0 )
    {
      /* Hit the very corner */
      direction = DIRECTION_NE;
    }
    else
    {
      if( player_y == 182 )
      {
	/* Hit the floor */
        player_y-=2;
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
    if( player_y < 182 && player_x < 254 )
    {
      /* Room to move as required */
      player_y+=2;
      player_x+=2;
    }
    else if( player_y == 182 && player_x == 254 )
    {
      /* Hit the very corner */
      direction = DIRECTION_NW;
    }
    else
    {
      if( player_y == 182 )
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

  if( in_key_pressed( IN_KEY_SCANCODE_1 ) )
    return 1;
  else if( in_key_pressed( IN_KEY_SCANCODE_2 ) )
    return 2;
  else if( in_key_pressed( IN_KEY_SCANCODE_s ) )
  {
    /* Need to wait for the key to be released. Can do better. */
    z80_delay_ms(200);
    return 3;
  }
  else
    return 0;
}


/*
 * This clear-and-redraw code is called from interrupt, so 50 times a
 * second. Depending on how long it takes to reposition and redraw the
 * swarm the player's key scanning and repositioning might not have
 * happened. (This isn't a 50fps game.) So only clear and redraw when
 * we know the player has actually moved.
 */
uint8_t redraw_required = 0;

void draw_player( void )
{
  /* Use y-pos as a sentinel to indicate the caption is on screen */
  if( redraw_required == 0 || player_y == 255  )
    return;

  snow_plot( player_x,   player_y );
  snow_plot( player_x+1, player_y );
  snow_plot( player_x,   player_y+1 );
  snow_plot( player_x+1, player_y+1 );

  previous_player_x = player_x;
  previous_player_y = player_y;

  redraw_required = 0;
}


void clear_player( void )
{
  if( (player_x == previous_player_x) && (player_y == previous_player_y) )
    return;

  snow_unplot( previous_player_x,   previous_player_y );
  snow_unplot( previous_player_x+1, previous_player_y );
  snow_unplot( previous_player_x,   previous_player_y+1 );
  snow_unplot( previous_player_x+1, previous_player_y+1 );

  redraw_required = 1;
}
