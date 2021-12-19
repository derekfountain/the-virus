#ifndef __CONTROLS_H
#define __CONTROLS_H

typedef enum _control
{
  KEYBOARD,
  JOYSTICK
} CONTROL;

CONTROL select_controls(void);

#endif
