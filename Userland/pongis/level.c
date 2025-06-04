#include "level.h"
#include "pongis.h"    // for GameState definition


const Level levels[1] = {
    // Level 0: Simple level with no obstacles
    {
        .ball_start_x    = 500,
        .ball_start_y    = 500,

        .player_start_x  = 100,
        .player_start_y  = 500,

        .holeX           = 800,
        .holeY           = 500,
        .holeRadius      = 200,

        .numObstacles    =  0,
        .obstacles       = ((void*)0)
    },
    // Level 1
    {
        .ball_start_x    = 500,
        .ball_start_y    = 500,

        .player_start_x  = 100,
        .player_start_y  = 500,

        .holeX           = 800,
        .holeY           = 500,
        .holeRadius      = 50,

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
