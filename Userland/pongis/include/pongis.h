/* game.h */

#ifndef PONGIS_H
#define PONGIS_H

#include <stdint.h>
#include "libc.h"
#include "level.h"
#include "shell.h"
#include "bitmaps.h"
#include "math.h"

// Physics constants (tweak as you iterate) 
#define ACCELERATION   0.2f   /* px/tick² */
#define MAX_SPEED      5.0f   /* px/tick */
#define FRICTION       0.99f  /* velocity scaling per frame */
#define BALL_RADIUS    8      /* px */


int pongis(ModeInfo mode);

void pongis_init(void);
#endif /* GAME_H */
