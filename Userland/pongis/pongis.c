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
static uint32_t int_sqrt(uint32_t x) {
    // Simple integer square‐root via binary search / Newton’s method.
    // You can replace this with any fast integer-sqrt you already have.
    uint32_t op = x;
    uint32_t res = 0;
    uint32_t one = 1uL << 30;  // 2^30, highest power of four <= UINT32_MAX

    // "one" starts at the highest power of four <= the argument.
    while (one > op) {
        one >>= 2;
    }

    while (one != 0) {
        if (op >= res + one) {
            op -= res + one;
            res = (res >> 1) + one;
        } else {
            res = res >> 1;
        }
        one >>= 2;
    }
    return res;
}

static void check_ball_player_collision(GameState *state)
{
    // Calculate positions in x and y
    uint64_t bx = state->ball_x;
    uint64_t by = state->ball_y;
    uint64_t px = state->player_x;
    uint64_t py = state->player_y;

    // Deltas
    uint64_t dx = bx - px;
    uint64_t dy = by - py;

    // Squared distance between centers
    uint64_t dist_sq = dx * dx + dy * dy;

    // 4) Minimum center‐to‐center to just touch (edge‐to‐edge)
    uint64_t min_dist = PLAYER_RADIUS + BALL_RADIUS;

    uint64_t min_dist_sq = min_dist * min_dist;

    // 5) Only run collision logic if edges overlap or touch
    if (dist_sq <= min_dist_sq)
    {
        // Calculate exact distance between centers 
        uint32_t dist_i = int_sqrt(dist_sq);
        float dist_f = (dist_i == 0 ? 1.0f : (float)dist_i);

        // Calculate normalized direction vector from player to ball
        float nx = (float)dx / dist_f;
        float ny = (float)dy / dist_f;


        // To avoid floating point multiplication, when we want ints, we use Q8.8 Fixed-Point logic       
        // Multiplying by 256 gives us a 24.8 representation (High 24 bits are ints, and 8 low bits are fractional part)
        int pushDist = min_dist + 1;
        int ux_q8 = (dx / (int)dist_f) * 256;  
        int uy_q8 = (dy / (int)dist_f) * 256;  

        // We want to move balls center by pushDist pixels
        // Multiplying ux_q8 (Our unit vector) by pushDist gives us the offset in Q8.8 format
        // We then shift right by 8 to convert it back to int
        int offsetX = (ux_q8 * pushDist) >> 8;
        int offsetY = (uy_q8 * pushDist) >> 8;

        // Update ball position by offset
        state->ball_x = (float)(px + offsetX);
        state->ball_y = (float)(py + offsetY);

        // compute normal velocity components
        float v_ball_norm   = state->ball_vel_x * nx + state->ball_vel_y * ny;
        float v_plr_norm    = state->player_vel_x * nx + state->player_vel_y * ny;

        //----------------------------------------------------------------
        // D) Distinguish two cases:
        //
        // (1) Player is “catching up” to or pushing into a slower (or stationary) ball:
        //     i.e. v_plr_norm > v_ball_norm. In that case, we transfer the player's normal
        //     velocity onto the ball so it immediately moves forward with that speed.
        //
        // (2) Ball is moving faster into (or along) the player: we do an elastic reflection
        //     of the ball's velocity about the normal: v'_ball = v_ball – 2*(v_ball·n)*n.
        //
        // Note: if v_plr_norm == v_ball_norm, they’re moving together along normal—no bounce.

        if (v_plr_norm > v_ball_norm)
        {
            // CASE 1: Transfer player’s normal velocity onto ball
            //    ball's new velocity = v_player_norm * n  (pure normal component)
            state->ball_vel_x = v_plr_norm * nx;
            state->ball_vel_y = v_plr_norm * ny;
        }
        else
        {
            // CASE 2: Elastic reflection of ball’s velocity
            //    v_rel_norm = v_ball_norm  (since v_plr_norm <= v_ball_norm)
            //    New v_ball = v_ball – 2*(v_ball_norm)*n
            float reflectFactor = -2.0f * v_ball_norm;
            state->ball_vel_x += reflectFactor * nx;
            state->ball_vel_y += reflectFactor * ny;
        }
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
