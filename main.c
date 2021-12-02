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
#include "draw_swarm.h"

/* Keep these together so the copy into 'previous' can be done in one go */
int16_t swarm_x_i[MAX_IN_SWARM];
int16_t swarm_y_i[MAX_IN_SWARM];
int16_t previous_swarm_x_i[MAX_IN_SWARM];
int16_t previous_swarm_y_i[MAX_IN_SWARM];

half_t swarm_x_f[MAX_IN_SWARM];
half_t swarm_y_f[MAX_IN_SWARM];

half_t swarm_velocity_x[MAX_IN_SWARM];
half_t swarm_velocity_y[MAX_IN_SWARM];

uint8_t swarm_active[MAX_IN_SWARM];

int16_t  player_x_i;
int16_t  player_y_i;
int16_t  previous_player_x_i;
int16_t  previous_player_y_i;

int16_t  move_to_player_x_i;
int16_t  move_to_player_y_i;

half_t   random_values[255];


/*
 * Standard SP1 interrupt set up for now.
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
  uint8_t i;

  zx_border( INK_BLUE );
  zx_cls( PAPER_WHITE );

  // Middle of screen for now.
  player_x_i = 128;
  player_y_i =  96;

  *(zx_cxy2aaddr(5,5)) = PAPER_RED;

  const half_t f100 = f16_i16( 100 );

  /* Create a pool of random floating point numbers for the jitter */
  for(i=0;i<255;i++)
  {
    random_values[i] = f16_f32( (rand()%100) / 1000.0 );
    if( i & 0x01 )
      random_values[i] = negf16(random_values[i]);
    //printf("%f ", f32_f16(random_values[i]));
  }

  init_draw_swarm();
  // ioctl(1, IOCTL_OTERM_PAUSE, 0);

  /* Starting points */
  for( i=0; i<MAX_IN_SWARM; i++ )
  {
    swarm_x_i[i] = rand()%256; swarm_x_f[i] = f16_u16( swarm_x_i[i] ); swarm_velocity_x[i] = f16_f32( 0.1 );
    swarm_y_i[i] = rand()%192; swarm_y_f[i] = f16_u16( swarm_y_i[i] ); swarm_velocity_y[i] = f16_f32( 0.1 );
    swarm_active[i] = 1;
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
      if( !swarm_active[i] )
	continue;

      /*
       * Original algorithm attempts to move dots so they don't get too close
       * to each other. That's not posssible on the Spectrum. I get the milling
       * around behaviour by introducing some random jitter.
       */
      if( bump++ & 0x04 )
      {
	swarm_velocity_x[i] = addf16( swarm_velocity_x[i], random_values[rand()&0xff] );
	swarm_velocity_y[i] = addf16( swarm_velocity_y[i], random_values[rand()&0xff] );
      }


      /*
       * Move to player. Calculate distance to player, take 1% of it. Add that to velocity.
       */
      move_to_player_x_i = player_x_i - swarm_x_i[i];
      move_to_player_y_i = player_y_i - swarm_y_i[i];

      swarm_velocity_x[i] = addf16( swarm_velocity_x[i], divf16( f16_i16(move_to_player_x_i), f100 ) );
      swarm_velocity_y[i] = addf16( swarm_velocity_y[i], divf16( f16_i16(move_to_player_y_i), f100 ) );
  

      /*
       * Limit velocity
       */
      const half_t SPEED_LIMIT = f16_f32(2.75);
      if( isgreaterf16( swarm_velocity_x[i], SPEED_LIMIT ) || isgreaterf16( swarm_velocity_y[i], SPEED_LIMIT ) )
      {
	swarm_velocity_x[i] = div2f16( swarm_velocity_x[i] );
	swarm_velocity_y[i] = div2f16( swarm_velocity_y[i] );
      }


      /* Finally, add calculated velocity to dot position */
      swarm_x_f[i] = addf16( swarm_x_f[i], swarm_velocity_x[i] );
      swarm_y_f[i] = addf16( swarm_y_f[i], swarm_velocity_y[i] );

      /* Updated rounded version of position */
      swarm_x_i[i] = i16_f16( ( swarm_x_f[i] ) );
      swarm_y_i[i] = i16_f16( ( swarm_y_f[i] ) );

    }

    /* Clear the previous swarm - it's hard coded */
    clear_swarm();
    clear_player( previous_player_x_i, previous_player_y_i );

    /* Draw the newly computed swarm - also hard coded */
    draw_player( player_x_i, player_y_i );
    draw_swarm_or();

    /* Copy currently displayed swarm so the clear routine can remove it */
    memcpy( previous_swarm_x_i, swarm_x_i, sizeof(swarm_x_i)+sizeof(swarm_y_i) );
    previous_player_x_i = player_x_i;
    previous_player_y_i = player_y_i;
  }
}
