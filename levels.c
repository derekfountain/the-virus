#include <stdint.h>
#include <arch/zx.h>
#include "main.h"
#include "level.h"
#include "levels.h"

LEVEL levels[] = 
{
  {MAX_IN_SWARM, MAX_IN_SWARM,
   100,
   INK_BLUE,
   50,
   draw_level0_frame,
  },
  {8, MAX_IN_SWARM,
   100,
   INK_RED,
   50,
   draw_level1_frame,
  },
};


uint8_t select_level(void)
{
  return 0;
}


LEVEL *get_level( uint8_t l )
{
  return &levels[l];
}


void draw_level0_frame(void)
{
  *(zx_cxy2aaddr(5,5)) = PAPER_RED;
}

void draw_level1_frame(void)
{
  *(zx_cxy2aaddr(25,15)) = PAPER_GREEN;
}

