#include "level.h"
#include "pongis.h"    // for GameState definition

static const Level lvl0 = {
    .ball_start_x    = 100,
    .ball_start_y    = 100,

    .player_start_x  = 200,
    .player_start_y  = 200,

    .holeX           = 400,
    .holeY           = 200,
    .holeRadius      =  15,

    .numObstacles    =  0,
    .obstacles       = ((void*)0)
};

const Level levels[1] = {
    // Level 0: Simple level with no obstacles
    {
        .ball_start_x    = 100,
        .ball_start_y    = 100,

        .player_start_x  = 200,
        .player_start_y  = 200,

        .holeX           = 400,
        .holeY           = 200,
        .holeRadius      =  15,

        .numObstacles    =  0,
        .obstacles       = ((void*)0)
    }


};


const uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index) {
    if (index < 0 || index >= (int)level_count) return;

    state->currentLevel = index;

    state->ball_x =  levels[index].ball_start_x;
    state->ball_y =  levels[index].ball_start_y;

    state->player_x = levels[index].player_start_x;
    state->player_y = levels[index].player_start_y;

    state->ball_vel_x  = state->ball_vel_y = 0.0f;
    state->player_vel_x  = state->player_vel_y = 0.0f;

    state->holeX = levels[index].holeX;
    state->holeY = levels[index].holeY;
    state->holeRadius = levels[index].holeRadius;
}
