#include "level.h"
#include "pongis.h" // for GameState definition

const Level levels[2] = {
    // Level 0: Simple level with no obstacles
    {
        .ball_start_x = 500,
        .ball_start_y = 500,
        .players = {
            {
                .x = 100,
                .y = 500,
            },
            {
                .x = 200,
                .y = 700,
            }},
        .holeX = 800,
        .holeY = 500,
        .holeRadius = 200,

        .numObstacles = 0,
        .obstacles = ((void *)0)},
    // Level 1
    {.ball_start_x = 500, .ball_start_y = 500,

     .players = {{
                     .x = 100,
                     .y = 500,
                 },
                 {
                     .x = 200,
                     .y = 700,
                 }},

     .holeX = 800,
     .holeY = 500,
     .holeRadius = 50,

     .numObstacles = 0,
     .obstacles = ((void *)0)}};

const uint64_t level_count = sizeof(levels) / sizeof(levels[0]);

void load_level(GameState *state, int index)
{
    if (index < 0 || index >= (int)level_count)
        return;

    state->currentLevel = index;
    // Initialize players and balls
    state->numPlayers = 2; // Assuming single player for now

    state->ball.physics.x = levels[index].ball_start_x;
    state->ball.physics.y = levels[index].ball_start_y;

    for (int i = 0; i < state->numPlayers; i++)
    {
        state->players[i].physics.x = levels[index].players[i].x;
        state->players[i].physics.y = levels[index].players[i].y;
        state->players[i].physics.vel_x = 0.0f;
        state->players[i].physics.vel_y = 0.0f;
        state->players[i].id = i + 1; // Player IDs start from 1
        state->players[i].physics.radius = PLAYER_RADIUS;
    }

    state->holeX = levels[index].holeX;
    state->holeY = levels[index].holeY;
    state->holeRadius = levels[index].holeRadius;
}
