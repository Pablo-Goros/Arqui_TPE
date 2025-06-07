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
    game_start_sound();
    while (1) {
        if (!isCharReady()) { // Wait for next char
            _hlt();  
            continue;
        }

        char c = getChar();
        switch (c) {
            case 'w':  // Move up
                selected = (selected + menuCount - 1) % menuCount;
                drawMainMenu(selected);
                break;

            case 's':  // Move down
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

    sys_call(SYS_RESET_KBD_BUFFER, 0, 0, 0, 0, 0);

    while (isCharReady()) {
        (void)getChar();
    }

    // Wait for user input
    while (1) {
        if (isCharReady()) {
            char key = getChar();
            if (key == 'c') {
                return;
            }
        }
        _hlt();
    }
}


float limit_vel(float vel, float min, float max)
{
    return vel < min ? min : (vel > max ? max : vel);
}

void clear_object(Point point, int radius) {
    // Clear the object by drawing a black circle over it
    sys_call(SYS_DRAW_CIRCLE, point.x, point.y, radius, BLACK_COLOR, 0); // 
}

int objects_overlap(Point point1, Point point2, int radius1, int radius2) {
    int dx = point1.x - point2.x;
    int dy = point1.y - point2.y;

    int dist_sq = dx * dx + dy * dy;
    int combined_radius = radius1 + radius2;
    return dist_sq <= (combined_radius * combined_radius);
}

void player_velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y) {
    if (dir_x != 0 || dir_y != 0) {
        // Calculate normalized input vector (ux, uy)
        float length = sqrtf((float)(dir_x * dir_x + dir_y * dir_y));
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
    float speed = sqrtf((*vel_x) * (*vel_x) + (*vel_y) * (*vel_y));

    // limit player speed
    if (speed > max_speed)
    {
        float scale = max_speed / speed;
        *vel_x *= scale;
        *vel_y *= scale;
    }
}

void movement_update(PhysicsObject *obj, ModeInfo *mode, int radius)
{
    obj->position.x += (int)obj->vel_x;
    obj->position.y += (int)obj->vel_y;

    // bounce ball off screen edges
    if (obj->position.x < radius)
    {
        obj->position.x = radius;
        obj->vel_x = -obj->vel_x; // * BOUNCE_FACTOR;
    }
    else if (obj->position.x > mode->width - radius)
    {
        obj->position.x = mode->width - radius;
        obj->vel_x = -obj->vel_x; // * BOUNCE_FACTOR;
    }
    if (obj->position.y < radius)
    {
        obj->position.y = radius;
        obj->vel_y = -obj->vel_y; // * BOUNCE_FACTOR;
    }
    else if (obj->position.y > mode->height - radius)
    {
        obj->position.y = mode->height - radius;
        obj->vel_y = -obj->vel_y; // * BOUNCE_FACTOR;
    }
}

void check_collision(PhysicsObject *obj1, PhysicsObject *obj2)
{
    // Calculate positions in x and y
    int  x1 = (int)obj1->position.x;
    int  y1 = (int)obj1->position.y;
    int  x2 = (int)obj2->position.x;
    int  y2 = (int)obj2->position.y;

    // Deltas
    int  dx = x1 - x2;
    int  dy = y1 - y2;

    // Squared distance between centers
    uint32_t  dist_sq = (uint32_t) ((uint32_t) dx * (uint32_t) dx + (uint32_t) dy * (uint32_t) dy);

    // Minimum center‐to‐center to just touch (edge‐to‐edge)
    int  min_dist = obj1->radius + obj2->radius;
    uint32_t  min_dist_sq = min_dist * min_dist;

    // Only run collision logic if edges overlap or touch
    if (dist_sq > min_dist_sq) {
        return;
    }
    //! REVISAR ESTO PARA EL CONTADOR DE TOQUES
    //! if ((obj1->radius == PLAYER_RADIUS && obj2->radius == BALL_RADIUS) || ()) {
    //! state->touch_counter++;

    float dist_f = (dist_sq == 0) ? 1.0f : sqrtf((float)dist_sq);
    
    float nx = (float)dx / dist_f;
    float ny = (float)dy / dist_f;

    // Push each object half of the overlap distance
    float penetration = (float)min_dist - dist_f;
    float push = penetration * 0.5f;
    obj1->position.x += nx * push;
    obj1->position.y += ny * push;
    obj2->position.x -= nx * push;
    obj2->position.y -= ny * push;
    
    // 5) Project both velocities onto the normal & tangent directions
    //    Normal components:
    float v1n = obj1->vel_x * nx + obj1->vel_y * ny;
    float v2n = obj2->vel_x * nx + obj2->vel_y * ny;
    //    Tangential components (remaining vector after removing normal part):
    float v1t_x = obj1->vel_x - (v1n * nx);
    float v1t_y = obj1->vel_y - (v1n * ny);
    float v2t_x = obj2->vel_x - (v2n * nx);
    float v2t_y = obj2->vel_y - (v2n * ny);

    // 6) Swap normal components for an equal‐mass elastic collision
    float new_v1n = v2n;
    float new_v2n = v1n;

    // 7) Reconstruct final velocities: tangential + swapped normal
    obj1->vel_x = v1t_x + new_v1n * nx;
    obj1->vel_y = v1t_y + new_v1n * ny;
    obj2->vel_x = v2t_x + new_v2n * nx;
    obj2->vel_y = v2t_y + new_v2n * ny;
}

uint8_t check_ball_in_hole(GameState *state)
{
    int dx = state->ball.physics.position.x - state->hole.x;
    int dy = state->ball.physics.position.y - state->hole.y;

    uint32_t dist_sq = (uint32_t)(dx * dx + dy * dy);
    int dist = (int)int_sqrt(dist_sq);  


    //! REVISAR ESTO
    //! Ball falls in when its center is close enough to hole center
    //! The threshold should be positive - when ball center gets within 
    //! (hole_radius - some_margin) of hole center
    int threshold = state->holeRadius - BALL_RADIUS;

    return dist <= threshold ? 1 : 0;
}

void draw_player(Point point, int radius, uint32_t color)
{
    sys_call(SYS_DRAW_CIRCLE, point.x, point.y, radius, color, 0);
}

void draw_ball(Point point, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, point.x, point.y, radius, BALL_COLOR, 0);
}

void draw_hole(Point point, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, point.x, point.y, radius, HOLE_COLOR, 0);
}

void draw_counter(int count, ModeInfo mode) {
    set_cursor(10, 10);
    set_zoom(4);

    // Draw counter in the bottom of screen
    sys_call(SYS_DRAW_RECT,0,mode.height+OFFSET,mode.width,UI,0x00FFFFFF);
    sys_call(SYS_DRAW_RECT,10, mode.height+OFFSET+10,mode.width - 20, UI - 20, 0x00000000); // Black background for counter

    set_zoom(2);
    set_cursor(20, 725);
    putString("Touches: ");
    char buffer[10];
    itoa(count, buffer, 10);
    putString(buffer);
}

void victory_sound() {
    unsigned int melody[] = { 659, 784, 987, 1046 }; // E5, G5, B5, C6
    unsigned int duration[] = { 200, 200, 200, 400 };

    for (int i = 0; i < 4; i++) {
        sys_call(SYS_BEEP,melody[i], duration[i],0,0, 0); // Llama al syscall para hacer beep
        for (volatile int j = 0; j < 15000 * duration[i]; j++);
    }
}

void game_start_sound() {
    unsigned int melody[] = {
            523, 659, 784, 523, 659, 784, 880, 988, 1046
    }; // C5, E5, G5, C5, E5, G5, A5, B5, C6
    unsigned int duration[] = {
            200, 200, 300, 200, 200, 300, 400, 400, 600
    };
    for (int i = 0; i < 5; i++) {
        sys_call(SYS_BEEP, melody[i], duration[i], 0, 0, 0);
        for (volatile int j = 0; j < 15000*duration[i]; j++);
    }
}

