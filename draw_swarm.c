#include <stdint.h>
#include <arch/zx.h>
#include "virion.h"
#include "draw_swarm.h"

/*
 * Line starts is the display file address. column 0.
 * Line offsets is the byte (0-31) along the display file line.
 * Byte values is the value to write into the byte to set the bit
 */
uint8_t *screen_line_starts[192];
uint8_t  screen_line_offsets[256];
uint8_t  screen_byte_values[256];

/*
 * Initialise the lookup tables
 */
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


void draw_swarm_or(void)
{
  extern VIRION swarm[MAX_IN_SWARM];

  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( !swarm[i].active )
      continue;

    if( swarm[i].x_i < 0 || swarm[i].x_i > 255
	||
	swarm[i].y_i < 0 || swarm[i].y_i > 191 )
      continue;

    uint8_t x = swarm[i].x_i;
    uint8_t y = swarm[i].y_i;

    if( *(zx_pxy2aaddr(x,y)) == PAPER_RED )
    {
      swarm[i].active = 0;
    }
    else
    {
      uint8_t *scr_byte = screen_line_starts[y];
      scr_byte += screen_line_offsets[x];

      *scr_byte |= screen_byte_values[x];
    }
  }
}


void clear_swarm(void)
{
  extern VIRION previous_swarm[MAX_IN_SWARM];

  uint8_t i;
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    if( !previous_swarm[i].active )
      continue;

    if( previous_swarm[i].x_i < 0 || previous_swarm[i].x_i > 255
	||
	previous_swarm[i].y_i < 0 || previous_swarm[i].y_i > 191 )
      continue;

    uint8_t x = previous_swarm[i].x_i;
    uint8_t y = previous_swarm[i].y_i;

    uint8_t *scr_byte = zx_pxy2saddr( x, y );

    *scr_byte = 0;
  }
}

void clear_virion( VIRION *v )
{
  if( ! v->active )
    return;

  if( v->previous_x_i < 0 || v->previous_x_i > 255
      ||
      v->previous_y_i < 0 || v->previous_y_i > 191 )
    return;

  uint8_t x = v->previous_x_i;
  uint8_t y = v->previous_y_i;

  /* I could cache this screen address... */
  uint8_t *scr_byte = screen_line_starts[y];
  scr_byte += screen_line_offsets[x];

  /*
   * I can't XOR this because if two virions are in the same place
   * they go off and back on again and I end up with trails of dots.
   */
  *scr_byte &= ~screen_byte_values[x];
}

void draw_virion( VIRION *v )
{
  if( ! v->active )
    return;

  if( v->x_i < 0 || v->x_i > 255
      ||
      v->y_i < 0 || v->y_i > 191 )
    return;

  uint8_t x = v->x_i;
  uint8_t y = v->y_i;

  uint8_t *scr_byte = screen_line_starts[y];
  scr_byte += screen_line_offsets[x];

  *scr_byte |= screen_byte_values[x];
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
