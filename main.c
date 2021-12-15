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

  /* Create a pool of random numbers for the jitter. These are added to the velocity */
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
    swarm[i].x_i = rand()%256; swarm[i].velocity_x = 100;
    swarm[i].y_i = rand()%192; swarm[i].velocity_y = 100;
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
      /*
       * Move towards player. Calculate distance to player, take 1/256th of it. Add that to velocity.
       */
      swarm[i].velocity_x += (player_x_i*(int16_t)256 - swarm[i].x_i*(int16_t)256)/(int16_t)256;
      swarm[i].velocity_y += (player_y_i*(int16_t)256 - swarm[i].y_i*(int16_t)256)/(int16_t)256;


      /*
       * Limit velocity
       */
      const int16_t SPEED_LIMIT = 400;
      if( swarm[i].velocity_x > SPEED_LIMIT || swarm[i].velocity_x < -SPEED_LIMIT )
      {
	if( swarm[i].velocity_x / 2 != 0 )
	  swarm[i].velocity_x /= 2;
      }
      if( swarm[i].velocity_y > SPEED_LIMIT || swarm[i].velocity_y < -SPEED_LIMIT )
      {
	if( swarm[i].velocity_y / 2 != 0 )
	  swarm[i].velocity_y /= 2;
      }


      /*
       * Original algorithm attempts to move dots so they don't get too close
       * to each other. That's not posssible on the Spectrum. I get the milling
       * around behaviour by introducing some random jitter.
       */
      swarm[i].velocity_x += *(random_values+(rand()/256));
      swarm[i].velocity_y += *(random_values+(rand()/256));


      /* Finally, add calculated velocity to dot position */
      swarm[i].x_i += swarm[i].velocity_x/100;
      swarm[i].y_i += swarm[i].velocity_y/100;
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
