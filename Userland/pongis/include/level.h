// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include "pongis_types.h"

/*
typedef struct {
    int x, y, w, h;
} Obstacle;
 */
typedef struct {
    Point ball_start;
    Point player_start[MAX_PLAYERS];
    Point hole;
    int holeRadius;
    //int numObstacles;
    //const Obstacle *obstacles;
} Level;

extern const Level levels[];
extern const uint64_t level_count;

void load_level(GameState *state, int index);

#endif // LEVEL_H