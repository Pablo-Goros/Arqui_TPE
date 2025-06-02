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

void pongis_init() {
    // Initialize the game, if needed
    clear_screen();
    putString("Pongis Golf\n");

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
    
    set_zoom(6);
    putString("PONGIS GOLF\n");
    set_zoom(4);
    putString("Press ENTER to start\n");
    putString("Press 'c' to quit\n");
    */
    
    char key = getChar();
    if (key == ' ' || key == '\n' || key == '\r') {
          pongis(mode);

    } else if (key == 'c') {
       // free(buffer);
       clear_screen();
        shell();
    }
    
}

int pongis(ModeInfo mode) {
    //! TENGO Q HACER LO DE LOS LVLS 
    putString("Entre\n");

    int   running = 1;
    GameState state;
    load_level(&state, 0);  
    

    // Umbral al 60% para "caida en el hoyo"
    // float threshold = hole.radius - 0.6f * ball.radius;
    
    while (running) {
        int dir_x = 0, dir_y = 0;
        while (isCharReady()) {
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
        
        /* BALL UPDATES*/
        ball_velocity_update(&state);

        ball_limit_velocity(&state);

        ball_movement_update(&state, &mode);
        /* BALL UPDATES*/
        
        uint8_t flag = check_ball_in_hole(&state);


        //Render
        /*arreglar_el_juego();*/
        draw_player(state.player_x,state.player_y, 10);/*player radius arbitrario*/
        draw_ball(state.ball_x, state.ball_y, state.ball_radius);
        draw_hole(state.holeX, state.holeY, state.holeRadius);

        wait_next_tick();
    }

    return 0;
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
    state->player_x += state->player_vel_x;
    state->player_y += state->player_vel_y;

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
    state->ball_x += state->ball_vel_x;
    state->ball_y += state->ball_vel_y;
    
    // bounce ball off screen edges
    if (state->ball_x < state->ball_radius) {
        state->ball_x = state->ball_radius;
        state->ball_vel_x = -state->ball_vel_x;
    } else if (state->ball_x > mode->width - state->ball_radius) {
        state->ball_x = mode->width - state->ball_radius;
        state->ball_vel_x = -state->ball_vel_x;
    }
    if (state->ball_y < state->ball_radius) {
        state->ball_y = state->ball_radius;
        state->ball_vel_y = -state->ball_vel_y;
    } else if (state->ball_y > mode->height - state->ball_radius) {
        state->ball_y = mode->height - state->ball_radius;
        state->ball_vel_y = -state->ball_vel_y;
    }
}

// 4) Ball–Player collision: treat each as a circle, radii = BALL_RADIUS & PLAYER_RADIUS
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
            state->ball_vel_x += rel_dot * nx;
            state->ball_vel_y += rel_dot * ny;
        }

        // move ball out so they’re not stuck overlapping
        float push_back = min_dist - dist + 0.5f;
        state->ball_x += (int)(nx * push_back);
        state->ball_y += (int)(ny * push_back);
    }
}

static uint8_t check_ball_in_hole(GameState *state) {
    int dx = state->ball_x - state->holeX;
    int dy = state->ball_y - state->holeY;
    
    int dist = (int) _sqrt(dx*dx + dy*dy);
    int min_dist = state->holeRadius + state->ball_radius;    

    return dist <= min_dist ? 1 : 0;
}







//! movelos a libc.c? o hagamos un render.c
void draw_player(int p_x, int p_y, int p_radius){
    sys_call(SYS_DRAW_CIRCLE, p_x, p_y, p_radius, 0x0000FF00,0);
}

void draw_ball(int b_x, int b_y, int b_radius){
    sys_call(SYS_DRAW_CIRCLE, b_x, b_y, b_radius, 0x00FFFFFF,0);
}

void draw_hole(int h_x, int h_y, int h_radius){
    sys_call(SYS_DRAW_CIRCLE, h_x, h_y, h_radius, 0x00808080, 0);
}

