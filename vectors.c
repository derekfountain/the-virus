#include "vectors.h"
#include <stdio.h>

#define ENABLE_TEST 0
#if ENABLE_TEST
#pragma printf %f

#include <stdio.h>
#endif

half_t vector_distance_f( Vector *v1, Vector *v2 )
{
  half_t x_sq = mulf16( subf16(v1->x_f, v2->x_f), subf16(v1->x_f, v2->x_f) );
  half_t y_sq = mulf16( subf16(v1->y_f, v2->y_f), subf16(v1->y_f, v2->y_f) );

  return addf16( x_sq, y_sq );
}

#if ENABLE_TEST
void test_vector_distance_f(void)
{
  Vector v1;
  Vector v2;

  v1.x_f = f16_i32(50);
  v1.y_f = f16_i32(40);

  v2.x_f = f16_i32(100);
  v2.y_f = f16_i32(80);

  half_t r = vector_distance_f(&v1,&v2);
  printf("Expect 4100: %f\n", f32_f16( r ) );

  r = vector_distance_f(&v2,&v1);
  printf("Expect 4100: %f\n", f32_f16( r ) );
}
#else
void test_vector_distance_f(void){}
#endif


int16_t vector_distance_i( Vector *v1, Vector *v2 )
{
  //printf("VD: %d %d   %d %d\n", v1->x_i,v1->y_i,  v2->x_i,v2->y_i );
  int16_t x_sq = (v1->x_i - v2->x_i) * (v1->x_i - v2->x_i);
  int16_t y_sq = (v1->y_i - v2->y_i) * (v1->y_i - v2->y_i);
  //printf("SM: %d %d\n", x_sq,y_sq );

  return x_sq + y_sq;
}

Vector *vector_add_f( Vector *v1, Vector *v2 )
{
  v1->x_f = v1->x_f + v2->x_f;
  v1->y_f = v1->y_f + v2->y_f;

  return v1;
}

Vector *vector_sub_f( Vector *v1, Vector *v2 )
{
  v1->x_f = subf16(v1->x_f, v2->x_f);
  v1->y_f = subf16(v1->y_f, v2->y_f);

  return v1;
}

Vector *vector_sub_i( Vector *v1, Vector *v2 )
{
  v1->x_i = v1->x_i - v2->x_i;
  v1->y_i = v1->y_i - v2->y_i;

  return v1;
}

Vector *vector_mul_f( Vector *v1, half_t mul )
{
  v1->x_f = v1->x_f * mul;
  v1->y_f = v1->y_f * mul;

  return v1;
}

Vector *vector_div_f( Vector *v1, half_t fac )
{
  v1->x_f = divf16(v1->x_f, fac);
  v1->y_f = divf16(v1->y_f, fac);

  return v1;
}

Vector *vector_div_i( Vector *v1, int16_t fac )
{
  v1->x_i = v1->x_i / fac;
  v1->y_i = v1->y_i / fac;

  return v1;
}

