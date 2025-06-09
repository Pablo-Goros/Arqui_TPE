#include "level.h"

const Level levels[6] = {
    // Level 0: A straight shot, no obstacles.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 600, .y = 300 },
        .holeRadius    = 30,
        .numObstacles  = 0,
        .obstacles     = (Obstacle[]){}
    },
    // Level 1: Hole tucked up in a corner.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 550, .y = 100 },
        .holeRadius    = 30,
        .numObstacles  = 0,
        .obstacles     = (Obstacle[]){}
    },
    // Level 2: Vertical “pinch” – two big red obstacles with just enough gap.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 600, .y = 300 },
        .holeRadius    = 30,
        .numObstacles  = 2,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 400, .y = 260 }, .radius = 40, .color = 0x00FF0000 },
            { .point = { .x = 400, .y = 340 }, .radius = 40, .color = 0x00FF0000 }
        }
    },
    // Level 3: Horizontal “barrier” – two green blocks forcing you around.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 650, .y = 300 },
        .holeRadius    = 30,
        .numObstacles  = 2,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 450, .y = 300 }, .radius = 40, .color = 0x0000FF00 },
            { .point = { .x = 550, .y = 300 }, .radius = 40, .color = 0x0000FF00 }
        }
    },
    // Level 4: Zig-zag blue posts you have to thread between.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 650, .y = 100 },
        .holeRadius    = 30,
        .numObstacles  = 3,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 450, .y = 270 }, .radius = 30, .color = 0x000000FF },
            { .point = { .x = 500, .y = 300 }, .radius = 30, .color = 0x000000FF },
            { .point = { .x = 450, .y = 330 }, .radius = 30, .color = 0x000000FF }
        }
    },
    // Level 5: Final magenta funnel – big blocker plus small exit posts.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 300 } },
        .hole          = { .x = 700, .y = 300 },
        .holeRadius    = 30,
        .numObstacles  = 4,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 400, .y = 300 }, .radius = 60, .color = 0x00FF00FF },
            { .point = { .x = 550, .y = 280 }, .radius = 30, .color = 0x00FF00FF },
            { .point = { .x = 550, .y = 320 }, .radius = 30, .color = 0x00FF00FF },
            { .point = { .x = 650, .y = 300 }, .radius = 20, .color = 0x00FF00FF }
        }
    }
};


const uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index) {
    if (index < 0 || index >= (int)level_count) return;

    state->currentLevel = index;

    state->touch_counter = 0;
    state->prev_touch_counter = 0;

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
