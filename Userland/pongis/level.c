#include "level.h"

// level 1 
static const Obstacle lvl_1[] = {
    { 50,  50, 100,  10 },
    {200,  80,  20, 150 },

};

// level 2 
static const Obstacle lvl_2[] = {
    { 30, 100,  80,  10 },

};

/* the master array of levels */
const Level levels[] = {
    // level 1
    { .numObstacles = sizeof(lvl_1)/sizeof(*lvl_1),
      .obstacles    = lvl_1,
      .ballStartX   = 60,
      .ballStartY   = 60 },
    // level 2
    { .numObstacles = sizeof(lvl_2)/sizeof(*lvl_2),
      .obstacles    = lvl_2,
      .ballStartX   = 40,
      .ballStartY   = 80 },
    // level 3

};

const uint64_t level_count = sizeof(levels)/sizeof(*levels);

/* optional helper implementation */
void load_level(int index) {
    if (index < 0 || index >= (int)level_count) return;
    const Level *L = &levels[index];
    /* copy L->obstacles into your game’s obstacle array,
       set ball position to L->ballStartX/Y, etc. */
}
