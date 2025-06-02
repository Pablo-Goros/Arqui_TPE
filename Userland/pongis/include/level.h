// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include "pongis.h"

typedef struct {
    int x, y, w, h;
} Obstacle;

typedef struct {
    int             ball_start_x, ball_start_y;
    int             player_start_x, player_start_y;

    int             holeX, holeY;
    int             holeRadius;

    int             numObstacles;
    const Obstacle *obstacles;
    // add lo q falte (e.g. terrain flags, etc.)
} Level;


void load_level(GameState *state, int index);

#endif // LEVEL_H