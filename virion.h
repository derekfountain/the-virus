#ifndef __VIRION_H
#define __VIRION_H

#include <stdint.h>
#include <math.h>

/*
 * A virion is a single particle of a virus. We all knew that, right?
 */
typedef struct _virion
{
  int16_t x_i;
  int16_t y_i;

  half_t  x_f;
  half_t  y_f;

  int16_t  velocity_x;
  int16_t  velocity_y;

  uint8_t active;

} VIRION;

#endif
