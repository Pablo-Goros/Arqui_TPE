// levels.c
#include "level.h"

/* define obstacles for level 0 */
static const Obstacle lvl0Obs[] = {
    { 50,  50, 100,  10 },
    {200,  80,  20, 150 },
    /* … */
};

/* define obstacles for level 1 */
static const Obstacle lvl1Obs[] = {
    { 30, 100,  80,  10 },
    /* … */
};

/* the master array of levels */
const Level levels[] = {
    { .numObstacles = sizeof(lvl0Obs)/sizeof(*lvl0Obs),
      .obstacles    = lvl0Obs,
      .ballStartX   = 60,
      .ballStartY   = 60 },
    { .numObstacles = sizeof(lvl1Obs)/sizeof(*lvl1Obs),
      .obstacles    = lvl1Obs,
      .ballStartX   = 40,
      .ballStartY   = 80 },
    /* add more levels here */
};

const size_t level_count = sizeof(levels)/sizeof(*levels);

/* optional helper implementation */
void load_level(int index) {
    if (index < 0 || index >= (int)level_count) return;
    const Level *L = &levels[index];
    /* copy L->obstacles into your game’s obstacle array,
       set ball position to L->ballStartX/Y, etc. */
}
