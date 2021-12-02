#include <stdint.h>
#include <arch/zx.h>
#include "draw_swarm.h"

/*
 * Line starts is the display file address. column 0.
 * Line offsets is the byte (0-31) along the display file line.
 * Byte values is the value to write into the byte to set the bit
 */
uint8_t *screen_line_starts[192];
uint8_t  screen_line_offsets[256];
uint8_t  screen_byte_values[256];

void init_draw_swarm(void)
{
  uint16_t i;
  for(i=0;i<192;i++)
    screen_line_starts[i] = zx_pxy2saddr( 0, i );

  for(i=0;i<256;i++)
  {
    /* Tried merging these into one table, but the multiply-by-2 on lookup cost too much */
    screen_line_offsets[i] = i >> 3;
    screen_byte_values[i]  = 0x80 >> (i & 0x07);
  }
}

void draw_swarm( Draw_Mode mode )
{
  extern int16_t swarm_x_i[MAX_IN_SWARM];
  extern int16_t swarm_y_i[MAX_IN_SWARM];

  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( swarm_x_i[i] < 0 || swarm_x_i[i] > 255
	||
	swarm_y_i[i] < 0 || swarm_y_i[i] > 191 )
      continue;

    uint8_t x = swarm_x_i[i];
    uint8_t y = swarm_y_i[i];

    uint8_t *scr_byte = screen_line_starts[y];
    scr_byte += screen_line_offsets[x];

    if( mode == OR_MODE )
      *scr_byte |= screen_byte_values[x];
    else
      *scr_byte ^= screen_byte_values[x];
  }
}

void draw_swarm_or(void)
{
  extern int16_t swarm_x_i[MAX_IN_SWARM];
  extern int16_t swarm_y_i[MAX_IN_SWARM];

  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( swarm_x_i[i] < 0 || swarm_x_i[i] > 255
	||
	swarm_y_i[i] < 0 || swarm_y_i[i] > 191 )
      continue;

    uint8_t x = swarm_x_i[i];
    uint8_t y = swarm_y_i[i];

    uint8_t *scr_byte = screen_line_starts[y];
    scr_byte += screen_line_offsets[x];

    *scr_byte |= screen_byte_values[x];
  }
}


void clear_swarm(void)
{
  extern int16_t previous_swarm_x_i[MAX_IN_SWARM];
  extern int16_t previous_swarm_y_i[MAX_IN_SWARM];

  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( previous_swarm_x_i[i] < 0 || previous_swarm_x_i[i] > 255
	||
	previous_swarm_y_i[i] < 0 || previous_swarm_y_i[i] > 191 )
      continue;

    uint8_t x = previous_swarm_x_i[i];
    uint8_t y = previous_swarm_y_i[i];

    uint8_t *scr_byte = zx_pxy2saddr( x, y );

    *scr_byte = 0;
  }
}

void draw_player( uint8_t x, uint8_t y )
{
  uint8_t *scr_byte = screen_line_starts[y];

  *(scr_byte + screen_line_offsets[x])   |= screen_byte_values[x];
  *(scr_byte + screen_line_offsets[x+1]) |= screen_byte_values[x+1];

  scr_byte = screen_line_starts[y+1];

  *(scr_byte + screen_line_offsets[x])   |= screen_byte_values[x];
  *(scr_byte + screen_line_offsets[x+1]) |= screen_byte_values[x+1];
}


void clear_player( uint8_t x, uint8_t y )
{
  uint8_t *scr_byte = screen_line_starts[y];

  *(scr_byte + screen_line_offsets[x])   = 0;
  *(scr_byte + screen_line_offsets[x+1]) = 0;

  scr_byte = screen_line_starts[y+1];

  *(scr_byte + screen_line_offsets[x])   = 0;
  *(scr_byte + screen_line_offsets[x+1]) = 0;
}
