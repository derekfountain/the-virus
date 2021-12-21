#include <stdint.h>
#include <arch/zx.h>
#include "virion.h"
#include "int.h"
#include "swarm.h"

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
void init_draw_virion_tables(void)
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


void activate_virion( VIRION *v )
{
  if( !v->active )
  {
    v->active = 1;
    set_swarm_size( get_active_swarm_size() + 1 );
  }
}

void deactivate_virion( VIRION *v )
{
  if( v->active )
  {
    v->active = 0;
    set_swarm_size( get_active_swarm_size() - 1 );
  }
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


void change_immunity( VIRION *v, uint8_t new_state )
{
  if( new_state == MAKE_IMMUNE )
    v->immunity_start = GET_TICKER;
  else
    v->immunity_start = 0;
}

