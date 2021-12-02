#ifndef __VECTORS_H
#define __VECTORS_H

#include <stdint.h>
#include <math.h>

typedef struct _vector
{
  half_t x_f;
  half_t y_f;

  int16_t x_i;
  int16_t y_i;

  half_t velocity_x;
  half_t velocity_y;
} Vector;

#endif
