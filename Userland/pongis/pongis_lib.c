#include "pongis_lib.h"

const char* menuOptions[] = {
    "Jugar (1 jugador)      ",
    "Jugar (2 jugadores)    ",
    "Instrucciones          ",
    "Volver al Shell        "
};
const int menuCount = sizeof(menuOptions) / sizeof(menuOptions[0]);


uint8_t startPongisMenu(ModeInfo mode) {
    int selected = 0;
    drawMainMenu(selected);

    while (1) {
        if (!isCharReady()) {
            _hlt();  // Espera a la siguiente interrupción
            continue;
        }

        char c = getChar();
        switch (c) {
            case 'w':  // Mover selección hacia arriba
                selected = (selected + menuCount - 1) % menuCount;
                drawMainMenu(selected);
                break;

            case 's':  // Mover selección hacia abajo
                selected = (selected + 1) % menuCount;
                drawMainMenu(selected);
                break;

            case '\n':
            case '\r':
                return selected;
                break;

            default:
                break;
        }
    }
}

void drawMainMenu(int selected) {
    clear_screen();
    set_zoom(5);
    putString("       PONGIS GOLF\n\n");

    set_zoom(3);
    for (int i = 0; i < menuCount; i++) {
        putString("  ");
        putString(menuOptions[i]);
        if (i == selected) {
            putString("  <--");
        }
        putString("\n\n");
    }
}

void drawInstructions(void) {
    clear_screen();
    set_zoom(4);
    putString("      INSTRUCCIONES\n\n");

    set_zoom(2);
    putString(" Jugador 1:\n");
    putString("   W    -> Arriba\n");
    putString("   S    -> Abajo\n");
    putString("   A    -> Izquierda\n");
    putString("   D    -> Derecha\n\n");

    putString(" Jugador 2:\n");
    putString("   Flecha Arriba    -> Arriba\n");
    putString("   Flecha Abajo     -> Abajo\n");
    putString("   Flecha Izquierda -> Izquierda\n");
    putString("   Flecha Derecha   -> Derecha\n\n");

    putString(" Presiona 'c' para volver al menú principal.\n");

    // Espera hasta que el usuario presione 'c' o ENTER
    while (1) {
        if (isCharReady()) {
            char key = getChar();
            if (key == 'c' || key == '\n' || key == '\r') {
                return;
            }
        }
        _hlt();
    }
}


void display_welcome_screen(ModeInfo mode)
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

void handle_menu_input(ModeInfo mode)
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


float limit_vel(float vel, float min, float max)
{
    return vel < min ? min : (vel > max ? max : vel);
}

void clear_object(int x, int y, int radius) {
    sys_call(SYS_DRAW_CIRCLE, x, y, radius + 2, 0x000000, 0); // Black with slight padding
}

int objects_overlap(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dist_sq = dx * dx + dy * dy;
    int combined_radius = r1 + r2;
    return dist_sq <= (combined_radius * combined_radius);
}

void player_velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y) {
    if (dir_x != 0 || dir_y != 0) {
        // Calculate normalized input vector (ux, uy)
        float length = _sqrt((float)(dir_x * dir_x + dir_y * dir_y));
        float ux = dir_x / length;
        float uy = dir_y / length;

        // Calculate ±MAX_PLAYER_SPEED for each axis
        float target_vx = ux * MAX_PLAYER_SPEED;
        float target_vy = uy * MAX_PLAYER_SPEED;

        // Detect diagonal to straight shift 
        int shifting_to_cardinal = ((dir_x == 0 && dir_y != 0) || (dir_x != 0 && dir_y == 0));

        // Check if current speed ≥ SNAP_SPEED_THRESHOLD% of max_sq
        float speed_sq = (*vel_x) * (*vel_x) + (*vel_y) * (*vel_y);
        float max_sq   = MAX_PLAYER_SPEED * MAX_PLAYER_SPEED;
        int   already_fast = (speed_sq >= SNAP_SPEED_THRESHOLD * max_sq);

        if (shifting_to_cardinal && already_fast) {
            // ─── Snap the held axis, gently decay the released axis ───
            if (dir_x != 0 && dir_y == 0) {
                *vel_x = target_vx;
                *vel_y *= RELEASED_AXIS_FRICTION;
            }
            else {
                *vel_y = target_vy;
                *vel_x *= RELEASED_AXIS_FRICTION;
            }
        }
        else {
            // ─── Slow “ease‐toward” to the target (using SLOW_ACCEL_RATE) ───
            float dvx = target_vx - *vel_x;
            float dvy = target_vy - *vel_y;
            *vel_x += dvx * SLOW_ACCEL_RATE;
            *vel_y += dvy * SLOW_ACCEL_RATE;
        }
    }
    else {
        // ─── No key pressed → standard friction coast ───
        *vel_x *= PLAYER_FRICTION;
        *vel_y *= PLAYER_FRICTION;
    }

    limit_velocity(vel_x, vel_y, IS_PLAYER);
}

void ball_velocity_update(float *vel_x, float *vel_y)
{
    // Friction multiplier for the ball
    *vel_x *= BALL_FRICTION;
    *vel_y *= BALL_FRICTION;

    limit_velocity(vel_x, vel_y, IS_BALL);
}

void limit_velocity(float *vel_x, float *vel_y, int is_player)
{
    int max_speed = (is_player == IS_PLAYER) ? MAX_PLAYER_SPEED : MAX_BALL_SPEED;
    float speed = _sqrt((*vel_x) * (*vel_x) + (*vel_y) * (*vel_y));

    // limit player speed
    if (speed > max_speed)
    {
        float scale = max_speed / speed;
        *vel_x *= scale;
        *vel_y *= scale;
    }
}

void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player)
{
    int radius = (is_player == IS_PLAYER) ? PLAYER_RADIUS : BALL_RADIUS;
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

void check_ball_player_collision(GameState *state)
{
    // Calculate positions in x and y
    int  bx = (int)state->ball_x;
    int  by = (int)state->ball_y;
    int  px = (int)state->player_x;
    int  py = (int)state->player_y;

    // Deltas
    int  dx = bx - px;
    int  dy = by - py;

    // Squared distance between centers
    uint32_t  dist_sq = (uint32_t) ((uint32_t) dx * (uint32_t) dx + (uint32_t) dy * (uint32_t) dy);

    // Minimum center‐to‐center to just touch (edge‐to‐edge)
    int  min_dist = PLAYER_RADIUS + BALL_RADIUS;
    uint32_t  min_dist_sq = min_dist * min_dist;

    // Only run collision logic if edges overlap or touch
    if (dist_sq <= min_dist_sq)
    {
        state->touch_counter++;
        // Calculate exact distance between centers 
        uint32_t dist_i = int_sqrt(dist_sq);
        float dist_f = (dist_i == 0 ? 1.0f : (float)dist_i);

        // Calculate normalized direction vector from player to ball
        float nx = (float)dx / dist_f;
        float ny = (float)dy / dist_f;

        /*
        To avoid floating point multiplication, when we want ints, we use Q8.8 Fixed-Point logic       
        Multiplying by 256 gives us a 24.8 representation (High 24 bits are ints, and 8 low bits are fractional part)
        */
        int pushDist = min_dist + 1;
        int ux_q8 = (dx * 256) / (int)dist_f;  
        int uy_q8 = (dy * 256) / (int)dist_f;  

        /*
        We want to move balls center by pushDist pixels
        Multiplying ux_q8 (Our unit vector) by pushDist gives us the offset in Q8.8 format
        We then shift right by 8 to convert it back to int
        */ 
        int offsetX = (ux_q8 * pushDist) >> 8;
        int offsetY = (uy_q8 * pushDist) >> 8;

        // Update ball position by offset
        state->ball_x = (float)(px + offsetX);
        state->ball_y = (float)(py + offsetY);

        // compute normal velocity components
        float v_ball_norm   = state->ball_vel_x * nx + state->ball_vel_y * ny;
        float v_player_norm = state->player_vel_x * nx + state->player_vel_y * ny;

        /* Cases:
              1. Player is "cathing up" to or pushing into a slower (or stationary) ball (v_player_norm > v_ball_norm)
                 Transfer the player's normal velocity onto the ball so it immediately moves forward with that speed.
              
              2. Ball is moving faster into the player 
                 Elastic reflection: v'_ball = v_ball – 2*(v_ball·n)*n
        */
        if (v_player_norm > v_ball_norm) // Case 1
        {   
            float boost = 4.0f; // 20% extra “kick”
            state->ball_vel_x = v_player_norm * nx * boost;
            state->ball_vel_y = v_player_norm * ny * boost;
        }
        else // Case 2
        {
            float reflectFactor = -2.0f * v_ball_norm;
            state->ball_vel_x += reflectFactor * nx;
            state->ball_vel_y += reflectFactor * ny;
        }
    }
}

uint8_t check_ball_in_hole(GameState *state)
{
    // Convert float positions to int for distance calculation
    int ball_x = (int)state->ball_x;
    int ball_y = (int)state->ball_y;
    
    int dx = ball_x - state->holeX;
    int dy = ball_y - state->holeY;

    // Use integer square root like you do in collision detection
    uint32_t dist_sq = (uint32_t)(dx * dx + dy * dy);
    int dist = (int)int_sqrt(dist_sq);  // Use your existing int_sqrt function

    // Ball falls in when its center is close enough to hole center
    // The threshold should be positive - when ball center gets within 
    // (hole_radius - some_margin) of hole center
    int threshold = state->holeRadius - (BALL_RADIUS / 2);
    
    // Make sure threshold is positive
    if (threshold <= 0) {
        threshold = state->holeRadius / 2;  // Fallback to half hole radius
    }

    return dist <= threshold ? 1 : 0;
}

void draw_player(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, PLAYER_ONE_COLOR, 0);
}

void draw_ball(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, BALL_ONE_COLOR, 0);
}

void draw_hole(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, HOLE_COLOR, 0);
}

void draw_counter(int count, ModeInfo mode) {
    set_cursor(10, 10);
    set_zoom(4);
    //dibuja el rectangulo en toda la parte inferior de la pantalla
    sys_call(SYS_DRAW_RECT,0,mode.height+OFFSET,mode.width,UI,0x00FFFFFF);
    sys_call(SYS_DRAW_RECT,10, mode.height+OFFSET+10,mode.width - 20, UI - 20, 0x00000000); // Black background for counter

    set_zoom(2);
    set_cursor(20, 725);
    putString("Touches: ");
    char buffer[10];
    itoa(count, buffer, 10);
    putString(buffer);

}
