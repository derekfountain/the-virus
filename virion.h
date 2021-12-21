#ifndef __VIRION_H
#define __VIRION_H

#include <stdint.h>

#define MAKE_IMMUNE     ((uint8_t)1)
#define MAKE_NON_IMMUNE ((uint8_t)0)
/*
 * A virion is a single particle of a virus. We all knew that, right?
 */
typedef struct _virion
{
  int16_t   x_i;
  int16_t   y_i;

  int16_t   velocity_x;
  int16_t   velocity_y;

  uint8_t  active;
  uint16_t immunity_start;

  int16_t  previous_x_i;
  int16_t  previous_y_i;

} VIRION;

void init_draw_virion_tables(void);
void draw_virion( VIRION* );
void clear_virion( VIRION* );
void activate_virion( VIRION* );
void deactivate_virion( VIRION* );
void change_immunity( VIRION *, uint8_t );

#endif
