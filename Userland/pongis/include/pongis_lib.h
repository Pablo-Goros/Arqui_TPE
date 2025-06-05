#ifndef PONGIS_LIB_H
#define PONGIS_LIB_H
#include <stdint.h>
#include "pongis.h"
#include "libc.h"
#include "math.h"

#define ACCELERATION        0.05f   /* px/tick² */
#define MAX_PLAYER_SPEED    5.0f   /* px/tick */
#define MAX_BALL_SPEED      15.0f  /* px/tick */
#define ACCELERATION_RATE   0.6f  

#define SLOW_ACCEL_RATE     0.3f   /* px/tick² */
#define RELEASED_AXIS_FRICTION 0.99f /* friction when one axis is released */
#define SNAP_SPEED_THRESHOLD 0.75

#define PLAYER_FRICTION     0.99f  /* velocity scaling per frame */
#define BALL_FRICTION       0.98f  /* ball velocity scaling per frame */

#define BALL_RADIUS         10      /* px */
#define PLAYER_RADIUS       20     /* px */

#define IS_PLAYER           1
#define IS_BALL             0

#define PLAYER_ONE_COLOR        0x0000FF00  // Green player
#define PLAYER_TWO_COLOR        0x00FF0000  // Red player
#define BALL_ONE_COLOR          0x000000FF  // Blue ball
#define BALL_TWO_COLOR          0x00FFFF00  // Yellow ball
#define HOLE_COLOR              0x00808080  // Grey hole

typedef enum {
    GAME_PLAYING,
    GAME_LEVEL_COMPLETE,
    GAME_ALL_COMPLETE
} GamePhase;

typedef struct GameState {
    int     currentLevel;

    int     ball_x,    ball_y;
    float     ball_vel_x,    ball_vel_y;

    int     player_x, player_y;
    float     player_vel_x, player_vel_y;

    int             holeX, holeY;
    int             holeRadius;
} GameState;


void clear_object(int x, int y, int radius);

int objects_overlap(int x1, int y1, int r1, int x2, int y2, int r2);

/**
   @brief: Update velocity for player or ball. If is_player is non-zero, apply acceleration based on dir_x/dir_y, then apply friction. Otherwise, apply friction only.
   @param vel_x:      Pointer to the current x-component of velocity (modifiable).
   @param vel_y:      Pointer to the current y-component of velocity (modifiable).
**/
void ball_velocity_update(float *vel_x, float *vel_y);

/**
   @brief: Clamp a velocity vector so its magnitude does not exceed maximum speed.
   @param vel_x:  Pointer to the x-component of velocity (modifiable).
   @param vel_y:  Pointer to the y-component of velocity (modifiable).
**/
void limit_velocity(float *vel_x, float *vel_y, int is_player);

/**
   @brief: Update an object's position based on its velocity, handling screen boundaries.
   @param x:         Pointer to the current x-coordinate of the object (modifiable).
   @param y:         Pointer to the current y-coordinate of the object (modifiable).
   @param vel_x:     Pointer to the x-component of velocity.
   @param vel_y:     Pointer to the y-component of velocity.
   @param mode:      Pointer to ModeInfo containing screen dimensions.
   @param is_player: Non-zero if updating the player (use player clamping); zero if updating the ball.
**/
void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player);

/**
   @brief: Apply friction or other limits to the ball's velocity only.
   @param state: Pointer to the GameState containing ball_vel_x and ball_vel_y (modifiable).
**/
void ball_limit_velocity(GameState *state);

/**
   @brief: Update the ball's position based on its velocity, handling screen boundaries.
   @param state: Pointer to the GameState containing ball_x, ball_y, ball_vel_x, ball_vel_y.
   @param mode:  Pointer to ModeInfo containing screen dimensions.
**/
void ball_movement_update(GameState *state, ModeInfo *mode);

/**
   @brief: Check if the ball is within the hole region (win condition).
   @param state: Pointer to the GameState containing ball_x, ball_y, and hole parameters.
   @return:      Non-zero (1) if ball is in the hole; zero otherwise.
**/
uint8_t check_ball_in_hole(GameState *state);

/**
   @brief: Detect and resolve collisions between the player and the ball, adjusting positions and velocities.
   @param state: Pointer to the GameState containing player_x, player_y, player_vel_x, player_vel_y, ball_x, ball_y, ball_vel_x, ball_vel_y.
**/
void check_ball_player_collision(GameState *state);

/**
   @brief: Display the initial welcome screen and any instructions before the game starts. Blocks until user proceeds.
**/
void display_welcome_screen(ModeInfo mode);

/**
   @brief: Handle user input when in the main menu or pause screen.
   @param mode: ModeInfo containing screen dimensions and menu state.
**/
void handle_menu_input(ModeInfo mode);

/**
   @brief: Update the player's velocity using acceleration toward a desired direction and apply friction.
   @param dir_x:  Direction on the x-axis (-1, 0, +1) based on user input.
   @param dir_y:  Direction on the y-axis (-1, 0, +1) based on user input.
   @param vel_x:  Pointer to the current x-component of the player's velocity (modifiable).
   @param vel_y:  Pointer to the current y-component of the player's velocity (modifiable).
**/
void player_velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y);

/**
   @brief: Check if two objects overlap based on their positions and radius.
   @param state: Pointer to the GameState containing object positions and radius.
**/
void check_ball_player_collision(GameState *state);

/** 
   @brief: Verify if the ball is within the hole's radius, indicating a win condition.
   @param state: Pointer to the GameState containing ball_x, ball_y, holeX, holeY, and holeRadius.  
**/
uint8_t check_ball_in_hole(GameState *state);

void draw_player(int x, int y, int radius);

void draw_ball(int x, int y, int radius);

void draw_hole(int x, int y, int radius);

#endif // PONGIS_LIB_H