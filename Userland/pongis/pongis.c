#include "pongis.h"

static float limit_vel(float vel, float min, float max) {
    return vel < min ? min : (vel > max ? max : vel);
}

static void player_velocity_update(int dir_x, int dir_y, GameState *state);

static void player_limit_velocity(GameState *state);

static void player_movement_update(GameState *state, ModeInfo *mode);

static void ball_velocity_update(GameState *state);

static void ball_limit_velocity(GameState *state);

static void ball_movement_update(GameState *state, ModeInfo *mode);

static uint8_t check_ball_in_hole(GameState *state);

static void check_ball_player_collision(GameState *state);


void pongis_init() {
    // Initialize the game, if needed
    clear_screen();

    ModeInfo mode;

    // ! PREGUNTARRRRRRRRRRRRRRRRRRRRRRR
    
    uint64_t ret = get_mode_info(&mode);
    /*
    if (ret != 0) {
        char buffer[10];
        itoa(ret, buffer, 10);
        putString(buffer);
        char buffer1[10];
        char buffer2[10];
        char buffer3[10];

        itoa(mode.width, buffer1, 10);
        itoa(mode.height, buffer2, 10);
        itoa(mode.bpp, buffer3, 10);
        putString(buffer1);
        putChar('\n');
        putString(buffer2);
        putChar('\n');
        putString(buffer3);
        putString("\nError getting mode info\n");
        return;
    }
    */

    // Display welcome screen
    // ! PREGUNTARRRRRRRRRRRRRRRRRRRRRRR
    /*
    int centerX = mode.width;
    int centerY = mode.height;
    char buffer1[10];
    char buffer2[10];
    itoa(centerX, buffer1, 10);
    itoa(centerY, buffer2, 10);
    putString("Center X: ");
    putString(buffer1);
    putChar('\n');
    putString("Center Y: ");
    putString(buffer2);
    putChar('\n');

    /*
    // Create buffer for welcome screen
    uint64_t bufpx = mode.width * mode.height;
    uint64_t bufsz = bufpx * sizeof(uint32_t);
    uint32_t *buffer = malloc(bufsz);
    if (!buffer) return;
    
    // Clear buffer
    memset(buffer, 0, bufsz);
    
    set_cursor(centerX/2, centerY/2 );
    */
    set_zoom(6);
    putString("PONGIS GOLF\n");
    set_zoom(4);
    putString("Press ENTER to start\n");
    putString("Press 'c' to quit\n");
    
    while (1) {
        if (isCharReady()) {
            char key = getChar();
            if (key == '\n' || key == '\r') {
                pongis(mode);
                break;  // Exit the loop to start the game
            } else if (key == 'c') {
                clear_screen();
                return;  // Exit the game
            }
        }

    }

}

void pongis(ModeInfo mode) {
    //! TENGO Q HACER LO DE LOS LVLS 
    clear_screen();

    int   running = 1;
    GameState state;
    load_level(&state, 0);  
    

    // Umbral al 60% para "caida en el hoyo"
    // float threshold = hole.radius - 0.6f * ball.radius;
    
    while (running) {
        int dir_x = 0, dir_y = 0;
        if (isCharReady()) {
            char k = getChar();
            switch (k) {
              case 'c': running = 0; break;
              case 'w': dir_y = -1; break;
              case 's': dir_y = +1; break;
              case 'a': dir_x = -1; break;
              case 'd': dir_x = +1; break;
              default: break;
            }
        }
    
        /* PLAYER UPDATES*/
        player_velocity_update(dir_x, dir_y, &state);
        
        player_limit_velocity(&state);
        
        player_movement_update(&state, &mode);
        /* PLAYER UPDATES*/
        putString("actualice player");

        /* BALL UPDATES*/
        ball_velocity_update(&state);

        ball_limit_velocity(&state);

        ball_movement_update(&state, &mode);
        /* BALL UPDATES*/
        check_ball_player_collision(&state);

        putString("actualice ball");
        
        uint8_t flag = check_ball_in_hole(&state);

        if (flag) {
            clear_screen();
            set_zoom(6);
            putString("Level Complete!\n");
            set_zoom(4);
            if (state.currentLevel + 1 < (int)level_count) {
                putString("Press ENTER for next level or 'c' to quit\n");
                char key = getChar();  // Blocking wait for input
                if (key == '\n' || key == '\r') {
                    load_level(&state, state.currentLevel + 1);
                    continue;  // Continue game loop with new level
                } else if (key == 'c') {
                    running = 0;  // Exit loop
                }
            } else {
                putString("All levels complete! Press any key to return to shell\n");
                getChar();  // Wait for any key
                running = 0;  // Exit loop
            }
        }

        clear_screen();

        //Render
        draw_player(state.player_x,state.player_y, PLAYER_RADIUS);
        draw_ball(state.ball_x, state.ball_y, BALL_RADIUS);
        draw_hole(state.holeX, state.holeY, state.holeRadius);


        
        wait_next_tick();
    }
    clear_screen();

    return;
}

static void player_velocity_update(int dir_x, int dir_y, GameState *state) {
    state->player_vel_x += dir_x * ACCELERATION;
    state->player_vel_y += dir_y * ACCELERATION;

    state->player_vel_x *= FRICTION;
    state->player_vel_y *= FRICTION;
}

static void player_limit_velocity(GameState *state) {
    float speed = _sqrt(state->player_vel_x*state->player_vel_x + state->player_vel_y*state->player_vel_y);

    // limit player speed
    if (speed > MAX_SPEED) {
        float scale = MAX_SPEED / speed;
        state->player_vel_x *= scale;
        state->player_vel_y *= scale;
    }
}

static void player_movement_update(GameState *state, ModeInfo *mode) {
    state->player_x += (int)state->player_vel_x;
    state->player_y += (int)state->player_vel_y;

    if (state->player_x < PLAYER_RADIUS) {
        state->player_x = PLAYER_RADIUS;
        state->player_vel_x = -state->player_vel_x;
    } else if (state->player_x > mode->width - PLAYER_RADIUS) {
        state->player_x = mode->width - PLAYER_RADIUS;
        state->player_vel_x = -state->player_vel_x;
    }

    if (state->player_y < PLAYER_RADIUS) {
        state->player_y = PLAYER_RADIUS;
        state->player_vel_y = -state->player_vel_y;
    } else if (state->player_y > mode->height - PLAYER_RADIUS) {
        state->player_y = mode->height - PLAYER_RADIUS;
        state->player_vel_y = -state->player_vel_y;
    }
}

static void ball_velocity_update(GameState *state) {
    state->ball_vel_x *= FRICTION;
    state->ball_vel_y *= FRICTION;
}

static void ball_limit_velocity(GameState *state) {
    // limit ball speed
    float speed = _sqrt(state->ball_vel_x*state->ball_vel_x + state->ball_vel_y*state->ball_vel_y);
    if (speed > MAX_SPEED) {
        float scale = MAX_SPEED / speed;
        state->ball_vel_x *= scale;
        state->ball_vel_y *= scale;
    }
}

static void ball_movement_update(GameState *state, ModeInfo *mode) {
    state->ball_x += (int)state->ball_vel_x;
    state->ball_y += (int)state->ball_vel_y;
    
    // bounce ball off screen edges
    if (state->ball_x < BALL_RADIUS) {
        state->ball_x = BALL_RADIUS;
        state->ball_vel_x = -state->ball_vel_x;
    } else if (state->ball_x > mode->width - BALL_RADIUS) {
        state->ball_x = mode->width - BALL_RADIUS;
        state->ball_vel_x = -state->ball_vel_x;
    }
    if (state->ball_y < BALL_RADIUS) {
        state->ball_y = BALL_RADIUS;
        state->ball_vel_y = -state->ball_vel_y;
    } else if (state->ball_y > mode->height - BALL_RADIUS) {
        state->ball_y = mode->height - BALL_RADIUS;
        state->ball_vel_y = -state->ball_vel_y;
    }
}

static void check_ball_player_collision(GameState *state) {
    int dx = state->ball_x - state->player_x;
    int dy = state->ball_y - state->player_y;
    int dist = (int) _sqrt(dx*dx + dy*dy);
    int min_dist = BALL_RADIUS + PLAYER_RADIUS;
    
    if (dist <= min_dist) {
        if (dist < 1) {
            // extremely rare overlap; pick arbitrary direction
            dx = 1; dy = 0; dist = 1;
        }
        float nx = (float)dx / dist;
        float ny = (float)dy / dist;

        // relative velocity of player vs. ball
        float rel_vx = state->player_vel_x - state->ball_vel_x;
        float rel_vy = state->player_vel_y - state->ball_vel_y;
        // project relative velocity onto normal
        float rel_dot = rel_vx * nx + rel_vy * ny;
        if (rel_dot > 0.0f) {
            // player is “pushing” into the ball
            // simply transfer “rel_dot” along normal into ball
            state->ball_vel_x += (int) rel_dot * nx;
            state->ball_vel_y += (int) rel_dot * ny;
        }

        // move ball out so they’re not stuck overlapping
        float push_back = min_dist - dist;
        state->ball_x += (int)(nx * push_back);
        state->ball_y += (int)(ny * push_back);
    }
}

static uint8_t check_ball_in_hole(GameState *state) {
    int dx = state->ball_x - state->holeX;
    int dy = state->ball_y - state->holeY;
    
    int dist = (int) _sqrt(dx*dx + dy*dy);

    // Ball falls in when it's mostly inside the hole
    int threshold = state->holeRadius - (BALL_RADIUS * 0.8f); 

    return dist <= threshold ? 1 : 0;
}

//? quizas agregar una variable en draw circle q sea con o sin outline, por ejemplo para el hole...

void draw_player(int x, int y, int radius) {
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, PLAYER_COLOR, 0);
}

void draw_ball(int x, int y, int radius) {
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, BALL_COLOR, 0);
}

void draw_hole(int x, int y, int radius) {
    sys_call(SYS_DRAW_CIRCLE, x, y, radius, HOLE_COLOR, 0);  
}



