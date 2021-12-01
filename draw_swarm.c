#include <stdint.h>
#include <arch/zx.h>
#include "draw_swarm.h"
#include "vectors.h"

/*
 * Table lookup is much faster than the call to zx_pxy2saddr(), but it
 * costs memory. Not much choice though. Stopwatch says 3.9ms seconds
 * with the lookup vs 10ms with the call.
 */
#define USE_LOOKUP 1

#if USE_LOOKUP
/*
 * Line starts is the display file address. column 0.
 * Line offsets is the byte (0-31) along the display file line.
 * Byte values is the value to write into the byte to set the bit
 */
uint8_t *screen_line_starts[192];
uint8_t  screen_line_offsets[256];
uint8_t  screen_byte_values[256];
#endif

void init_draw_swarm(void)
{
#if USE_LOOKUP
  uint16_t i;
  for(i=0;i<192;i++)
    screen_line_starts[i] = zx_pxy2saddr( 0, i );

  for(i=0;i<256;i++)
  {
    /* Tried merging these into one table, but the multiply-by-2 on lookup cost too much */
    screen_line_offsets[i] = i >> 3;
    screen_byte_values[i]  = 0x80 >> (i & 0x07);
  }
#endif
}

void draw_swarm( Draw_Mode mode )
{
  extern Vector swarm[NUM_IN_SWARM];

  uint8_t i;
  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    uint8_t x = swarm[i].x_i;
    uint8_t y = swarm[i].y_i;
#if USE_LOOKUP
    uint8_t *scr_byte = screen_line_starts[y];
    scr_byte += screen_line_offsets[x];

    if( mode == OR_MODE )
      *scr_byte |= screen_byte_values[x];
    else
      *scr_byte ^= screen_byte_values[x];
#else
    uint8_t *scr_byte = zx_pxy2saddr( x, y );
    uint8_t  val      = 0x80 >> (x & 0x07);

    if( mode == OR_MODE )
      *scr_byte |= val;
    else
      *scr_byte ^= val;
#endif

  }
}

void draw_swarm_or(void)
{
  extern Vector swarm[NUM_IN_SWARM];

  uint8_t i;
  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    uint8_t x = swarm[i].x_i;
    uint8_t y = swarm[i].y_i;
#if USE_LOOKUP
    uint8_t *scr_byte = screen_line_starts[y];
    scr_byte += screen_line_offsets[x];

    *scr_byte |= screen_byte_values[x];
#else
    uint8_t *scr_byte = zx_pxy2saddr( x, y );
    uint8_t  val      = 0x80 >> (x & 0x07);

    *scr_byte |= val;
#endif

  }
}

void clear_swarm(void)
{
  extern Vector previous_swarm[NUM_IN_SWARM];

  uint8_t i;
  for( i=0; i<NUM_IN_SWARM; i++ )
  {
    uint8_t x = previous_swarm[i].x_i;
    uint8_t y = previous_swarm[i].y_i;
#if USE_LOOKUP
    uint8_t *scr_byte = screen_line_starts[y];
    scr_byte += screen_line_offsets[x];

    *scr_byte = 0;
#else
    uint8_t *scr_byte = zx_pxy2saddr( x, y );
    //uint8_t  val      = 0x80 >> (x & 0x07);

    *scr_byte = 0;
#endif

  }
}

