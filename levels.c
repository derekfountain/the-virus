#include <stdint.h>
#include <arch/zx.h>
#include "main.h"
#include "level.h"
#include "levels.h"

LEVEL levels[] = 
{
  {
    NAMED_ARG("Starting num virions", MAX_IN_SWARM),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_BLUE),
    NAMED_ARG("Immune frames",        50),
    NAMED_ARG("Draw frame handler",   draw_level0_frame),
  },
  {
    NAMED_ARG("Starting num virions", 8),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_MAGENTA),
    NAMED_ARG("Immune frames",        50),
    NAMED_ARG("Draw frame handler",   draw_level1_frame),
  },
  {
    NAMED_ARG("Starting num virions", 12),
    NAMED_ARG("Max num virions",      MAX_IN_SWARM),
    NAMED_ARG("Starting velocity",    100),
    NAMED_ARG("Border colour",        INK_RED),
    NAMED_ARG("Immune frames",        50),
    NAMED_ARG("Draw frame handler",   draw_level2_frame),
  },
};

uint8_t select_level(void)
{
  return 0;
}


LEVEL *get_level( uint8_t lev )
{
  return &levels[lev];
}


void draw_level0_frame(void)
{
  *(zx_cxy2aaddr(5,5))  = PAPER_RED;
}

void draw_level1_frame(void)
{
  *(zx_cxy2aaddr(5,5))   = PAPER_RED;
  *(zx_cxy2aaddr(25,15)) = PAPER_GREEN;
}

void draw_level2_frame(void)
{
  *(zx_cxy2aaddr(5,5))   = PAPER_RED;
  *(zx_cxy2aaddr(5,15))  = PAPER_BLACK;
  *(zx_cxy2aaddr(25,15)) = PAPER_GREEN;
}
