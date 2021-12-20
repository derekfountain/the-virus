#include <stdint.h>
#include <arch/zx.h>
#include "level.h"

void apply_virion_logic( LEVEL *level, VIRION *v )
{
  if( ! v->active )
    return;

  if( v->x_i < 0 || v->x_i > 255
      ||
      v->y_i < 0 || v->y_i > 191 )
    return;

  uint8_t x = v->x_i;
  uint8_t y = v->y_i;

  uint8_t attribute = *(zx_pxy2aaddr(x,y));

  if( attribute == PAPER_RED )
  {
    v->active = 0;
    level->current_num_virions--;
  }
  else if( attribute == PAPER_GREEN )
  {
    
  }
  
}
