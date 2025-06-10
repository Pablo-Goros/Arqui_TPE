#include "level.h"

const Level levels[7] = {
    // Level 0: A straight shot, no obstacles.
    // Level 7: 
    {
        .ball_start    = { .x = 200, .y = 300 },
        .player_start  = { { .x = 100, .y = 200 }, { .x = 100, .y = 400 } },
        .hole          = { .x = 400, .y = 300 },
        .holeRadius    = 20,
        .numObstacles  = 9,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 300, .y = 600 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 300, .y = 675 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 340, .y = 600 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 340, .y = 675 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 380, .y = 675 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 420, .y = 675 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 420, .y = 675 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 420, .y = 600 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 420, .y = 630 }, .radius = 20, .color = OBSTACLE_COLOR },   // Norte
        }
    },{
        .ball_start    = { .x = 200, .y = 300 },
        .player_start  = { { .x = 100, .y = 200 }, { .x = 100, .y = 400 } },
        .hole          = { .x = 400, .y = 300 },
        .holeRadius    = 20,
        .numObstacles  = 7,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 400, .y = 255 }, .radius = 18, .color = OBSTACLE_COLOR },   // Norte
            { .point = { .x = 432, .y = 268 }, .radius = 18, .color = OBSTACLE_COLOR },   // Noreste
            // Ranura Este omitida
            { .point = { .x = 432, .y = 332 }, .radius = 18, .color = OBSTACLE_COLOR },   // Sureste
            { .point = { .x = 400, .y = 345 }, .radius = 18, .color = OBSTACLE_COLOR },   // Sur
            { .point = { .x = 368, .y = 332 }, .radius = 18, .color = OBSTACLE_COLOR },   // Suroeste
            { .point = { .x = 355, .y = 300 }, .radius = 18, .color = OBSTACLE_COLOR },   // Oeste
            { .point = { .x = 368, .y = 268 }, .radius = 18, .color = OBSTACLE_COLOR }    // Noroeste
        }
    },
    
    {
        .ball_start    = { .x = 300, .y = 400 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 200 } },
        .hole          = { .x = 600, .y = 400 },
        .holeRadius    = 30,
        .numObstacles  = 6,
        .obstacles     = (Obstacle[]){
            // generate 6 obstacles in a grid pattern
            { .point = { .x = 400, .y = 400 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 300 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 200 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 100 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 500 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 400 }, .radius = 20, .color = OBSTACLE_COLOR }
        }
    },    
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 200 } },
        .hole          = { .x = 650, .y = 100 },
        .holeRadius    = 30,
        .numObstacles  = 3,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 400, .y = 400 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 300 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 200 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 100 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 500 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 400 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 300 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 200 }, .radius = 20, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 100 }, .radius = 20, .color = OBSTACLE_COLOR }
        }
    },
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
            { .point = { .x = 400, .y = 260 }, .radius = 40, .color = OBSTACLE_COLOR },
            { .point = { .x = 400, .y = 340 }, .radius = 40, .color = OBSTACLE_COLOR }
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
            { .point = { .x = 450, .y = 300 }, .radius = 40, .color = OBSTACLE_COLOR },
            { .point = { .x = 550, .y = 300 }, .radius = 40, .color = OBSTACLE_COLOR }
        }
    },
    // Level 4: Zig-zag blue posts you have to thread between.
    {
        .ball_start    = { .x = 300, .y = 300 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 200 } },
        .hole          = { .x = 650, .y = 100 },
        .holeRadius    = 30,
        .numObstacles  = 3,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 450, .y = 270 }, .radius = 30, .color = OBSTACLE_COLOR },
            { .point = { .x = 500, .y = 300 }, .radius = 30, .color = OBSTACLE_COLOR },
            { .point = { .x = 450, .y = 330 }, .radius = 30, .color = OBSTACLE_COLOR }
        }
    },
    // Level 5 
{
        .ball_start    = { .x = 300, .y = 400 },
        .player_start  = { { .x = 150, .y = 300 }, { .x = 150, .y = 200 } },
        .hole          = { .x = 600, .y = 400 },
        .holeRadius    = 30,
        .numObstacles  = 4,
        .obstacles     = (Obstacle[]){
            { .point = { .x = 500, .y = 400 }, .radius = 65, .color = OBSTACLE_COLOR },
            { .point = { .x = 700, .y = 400 }, .radius = 65, .color = OBSTACLE_COLOR },
            { .point = { .x = 600, .y = 500 }, .radius = 65, .color = OBSTACLE_COLOR },
            { .point = { .x = 600, .y = 300 }, .radius = 65, .color = OBSTACLE_COLOR }
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
