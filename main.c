#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arch/zx.h>
#include <input.h>
#include <math.h>
#include <sys/ioctl.h>
#include <intrinsic.h>
#include <z80.h>
#include <im2.h>

#include "main.h"
#include "virion.h"
#include "draw_swarm.h"

unsigned char version[8] = "ver0.01";

VIRION          swarm[MAX_IN_SWARM];
VIRION previous_swarm[MAX_IN_SWARM];

int16_t  player_x_i;
int16_t  player_y_i;
int16_t  previous_player_x_i;
int16_t  previous_player_y_i;

int16_t  move_to_player_x_i;
int16_t  move_to_player_y_i;

int16_t  random_values[255];


/*
 * Standard interrupt set up for now.
 */
#define TABLE_HIGH_BYTE        ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE   ((unsigned int)0xD1)

#define UI_256                 ((unsigned int)256)
#define TABLE_ADDR             ((void*)(TABLE_HIGH_BYTE*UI_256))
#define JUMP_POINT             ((unsigned char*)( (unsigned int)(JUMP_POINT_HIGH_BYTE*UI_256) + JUMP_POINT_HIGH_BYTE ))

uint8_t int_fired = 0;
IM2_DEFINE_ISR(isr)
{
  int_fired = 1;
}

void setup_int(void)
{
  memset( TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257 );
  z80_bpoke( JUMP_POINT,   195 );
  z80_wpoke( JUMP_POINT+1, (unsigned int)isr );
  im2_init( TABLE_ADDR );
  intrinsic_ei();
}

static uint32_t usqrt4(uint32_t val) {
    uint32_t a, b;

    if (val < 2) return val; /* avoid div/0 */

    a = (uint32_t)1255;       /* starting point is relatively unimportant */

    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;

    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;

    return a;
}

#pragma printf %f %lf %d %ld

void main(void)
{
  uint16_t i;

  zx_border( INK_BLUE );
  zx_cls( PAPER_WHITE );

  ioctl(1, IOCTL_OTERM_PAUSE, 0);

  // Middle of screen for now.
  player_x_i = 128;
  player_y_i =  96;

  *(zx_cxy2aaddr(5,5)) = PAPER_RED;

  /* Create a pool of random numbers for the jitter. These are added to the velocity occasionally */
  for(i=0;i<256;i++)
  {
    random_values[i] = rand()%100;
    if( i & 0x01 )
      random_values[i] = -(random_values[i]);
    // printf("%d ", random_values[i]);
  }

  init_draw_swarm();

  /* Starting points */
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    swarm[i].x_i = rand()%256; swarm[i].x_f = f16_u16( swarm[i].x_i ); swarm[i].velocity_x = 0;
    swarm[i].y_i = rand()%192; swarm[i].y_f = f16_u16( swarm[i].y_i ); swarm[i].velocity_y = 0;
    swarm[i].active = 1;
  }

  /* Currently unused, but useful for test and timing checks */
  intrinsic_ei();

#define TIME_TEST 0
#if TIME_TEST
  uint16_t countdown = 500;
#endif

  uint8_t bump=0;
  while(1
#if TIME_TEST
	&& --countdown
#endif
    )
  {
    int i;

    if( in_key_pressed( IN_KEY_SCANCODE_q ) && player_y_i )       player_y_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_a ) && player_y_i < 190 ) player_y_i+=2;
    if( in_key_pressed( IN_KEY_SCANCODE_o ) && player_x_i )       player_x_i-=2;
    if( in_key_pressed( IN_KEY_SCANCODE_p ) && player_x_i < 254 ) player_x_i+=2;

    for( i=0; i < MAX_IN_SWARM; i++ )
    {
      swarm[i].active=1;
//      if( !swarm[i].active )
//	continue;

      /*
       * Original algorithm attempts to move dots so they don't get too close
       * to each other. That's not posssible on the Spectrum. I get the milling
       * around behaviour by introducing some random jitter.
       */
      if(0 && bump++ & 0x04 )
      {
	uint16_t r = rand() / 256;

	uint16_t t = *(random_values+r);
	swarm[i].velocity_x += t;
	r = rand() / 256;
	t = *(random_values+r);
	swarm[i].velocity_y += t;
      }


      /*
       * Move to player. Calculate distance to player, take 1% of it. Add that to velocity.
       */
      move_to_player_x_i = player_x_i*100 - swarm[i].x_i*100;
      move_to_player_y_i = player_y_i*100 - swarm[i].y_i*100;
//      printf("%d: MTP dot at %d,%d, distance x,y=%d,%d v is %d,%d\n",
//	     i,
//	     swarm[i].x_i, swarm[i].y_i,
//	     move_to_player_x_i, move_to_player_y_i,
//	     swarm[i].velocity_x, swarm[i].velocity_y);

      int16_t t_x = move_to_player_x_i/100;
      int16_t t_y = move_to_player_y_i/100;
//      printf("%d: scaled, that's x,y=%d,%d\n", i, t_x, t_y);

      swarm[i].velocity_x += t_x;
      swarm[i].velocity_y += t_y;
//      printf("%d: final move to player vel vx,vy=%d,%d\n\n", i, swarm[i].velocity_x, swarm[i].velocity_y);




      /*
       * Limit velocity
       */
      const int32_t SPEED_LIMIT = 250;
      uint32_t mag = usqrt4( ((uint32_t)swarm[i].velocity_x * (uint32_t)swarm[i].velocity_x)
			     +
 			     ((uint32_t)swarm[i].velocity_y * (uint32_t)swarm[i].velocity_y)
			     );
      if( mag > SPEED_LIMIT )
      {
//	printf("%d: preg vx,vy=%d,%d mag %ld\n", i, swarm[i].velocity_x, swarm[i].velocity_y, mag);
        int32_t dx = ((int32_t)swarm[i].velocity_x)*(int32_t)100;
	int32_t dy = ((int32_t)swarm[i].velocity_y)*(int32_t)100;

//	printf("%d: int1 dx,dy=%ld,%ld mag %ld\n", i, dx, dy, mag);

        dx = dx/(int32_t)mag;
	dy = dy/(int32_t)mag;

//	printf("%d: int2 dx,dy=%ld,%ld mag %ld\n", i, dx, dy, mag);

	dx *= SPEED_LIMIT;
	dy *= SPEED_LIMIT;

//	printf("%d: int3 dx,dy=%ld,%ld mag %ld\n", i, dx, dy, mag);

	dx /= (int32_t)100;
	dy /= (int32_t)100;

//	printf("%d: int4 dx,dy=%ld,%ld mag %ld\n", i, dx, dy, mag);

	swarm[i].velocity_x = (int16_t)((dx)*(int32_t)1);
	swarm[i].velocity_y = (int16_t)((dy)*(int32_t)1);
//      printf("%d: post vx,vy=%d,%d\n", i, swarm[i].velocity_x, swarm[i].velocity_y);
      }





      //    printf("%d: adding vx,vy=%d,%d div 100\n", i, swarm[i].velocity_x, swarm[i].velocity_y);
      /* Finally, add calculated velocity to dot position */
      swarm[i].x_i += swarm[i].velocity_x/100;
      swarm[i].y_i += swarm[i].velocity_y/100;
      //printf("%d: putting dot at x,y=%d,%d\n\n", i, swarm[i].x_i, swarm[i].y_i);

    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();
    clear_player( previous_player_x_i, previous_player_y_i );

    /* Draw the newly computed swarm - also hard coded */
    draw_player( player_x_i, player_y_i );
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    memcpy( previous_swarm, swarm, sizeof(swarm) );
    previous_player_x_i = player_x_i;
    previous_player_y_i = player_y_i;
  }
}
