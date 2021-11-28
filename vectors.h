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

half_t  vector_distance_f( Vector *v1, Vector *v2 );
int16_t vector_distance_i( Vector *v1, Vector *v2 );

Vector  *vector_add_f( Vector *v1, Vector *v2 );

Vector  *vector_sub_f( Vector *v1, Vector *v2 );
Vector  *vector_sub_i( Vector *v1, Vector *v2 );

Vector  *vector_mul_f( Vector *v1, half_t mul );

Vector  *vector_div_f( Vector *v1, half_t fac );
Vector  *vector_div_i( Vector *v1, int16_t fac );


void test_vector_distance_f(void);

#endif
