// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

// Forward declaration instead of including pongis.h
typedef struct GameState GameState;

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

extern const Level  levels[];
extern const uint64_t level_count;

void load_level(GameState *state, int index);

#endif // LEVEL_H