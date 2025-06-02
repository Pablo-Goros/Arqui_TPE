// level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

typedef struct {
    int x, y, w, h;
} Obstacle;

typedef struct {
    int         numObstacles;
    const Obstacle *obstacles;
    int         ballStartX, ballStartY;
    int         playerStartX, playerStartY;
    int         holeStartX, holeStartY;
    int         holeRadius;

    // add lo q falte (e.g. terrain flags, etc.)
} Level;

// y esto?
extern const Level levels[];
extern const uint64_t level_count;

void load_level(int index);

#endif // LEVEL_H