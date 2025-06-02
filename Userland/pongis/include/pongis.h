/* game.h */

#ifndef PONGIS_H
#define PONGIS_H

#include <stdint.h>
#include "libc.h"
#include "level.h"
#include "shell.h"
#include "bitmaps.h"
#include "math.h"

// Physics constants 
#define ACCELERATION   0.2f   /* px/tick² */
#define MAX_SPEED      5.0f   /* px/tick */
#define FRICTION       0.99f  /* velocity scaling per frame */
#define BALL_RADIUS    8      /* px */
#define PLAYER_RADIUS 10     /* px */

typedef struct {
    int     currentLevel;

    int     ball_x,    ball_y, ball_radius;
    int     ball_vel_x,    ball_vel_y;

    int     player_x, player_y;
    int     player_vel_x, player_vel_y;

    int             holeX, holeY;
    int             holeRadius;
} GameState;

int pongis(ModeInfo mode);



#endif 
