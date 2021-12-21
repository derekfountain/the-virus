#include <stdint.h>
#include <arch/zx.h>
#include "main.h"
#include "level.h"
#include "levels.h"

LEVEL levels[] = 
{
  {(MAX_IN_SWARM-5), MAX_IN_SWARM,
   INK_BLUE,
   draw_level0_frame,
  },
  {8, MAX_IN_SWARM,
   INK_RED,
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

void draw_level( LEVEL *level )
{
  zx_border( level->border_colour );

  level->current_num_virions  = level->starting_num_virions;
  level->current_frame        = 0;
  level->frames_before_change = 0;
  level->immune_frames        = 0;
 
  (level->draw_frame)();

  return;
}
