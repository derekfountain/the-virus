#ifndef __LEVEL_H
#define __LEVEL_H

#include <stdint.h>
#include "virion.h"

typedef struct __level
{
  uint8_t  starting_num_virions;
  uint8_t  max_virions;

  uint8_t  border_colour;

  void   (*draw_frame)(void);


  /*** Variables follow ***/
  uint8_t  current_num_virions;
  uint8_t  current_frame;
  uint16_t frames_before_change;
  uint16_t immune_frames;
} LEVEL;

void apply_virion_logic( LEVEL*, VIRION* );

#endif
