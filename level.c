#include <stdint.h>
#include <stdlib.h>
#include <arch/zx.h>
#include "level.h"
#include "levels.h"
#include "main.h"
#include "int.h"
#include "swarm.h"

uint8_t  current_frame;
uint16_t frames_before_change;

void init_level( LEVEL *level )
{
  zx_border( level->border_colour );

  set_swarm_size( level->starting_num_virions );
  current_frame        = 0;
  frames_before_change = 0;
 
  (level->draw_frame)();

  return;
}

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
    /* If virion isn't immune, or its immunity has expired, deactivate it */
    if( !v->immunity_start ||
	(GET_TICKER > (v->immunity_start + level->immune_frames)) )
    {
      /* Deactivate this one */
      deactivate_virion( v );

      /* For when/if it's reactivated */
      change_immunity( v, MAKE_NON_IMMUNE );
    }
  }
  else if( attribute == PAPER_GREEN )
  {
    if( get_active_swarm_size() < level->max_virions )
    {
      /* Active any currently inactive virion in the swarm, it starts off immune */
      activate_virion_in_swarm( START_IMMUNE );

      /*
       * Virion which has triggered the new one becomes immune so it doesn't create lots
       * while it's in the activation area
       */
      change_immunity( v, MAKE_IMMUNE );
    }
  }
  else if( attribute == PAPER_BLUE )
  {
    v->x_i = rand()&255;
    v->y_i = rand()&191;

    v->velocity_x = 0;
    v->velocity_y = 0;

    v->immunity_start = GET_TICKER;
    
    v->previous_x_i = -1;
    v->previous_y_i = -1;
  }
  else if( attribute == PAPER_BLACK )
  {
    /*
     * I'd prefer a better "bounce" dynamic, but it's expensive to work out
     * where the virion has come from and hence which direction it should
     * bounce off in. This produces a reasonable approximation.
     * No immunity here, which means they tend to get a bit stuck under the
     * black cell, which actually works nicely.
     */
    v->velocity_x = -(v->velocity_x);
    v->velocity_y = -(v->velocity_y);
  }
}
