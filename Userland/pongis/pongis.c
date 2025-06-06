#include "pongis.h"

float limit_vel(float vel, float min, float max)
{
    return vel < min ? min : (vel > max ? max : vel);
}

const char *menuOptions[] = {
    "Jugar (1 jugador)      ",
    "Jugar (2 jugadores)    ",
    "Instrucciones          ",
    "Volver al Shell        "};
const int menuCount = sizeof(menuOptions) / sizeof(menuOptions[0]);

uint8_t startPongisMenu(ModeInfo mode)
{
    int selected = 0;
    drawMainMenu(selected);

    while (1)
    {
        if (!isCharReady())
        { // Wait for next char
            _hlt();
            continue;
        }

        char c = getChar();
        switch (c)
        {
        case 'w': // Move up
            selected = (selected + menuCount - 1) % menuCount;
            drawMainMenu(selected);
            break;

        case 's': // Move down
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

void drawMainMenu(int selected)
{
    clear_screen();
    set_zoom(5);
    putString("       PONGIS GOLF\n\n");

    set_zoom(3);
    for (int i = 0; i < menuCount; i++)
    {
        putString("  ");
        putString(menuOptions[i]);
        if (i == selected)
        {
            putString("  <--");
        }
        putString("\n\n");
    }
}

void drawInstructions(void)
{
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

    sys_call(SYS_RESET_KBD, 0, 0, 0, 0, 0);

    while (isCharReady())
    {
        (void)getChar();
    }

    // Wait for user input
    while (1)
    {
        if (isCharReady())
        {
            char key = getChar();
            if (key == 'c')
            {
                return;
            }
        }
        _hlt();
    }
}

void clear_object(int x, int y, int radius)
{
    sys_call(SYS_DRAW_CIRCLE, x, y, radius + 2, 0x000000, 0); // Black with slight padding
}

int objects_overlap(PhysicsObject *obj1, PhysicsObject *obj2)
{
    return calc_sq_dist(obj1, obj2) <= 0;
}

int calc_sq_dist(PhysicsObject *obj1, PhysicsObject *obj2)
{
    int dx = obj1->x - obj2->x;
    int dy = obj1->y - obj2->y;
    int dist_sq = dx * dx + dy * dy;
    int combined_radius = obj1->radius + obj2->radius;
    return dist_sq - (combined_radius * combined_radius);
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

    while (1)
    {
        uint8_t selected = startPongisMenu(mode);
        switch (selected)
        {
        case 0:
            // One player
            pongis(mode, ONE_PLAYER_MODE);
            return;

        case 1:
            // Two players
            pongis(mode, TWO_PLAYER_MODE);
            clear_screen();
            return;

        case 2:
            // Instructions
            drawInstructions();
            sys_call(SYS_RESET_KBD, 0, 0, 0, 0, 0);
            startPongisMenu(mode);
            break;

        case 3:
            // Back to shell
            clear_screen();
            return;

        default:
            return;
        }
    }
}

// void display_welcome_screen(ModeInfo mode)
// {
//     clear_screen();
//     set_zoom(6);
//     set_cursor((mode.width / 2) - 300, mode.height / 4);
//     putString("PONGIS GOLF\n");

//     set_zoom(4);

//     set_cursor((mode.width / 2) - 300, mode.height / 2);
//     putString("Press ENTER to start\n");

//     set_cursor((mode.width / 2) - 300, 3 * mode.height / 4);
//     putString("Press 'c' to quit\n");
// }

// void handle_menu_input(ModeInfo mode)
// {
//     while (1)
//     {
//         if (isCharReady())
//         {
//             char key = getChar();
//             switch (key)
//             {
//             case '\n':
//             case '\r':
//                 pongis(mode);
//                 return; // Exit after game ends
//             case 'c':
//                 clear_screen();
//                 return; // Exit to shell
//             default:
//                 break; // Ignore other keys
//             }
//         }
//     }
// }
void pongis(ModeInfo mode, int player_count)
{
    clear_screen();
    int running = 1;
    GameState state;
    GamePhase phase = GAME_PLAYING;
    load_level(&state, 0);

    state.numPlayers = player_count;
    // state.touch_counter = 0;

    // Store previous positions for all players and ball
    int prev_player_x[MAX_PLAYERS], prev_player_y[MAX_PLAYERS];
    for (int i = 0; i < state.numPlayers; i++)
    {
        prev_player_x[i] = state.players[i].physics.x;
        prev_player_y[i] = state.players[i].physics.y;
    }
    int prev_ball_x = state.ball.physics.x, prev_ball_y = state.ball.physics.y;

    int level_complete_displayed = 0;
    int all_complete_displayed = 0;

    draw_hole(state.holeX, state.holeY, state.holeRadius);
    while (running)
    {
        if (isCharReady())
        {
            char ch = getChar();
            if (phase == GAME_PLAYING)
            {
                if (ch == 'c')
                    running = 0;
            }
            else if (phase == GAME_LEVEL_COMPLETE)
            {
                if (ch == '\n' || ch == '\r')
                {
                    load_level(&state, state.currentLevel + 1);
                    phase = GAME_PLAYING;
                    // Reset previous positions for new level
                    for (int i = 0; i < state.numPlayers; i++)
                    {
                        prev_player_x[i] = state.players[i].physics.x;
                        prev_player_y[i] = state.players[i].physics.y;
                    }
                    prev_ball_x = state.ball.physics.x;
                    prev_ball_y = state.ball.physics.y;
                    level_complete_displayed = 0;
                }
                else if (ch == 'c')
                {
                    running = 0;
                }
            }
            else if (phase == GAME_ALL_COMPLETE)
            {
                if (ch == 'c')
                    running = 0;
            }
        }

        // 2) Build continuous direction for each player
        int dir_x[MAX_PLAYERS] = {0}, dir_y[MAX_PLAYERS] = {0};
        if (phase == GAME_PLAYING)
        {
            // Example: WASD for player 0, IJKL for player 1, etc.
            // You can expand this mapping for more players as needed.
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'w', 0, 0, 0, 0))
                dir_y[0] -= 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'s', 0, 0, 0, 0))
                dir_y[0] += 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'a', 0, 0, 0, 0))
                dir_x[0] -= 1;
            if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'d', 0, 0, 0, 0))
                dir_x[0] += 1;

            if (state.numPlayers > 1)
            {
                if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'i', 0, 0, 0, 0))
                    dir_y[1] -= 1;
                if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'k', 0, 0, 0, 0))
                    dir_y[1] += 1;
                if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'j', 0, 0, 0, 0))
                    dir_x[1] -= 1;
                if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'l', 0, 0, 0, 0))
                    dir_x[1] += 1;
            }
            // Add more key mappings for more players if needed

            // Store previous positions
            for (int i = 0; i < state.numPlayers; i++)
            {
                prev_player_x[i] = state.players[i].physics.x;
                prev_player_y[i] = state.players[i].physics.y;
            }
            prev_ball_x = state.ball.physics.x;
            prev_ball_y = state.ball.physics.y;

            // Update all players
            for (int i = 0; i < state.numPlayers; i++)
            {
                velocity_update(dir_x[i], dir_y[i], &state.players[i].physics.vel_x, &state.players[i].physics.vel_y, 1);
                limit_velocity(&state.players[i].physics.vel_x, &state.players[i].physics.vel_y);
                movement_update(&state.players[i].physics.x, &state.players[i].physics.y, &state.players[i].physics.vel_x, &state.players[i].physics.vel_y, &mode, 1);
            }

            // Update ball
            velocity_update(0, 0, &state.ball.physics.vel_x, &state.ball.physics.vel_y, 0);
            limit_velocity(&state.ball.physics.vel_x, &state.ball.physics.vel_y);
            movement_update(&state.ball.physics.x, &state.ball.physics.y, &state.ball.physics.vel_x, &state.ball.physics.vel_y, &mode, 0);

            check_ball_player_collision(&state);
        }

        // Check win condition
        if (phase == GAME_PLAYING)
        {
            uint8_t flag = check_ball_in_hole(&state);
            if (flag)
            {
                clear_screen();
                sys_call(SYS_RESET_KBD, 0, 0, 0, 0, 0);
                if (state.currentLevel + 1 < (int)level_count)
                {
                    phase = GAME_LEVEL_COMPLETE;
                }
                else
                {
                    phase = GAME_ALL_COMPLETE;
                }
            }
        }

        // Render based on phase
        if (phase == GAME_PLAYING)
        {
            // Clear previous positions for all players
            for (int i = 0; i < state.numPlayers; i++)
            {
                if (prev_player_x[i] != state.players[i].physics.x || prev_player_y[i] != state.players[i].physics.y)
                {
                    clear_object(prev_player_x[i], prev_player_y[i], PLAYER_RADIUS);
                }
            }
            if (prev_ball_x != state.ball.physics.x || prev_ball_y != state.ball.physics.y)
            {
                clear_object(prev_ball_x, prev_ball_y, BALL_RADIUS);
            }

            // Draw all players
            for (int i = 0; i < state.numPlayers; i++)
            {
                draw_player(state.players[i].physics.x, state.players[i].physics.y, PLAYER_RADIUS);
            }
            draw_ball(state.ball.physics.x, state.ball.physics.y, BALL_RADIUS);

            // Redraw hole only if ball or any player is overlapping
            PhysicsObject hole_obj = {.x = state.holeX, .y = state.holeY, .radius = state.holeRadius};
            int hole_overlap = objects_overlap(&state.ball.physics, &hole_obj);
            for (int i = 0; i < state.numPlayers && !hole_overlap; i++)
            {
                if (objects_overlap(&state.players[i].physics, &hole_obj))
                {
                    hole_overlap = 1;
                }
            }
            if (hole_overlap)
            {
                draw_hole(state.holeX, state.holeY, state.holeRadius);
            }
        }
        else if (phase == GAME_LEVEL_COMPLETE)
        {
            if (!level_complete_displayed)
            {
                set_cursor((mode.width / 2) - 200, mode.height / 2 - 50);
                set_zoom(6);
                putString("Level Complete!\n");
                set_cursor((mode.width / 2) - 300, mode.height / 2 + 50);
                set_zoom(4);
                putString("Press ENTER for next level or 'c' to quit\n");
                level_complete_displayed = 1;
            }
        }
        else if (phase == GAME_ALL_COMPLETE)
        {
            if (!all_complete_displayed)
            {
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

void velocity_update(int dir_x, int dir_y, float *vel_x, float *vel_y, int is_player)
{
    if (is_player)
    {
        *vel_x += dir_x * ACCELERATION;
        *vel_y += dir_y * ACCELERATION;
    }

    *vel_x *= FRICTION;
    *vel_y *= FRICTION;
}

void limit_velocity(float *vel_x, float *vel_y)
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

void movement_update(int *x, int *y, float *vel_x, float *vel_y, ModeInfo *mode, int is_player)
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
uint32_t int_sqrt(uint32_t x)
{
    // Simple integer square‐root via binary search / Newton’s method.
    // You can replace this with any fast integer-sqrt you already have.
    uint32_t op = x;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // 2^30, highest power of four <= UINT32_MAX

    // "one" starts at the highest power of four <= the argument.
    while (one > op)
    {
        one >>= 2;
    }

    while (one != 0)
    {
        if (op >= res + one)
        {
            op -= res + one;
            res = (res >> 1) + one;
        }
        else
        {
            res = res >> 1;
        }
        one >>= 2;
    }
    return res;
}

void check_ball_player_collision(GameState *state)
{
    Ball *ball = &state->ball;

    // Player-ball collisions
    for (int i = 0; i < state->numPlayers; i++)
    {
        Player *player = &state->players[i];
        int dx = ball->physics.x - player->physics.x;
        int dy = ball->physics.y - player->physics.y;
        int dist = (int)_sqrt(dx * dx + dy * dy);
        int min_dist = BALL_RADIUS + PLAYER_RADIUS;

        if (dist <= min_dist)
        {
            // Only update lastTouchId if ball collides with player
            ball->lastTouchId = player->id;
            if (dist < 1)
            {
                dx = 1;
                dy = 0;
                dist = 1;
            }
            float nx = (float)dx / dist;
            float ny = (float)dy / dist;

            float rel_vx = player->physics.vel_x - ball->physics.vel_x;
            float rel_vy = player->physics.vel_y - ball->physics.vel_y;
            float rel_dot = rel_vx * nx + rel_vy * ny;
            if (rel_dot > 0.0f)
            {
                ball->physics.vel_x += (int)rel_dot * nx;
                ball->physics.vel_y += (int)rel_dot * ny;
            }

            float push_back = min_dist - dist;
            ball->physics.x += (int)(nx * push_back);
            ball->physics.y += (int)(ny * push_back);

            // Do not break here, so player-player collisions are also checked
        }
    }

    // Player-player collisions
    for (int i = 0; i < state->numPlayers; i++)
    {
        for (int j = i + 1; j < state->numPlayers; j++)
        {
            Player *p1 = &state->players[i];
            Player *p2 = &state->players[j];
            int dx = p1->physics.x - p2->physics.x;
            int dy = p1->physics.y - p2->physics.y;
            int dist = dx * dx + dy * dy;
            int min_dist = 4 * (PLAYER_RADIUS * PLAYER_RADIUS);

            if (dist < min_dist)
            {
                if (dist < 1)
                {
                    dx = 1;
                    dy = 0;
                    dist = 1;
                }
                float nx = (float)dx / dist;
                float ny = (float)dy / dist;

                // Simple elastic collision: swap velocities along normal
                float rel_vx = p1->physics.vel_x - p2->physics.vel_x;
                float rel_vy = p1->physics.vel_y - p2->physics.vel_y;
                float rel_dot = rel_vx * nx + rel_vy * ny;
                if (rel_dot < 0.0f)
                {
                    // Only resolve if moving towards each other
                    float impulse = -rel_dot * 0.5f;
                    p1->physics.vel_x += impulse * nx;
                    p1->physics.vel_y += impulse * ny;
                    p2->physics.vel_x -= impulse * nx;
                    p2->physics.vel_y -= impulse * ny;
                }

                // Push them apart so they don't overlap
                float push_back = (min_dist - dist) / 2.0f;
                p1->physics.x += (int)(nx * push_back);
                p1->physics.y += (int)(ny * push_back);
                p2->physics.x -= (int)(nx * push_back);
                p2->physics.y -= (int)(ny * push_back);
            }
        }
    }
}

uint8_t check_ball_in_hole(GameState *state)
{
    int dx = state->ball.physics.x - state->holeX;
    int dy = state->ball.physics.y - state->holeY;

    int dist = (int)_sqrt(dx * dx + dy * dy);

    // Ball falls in when it's mostly inside the hole
    int threshold = state->holeRadius - (BALL_RADIUS * 0.8f);

    return dist <= threshold;
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
