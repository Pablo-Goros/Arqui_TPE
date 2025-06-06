// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include "pongis_lib.h"

// Forward declaration instead of including pongis.h
typedef struct GameState GameState;
typedef struct Point Point;



typedef struct {
    int x, y, w, h;
} Obstacle;


typedef struct {
    Point ball_start;

    Point player_start[MAX_PLAYERS];

    Point hole;
    int             holeRadius;

    int             numObstacles;
    const Obstacle *obstacles;
    // add lo q falte (e.g. terrain flags, etc.)
} Level;

extern const Level  levels[];
extern const uint64_t level_count;

void load_level(GameState *state, int index);

#endif // LEVEL_H