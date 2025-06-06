/* game.h */

#ifndef PONGIS_H
#define PONGIS_H

#include <stdint.h>
#include "libc.h"
#include "level.h"
#include "shell.h"
#include "bitmaps.h"
#include "math.h"

// Physics constants
#define ACCELERATION 0.2f /* px/tick² */
#define MAX_SPEED 5.0f    /* px/tick */
#define FRICTION 0.995f   /* velocity scaling per frame */
#define BALL_RADIUS 10    /* px */
#define PLAYER_RADIUS 20  /* px */

#define PLAYER_COLOR 0x0000FF00 // Green player
#define BALL_COLOR 0x00FFFFFF   // White ball
#define HOLE_COLOR 0x00808080   // Grey hole

#define MAX_PLAYERS 2
#define ONE_PLAYER_MODE       1
#define TWO_PLAYER_MODE       2

typedef enum
{
    GAME_PLAYING,
    GAME_LEVEL_COMPLETE,
    GAME_ALL_COMPLETE
} GamePhase;

typedef struct PhysicsObject
{
    int x;
    int y;
    float vel_x;
    float vel_y;
    int radius;
    uint32_t color; // Color of the object
} PhysicsObject;

typedef struct Player
{
    PhysicsObject physics; // Velocity
    int id;                // Player ID (1 or 2)
} Player;

typedef struct Ball
{
    PhysicsObject physics; // Velocity
    // int ownerId; // Pointer to the player who owns the ball
    int lastTouchId; // Pointer to the last player who touched the ball
} Ball;

typedef struct GameState
{
    int currentLevel;
    int numPlayers; // 1 or 2 players

    Player players[2]; // Array of players, if numPlayers == 1,the second player will be unused
    Ball ball;         // Array of balls,

    int holeX, holeY;
    int holeRadius;
} GameState;

void pongis(ModeInfo mode, int player_count);

void pongis_init();

void draw_player(int x, int y, int radius);

void draw_ball(int x, int y, int radius);

void draw_hole(int x, int y, int radius);

void velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y, int is_player);

void limit_velocity(float *vel_x, float *vel_y);

void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player);

void ball_limit_velocity(GameState *state);

void ball_movement_update(GameState *state, ModeInfo *mode);

uint8_t check_ball_in_hole(GameState *state);

void check_ball_player_collision(GameState *state);

void display_welcome_screen();

void handle_menu_input(ModeInfo mode);
#endif
