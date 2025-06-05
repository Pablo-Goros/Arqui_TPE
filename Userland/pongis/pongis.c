#include "pongis.h"

static float limit_vel(float vel, float min, float max)
{
    return vel < min ? min : (vel > max ? max : vel);
}

static void velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y, int is_player);

static void limit_velocity(float *vel_x, float *vel_y);

static void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player);

static void ball_limit_velocity(GameState *state);

static void ball_movement_update(GameState *state, ModeInfo *mode);

static uint8_t check_ball_in_hole(GameState *state);

static void check_ball_player_collision(GameState *state);

static void display_welcome_screen();

static void handle_menu_input(ModeInfo mode);

static void clear_object(int x, int y, int radius) {
    sys_call(SYS_DRAW_CIRCLE, x, y, radius + 2, 0x000000, 0); // Black with slight padding
}

static int objects_overlap(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist_sq = dx * dx + dy * dy;
    int combined_radius = r1 + r2;
    return dist_sq <= (combined_radius * combined_radius);
}

void pongis_init()
{
    clear_screen();

    ModeInfo mode;
    if (get_mode_info(&mode) != 0)
    {
        putString("\nError getting mode info\n");
        return;
    }

    display_welcome_screen(mode);
    handle_menu_input(mode);
}

static void display_welcome_screen(ModeInfo mode)
{   
    clear_screen();
    set_zoom(6);
    set_cursor((mode.width / 2) - 300, mode.height / 4);
    putString("PONGIS GOLF\n");
   
    set_zoom(4);

    set_cursor((mode.width / 2) - 300, mode.height / 2);
    putString("Press ENTER to start\n");

    set_cursor((mode.width / 2) - 300, 3* mode.height / 4);
    putString("Press 'c' to quit\n");
}

static void handle_menu_input(ModeInfo mode)
{
    while (1)
    {
        if (isCharReady())
        {
            char key = getChar();
            switch (key)
            {
                case '\n':
                case '\r':
                    pongis(mode);
                    return; // Exit after game ends
                case 'c':
                    clear_screen();
                    return; // Exit to shell
                default:
                    break; // Ignore other keys
            }
        }
    }
}
void pongis(ModeInfo mode) {
    clear_screen();
    int running = 1;
    GameState state;
    GamePhase phase = GAME_PLAYING;
    load_level(&state, 0);
    
    // Store previous positions
    int prev_player_x = state.player_x, prev_player_y = state.player_y;
    int prev_ball_x = state.ball_x, prev_ball_y = state.ball_y;
    
    // Add flags to track if messages have been displayed
    int level_complete_displayed = 0;
    int all_complete_displayed = 0;

    draw_hole(state.holeX, state.holeY, state.holeRadius);


    while (running) {
        if (isCharReady()) {
            char ch = getChar();
            if (phase == GAME_PLAYING) {
                if (ch == 'c') running = 0;
            }
            else if (phase == GAME_LEVEL_COMPLETE) {
                if (ch == '\n' || ch == '\r') {
                    load_level(&state, state.currentLevel + 1);
                    phase = GAME_PLAYING;
                } else if (ch == 'c') {
                    running = 0;
                }
            }
            else if (phase == GAME_ALL_COMPLETE) {
                if (ch == 'c') running = 0;
            }
        }

        // 2) Build continuous direction via new syscall:
        int dir_x = 0, dir_y = 0;
        if (phase == GAME_PLAYING) {
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'w', 0, 0, 0, 0)) dir_y -= 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'s', 0, 0, 0, 0)) dir_y += 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'a', 0, 0, 0, 0)) dir_x -= 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'d', 0, 0, 0, 0)) dir_x += 1;

            // 3) Snapshot, update, collide, etc. (unchanged) …
            prev_player_x = state.player_x;
            prev_player_y = state.player_y;
            prev_ball_x   = state.ball_x;
            prev_ball_y   = state.ball_y;

            velocity_update(dir_x, dir_y, &state.player_vel_x, &state.player_vel_y, 1);
            limit_velocity(&state.player_vel_x, &state.player_vel_y);
            movement_update(&state.player_x, &state.player_y, &state.player_vel_x, &state.player_vel_y, &mode, 1);

            velocity_update(0, 0, &state.ball_vel_x, &state.ball_vel_y, 0);
            limit_velocity(&state.ball_vel_x, &state.ball_vel_y);
            movement_update(&state.ball_x, &state.ball_y, &state.ball_vel_x, &state.ball_vel_y, &mode, 0);

            check_ball_player_collision(&state);
        }
    
        
        
        // Check win condition
        if (phase == GAME_PLAYING) {
            uint8_t flag = check_ball_in_hole(&state);
            if (flag) {
                clear_screen();
                sys_call(SYS_RESET_KBD,0,0,0,0,0);
                if (state.currentLevel + 1 < (int)level_count) {
                    phase = GAME_LEVEL_COMPLETE;
                } else {
                    phase = GAME_ALL_COMPLETE;
                }
            }
        }
        
        // clear_screen();
        
        // Render based on phase
        if (phase == GAME_PLAYING) {
            if (prev_player_x != state.player_x || prev_player_y != state.player_y) {
                clear_object(prev_player_x, prev_player_y, PLAYER_RADIUS);
            }
            if (prev_ball_x != state.ball_x || prev_ball_y != state.ball_y) {
                clear_object(prev_ball_x, prev_ball_y, BALL_RADIUS);
            }

            draw_player(state.player_x, state.player_y, PLAYER_RADIUS);
            draw_ball(state.ball_x, state.ball_y, BALL_RADIUS);

            // redraw hole only if ball or player is overlapping
            if (objects_overlap(state.ball_x, state.ball_y, BALL_RADIUS, state.holeX, state.holeY, state.holeRadius) ||
                objects_overlap(state.player_x, state.player_y, PLAYER_RADIUS, state.holeX, state.holeY, state.holeRadius)) {
                draw_hole(state.holeX, state.holeY, state.holeRadius);
            }
        } else if (phase == GAME_LEVEL_COMPLETE) {
            if (!level_complete_displayed) {
                set_cursor((mode.width / 2) - 200, mode.height / 2 - 50);
                set_zoom(6);
                putString("Level Complete!\n");
                set_cursor((mode.width / 2) - 300, mode.height / 2 + 50);
                set_zoom(4);
                putString("Press ENTER for next level or 'c' to quit\n");
                level_complete_displayed = 1;
            }
        } else if (phase == GAME_ALL_COMPLETE) {
            if (!all_complete_displayed) {
                set_cursor((mode.width / 2) - 200, mode.height / 2 - 50);
                set_zoom(6);
                putString("All levels complete!\n");
                set_cursor((mode.width / 2) - 250, mode.height / 2 + 50);
                set_zoom(4);
                putString("Press 'c' to return to shell\n");
                all_complete_displayed = 1;
            }
        }
        
        wait_next_tick();
        
    }

    clear_screen();
    return;

}

static void velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y, int is_player)
{
    if (is_player)
    {
        *vel_x += dir_x * ACCELERATION;
        *vel_y += dir_y * ACCELERATION;
    }

    *vel_x *= FRICTION;
    *vel_y *= FRICTION;
}

static void limit_velocity(float *vel_x, float *vel_y)
{
    float speed = _sqrt((*vel_x) * (*vel_x) + (*vel_y) * (*vel_y));

    // limit player speed
    if (speed > MAX_SPEED)
    {
        float scale = MAX_SPEED / speed;
        *vel_x *= scale;
        *vel_y *= scale;
    }
}

static void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player)
{
    int radius = is_player ? PLAYER_RADIUS : BALL_RADIUS;
    *x += (int)*vel_x;
    *y += (int)*vel_y;

    // bounce ball off screen edges
    if (*x < radius)
    {
        *x = radius;
        *vel_x = -*vel_x;
    }
    else if (*x > mode->width - radius)
    {
        *x = mode->width - radius;
        *vel_x = -*vel_x;
    }
    if (*y < radius)
    {
        *y = radius;
        *vel_y = -*vel_y;
    }
    else if (*y > mode->height - radius)
    {
        *y = mode->height - radius;
        *vel_y = -*vel_y;
    }
}

static void check_ball_player_collision(GameState *state)
{
    int dx = state->ball_x - state->player_x;
    int dy = state->ball_y - state->player_y;
    int dist = (int)_sqrt(dx * dx + dy * dy);
    int min_dist = BALL_RADIUS + PLAYER_RADIUS;

    if (dist <= min_dist)
    {
        if (dist < 1)
        {
            // complete overlap
            // arbitrary values
            dx = 1;
            dy = 0;
            dist = 1;
        }
        float nx = (float)dx / dist;
        float ny = (float)dy / dist;

        // relative velocity of player vs. ball
        float rel_vx = state->player_vel_x - state->ball_vel_x;
        float rel_vy = state->player_vel_y - state->ball_vel_y;
        // project relative velocity onto normal
        float rel_dot = rel_vx * nx + rel_vy * ny;
        if (rel_dot > 0.0f)
        {
            // player is “pushing” into the ball
            // simply transfer “rel_dot” along normal into ball
            state->ball_vel_x += (int)rel_dot * nx;
            state->ball_vel_y += (int)rel_dot * ny;
        }

        // move ball out so they’re not stuck overlapping
        float push_back = min_dist - dist;
        state->ball_x += (int)(nx * push_back);
        state->ball_y += (int)(ny * push_back);
    }
}

static uint8_t check_ball_in_hole(GameState *state)
{
    int dx = state->ball_x - state->holeX;
    int dy = state->ball_y - state->holeY;

    int dist = (int)_sqrt(dx * dx + dy * dy);

    // Ball falls in when it's mostly inside the hole
    int threshold = state->holeRadius - (BALL_RADIUS * 0.8f);

    return dist <= threshold ? 1 : 0;
}

//? quizas agregar una variable en draw circle q sea con o sin outline, por ejemplo para el hole...

void draw_player(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, PLAYER_COLOR, 0);
}

void draw_ball(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, BALL_COLOR, 0);
}

void draw_hole(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, HOLE_COLOR, 0);
}
