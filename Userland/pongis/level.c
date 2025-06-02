#include "level.h"
#include "pongis.h"    // for GameState definition

/*——————————————————————————————
  Define obstacles for Level 0
  ——————————————————————————————*/
static const Obstacle lvl0_obs[] = {
    {  50,  50, 100,  10 },   // horizontal bar
    { 200,  80,  20, 150 }    // vertical wall
};

/* Level 0: ball at (60, 60), player at (120, 60), hole at (300, 200) radius 12 */
static const Level lvl0 = {
    .ball_start_x    =  60,
    .ball_start_y    =  60,
    .player_start_x  = 120,
    .player_start_y  =  60,
    .holeX           = 300,
    .holeY           = 200,
    .holeRadius      =  12,
    .numObstacles    = sizeof(lvl0_obs) / sizeof(lvl0_obs[0]),
    .obstacles       = lvl0_obs
};


/*——————————————————————————————
  Define obstacles for Level 1
  ——————————————————————————————*/
static const Obstacle lvl1_obs[] = {
    {  30, 100,  80,  10 },
    { 150, 150,  50,  10 },
    { 200,  30,  10, 100 }
};

/* Level 1: ball at (40, 80), player at (80, 40), hole at (100, 300) radius 10 */
static const Level lvl1 = {
    .ball_start_x    =  40,
    .ball_start_y    =  80,
    .player_start_x  =  80,
    .player_start_y  =  40,
    .holeX           = 100,
    .holeY           = 300,
    .holeRadius      =  10,
    .numObstacles    = sizeof(lvl1_obs) / sizeof(lvl1_obs[0]),
    .obstacles       = lvl1_obs
};


/*——————————————————————————————
  Define obstacles for Level 2
  (no obstacles, just a bigger hole)
  ——————————————————————————————*/
static const Level lvl2 = {
    .ball_start_x    = 100,
    .ball_start_y    = 100,
    .player_start_x  = 200,
    .player_start_y  = 200,
    .holeX           = 400,
    .holeY           = 200,
    .holeRadius      =  15,
    .numObstacles    =  0,
    .obstacles       = 0
};


/*——————————————————————————————
  Master array of levels (append more as desired)
  ——————————————————————————————*/
Level levels[] = {
    lvl0,
    lvl1,
    lvl2
};

uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index) {
    if (index < 0 || index >= (int)level_count) return;

    state->currentLevel = index;

    state->ball_x =  levels[index]->ball_start_x;
    state->ball_y =  levels[index]->ball_start_y;

    state->player_x = levels[index]->player_start_x;
    state->player_y = levels[index]->player_start_y;

    state->ball_vel_x  = state->ball_vel_y = 0.0f;
    state->player_vel_x  = state->player_vel_y = 0.0f;

    state->holeX = levels[index]->holeX;
    state->holeY = levels[index]->holeY;
    state->holeRadius = levels[index]->holeRadius;
}
