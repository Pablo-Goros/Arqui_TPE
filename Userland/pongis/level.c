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
        .numObstacles    =  1,
        .obstacles       = (Obstacle[]) {
            {
                .point = { .x = 400, .y = 500 },
                .radius = 50,
                .color = 0x00FF0000 // Red obstacle
            }
        }
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

        .numObstacles    =  1,
        .obstacles       = (Obstacle[]) {
            {
                .point = { .x = 400, .y = 500 },
                .radius = 50,
                .color = 0x00FF0000 // Red obstacle
            }
        }
    } /*
    // Level 2
    {

    },
    // Level 3
    {

    },
    // Level 4
    {

    }, 
    // Level 5
    {

    }, */
};


const uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index) {
    if (index < 0 || index >= (int)level_count) return;

    state->currentLevel = index;

    state->ball.physics.position.x =  levels[index].ball_start.x;
    state->ball.physics.position.y =  levels[index].ball_start.y;
    state->ball.physics.vel_x  = state->ball.physics.vel_y = 0.0f;
    state->ball.physics.radius = BALL_RADIUS;
    state->ball.physics.color  = BALL_COLOR;


    for (int i = 0; i < MAX_PLAYERS; i++) {
        state->players[i].id = i;
        state->players[i].physics.position.x = levels[index].player_start[i].x;
        state->players[i].physics.position.y = levels[index].player_start[i].y;
        state->players[i].physics.radius = PLAYER_RADIUS;
        state->players[i].physics.vel_x = 0.0f;
        state->players[i].physics.vel_y = 0.0f;
    }
    state->players[FIRST_PLAYER_ID].physics.color = PLAYER_ONE_COLOR;
    state->players[SECOND_PLAYER_ID].physics.color = PLAYER_TWO_COLOR;
    state->players[FIRST_PLAYER_ID].id = FIRST_PLAYER_ID;
    state->players[SECOND_PLAYER_ID].id = SECOND_PLAYER_ID;

    state->hole.x = levels[index].hole.x;
    state->hole.y = levels[index].hole.y;
    state->holeRadius = levels[index].holeRadius;

    state->touch_counter =state->prev_touch_counter= 0;

    state->current_level = (Level*)&levels[index];

}
