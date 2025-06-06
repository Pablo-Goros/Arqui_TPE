#include "level.h"


const Level levels[2] = {
    // Level 0: Simple level with no obstacles
    {
        .ball_start = {
            .x = 500,
            .y = 500
        },

        .player_start = {
            { .x = 100, .y = 500 },
            { .x = 200, .y = 700 }      
        },

        .hole = {
            .x = 800,
            .y = 500
        },
        .holeRadius      = 50,

        //.numObstacles    =  0,
        //.obstacles       = ((void*)0)
    },
    // Level 1
    {
        .ball_start = {
            .x = 500,
            .y = 500
        },

        .player_start = {
            { .x = 100, .y = 500 },
            { .x = 200, .y = 700 }      
        },

        .hole = {
            .x = 800,
            .y = 500
        },
        .holeRadius      = 20,

        //.numObstacles    =  0,
        //.obstacles       = ((void*)0)
    }
};


const uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index) {
    if (index < 0 || index >= (int)level_count) return;

    state->currentLevel = index;

    state->ball.physics.position.x =  levels[index].ball_start.x;
    state->ball.physics.position.y =  levels[index].ball_start.y;

    state->players[FIRST_PLAYER_ID].physics.position.x = levels[index].player_start[FIRST_PLAYER_ID].x;
    state->players[FIRST_PLAYER_ID].physics.position.y = levels[index].player_start[FIRST_PLAYER_ID].y;

    state->players[SECOND_PLAYER_ID].physics.position.x = levels[index].player_start[SECOND_PLAYER_ID].x;
    state->players[SECOND_PLAYER_ID].physics.position.y = levels[index].player_start[SECOND_PLAYER_ID].y;

    state->ball.physics.vel_x  = state->ball.physics.vel_y = 0.0f;
    
    state->players[FIRST_PLAYER_ID].physics.vel_x = state->players[FIRST_PLAYER_ID].physics.vel_y = 0.0f;
    state->players[SECOND_PLAYER_ID].physics.vel_x = state->players[SECOND_PLAYER_ID].physics.vel_y = 0.0f;
    
    state->ball.lastTouchId = -1; // No player has touched the ball yet
    
    state->hole.x = levels[index].hole.x;
    state->hole.y = levels[index].hole.y;
    state->holeRadius = levels[index].holeRadius;

    state->touch_counter = 0;
    state->prev_touch_counter = -1;
}
