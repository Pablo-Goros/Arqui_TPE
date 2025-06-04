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


#define PLAYER_COLOR    0x0000FF00  // Green player
#define BALL_COLOR      0x00FFFFFF  // White ball
#define HOLE_COLOR      0x00808080  // Grey hole

typedef enum {
    GAME_PLAYING,
    GAME_LEVEL_COMPLETE,
    GAME_ALL_COMPLETE
} GamePhase;

typedef struct GameState {
    int     currentLevel;

    int     ball_x,    ball_y;
    float     ball_vel_x,    ball_vel_y;

    int     player_x, player_y;
    float     player_vel_x, player_vel_y;

    int             holeX, holeY;
    int             holeRadius;
} GameState;

void pongis(ModeInfo mode);

void pongis_init();

void draw_player(int x, int y, int radius); 

void draw_ball(int x, int y, int radius);

void draw_hole(int x, int y, int radius);
#endif 
