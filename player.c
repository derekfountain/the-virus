#include <stdint.h>
#include "player.h"

/* These are in virion.c, bit of a cheat but I can use them here */
extern uint8_t *screen_line_starts[192];
extern uint8_t  screen_line_offsets[256];
extern uint8_t  screen_byte_values[256];

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
