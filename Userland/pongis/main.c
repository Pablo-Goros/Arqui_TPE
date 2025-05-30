/* main.c */

#include "pongis.h"

// Physics constants (tweak as you iterate) 
#define ACCELERATION   0.2f   /* px/tick² */
#define MAX_SPEED      5.0f   /* px/tick */
#define FRICTION       0.99f  /* velocity scaling per frame */
#define BALL_RADIUS    8      /* px */

/* simple clamp */
static float clampf(float vel, float min, float max) {
    return vel < min ? min : (vel > max ? max : vel);
}

int main(void) {
    // get screen mode info
    ModeInfo mode;
    if (sys_get_mode_info(&mode) != 0) {
        return 1;
    } 

    size_t bufpx = mode.width * mode.height;
    size_t bufsz = bufpx * sizeof(uint32_t);
    uint32_t *buffer = malloc(bufsz);
    if (!buffer) return 1;

    
    float ball_x = mode.width  / 2.0f;
    float ball_y = mode.height / 2.0f;
    float vel_x  = 0.0f, vel_y = 0.0f;
    int   dir_x  = 0;
    int   dir_y  = 0;
    int   running = 1;

    
    while (running) {
        dir_x = dir_y = 0;
        while (sys_key_ready()) {
            char k = sys_read_key();
            switch (k) {
              case 'q': running = 0; break;
              case 'w': dir_y = -1; break;
              case 's': dir_y = +1; break;
              case 'a': dir_x = -1; break;
              case 'd': dir_x = +1; break;
              default: break;
            }
        }

        // velocity update
        vel_x += dir_x * ACCELERATION;
        vel_y += dir_y * ACCELERATION;

        /* clamp speed */
        float speed = sqrtf(vel_x*vel_x + vel_y*vel_y);
        if (speed > MAX_SPEED) {
            vel_x = vel_x * (MAX_SPEED / speed);
            vel_y = vel_y * (MAX_SPEED / speed);
        }

        // coeficiente de friccion 
        vel_x *= FRICTION;
        vel_y *= FRICTION;

        // movement update 
        ball_x += vel_x;
        ball_y += vel_y;

        //limits 
        if (ball_x < BALL_RADIUS) { // derecha
            ball_x = BALL_RADIUS; 
            vel_x = -vel_x;
        } else if (ball_x > (mode.width - BALL_RADIUS)) { // izquierda
            ball_x = mode.width - BALL_RADIUS; 
            vel_x = -vel_x;
        }
        if (ball_y < BALL_RADIUS) { // arriba
            ball_y = BALL_RADIUS; 
            vel_y = -vel_y;
        } else if (ball_y > (mode.height - BALL_RADIUS)) { // abajo
            ball_y = mode.height - BALL_RADIUS; 
            vel_y = -vel_y;
        }

        /* --- render --- */
        /* clear off-screen buffer */
        memset(buffer, 0, bufsz);

        /* draw ball */
        sys_draw_circle((int)ball_x, (int)ball_y, BALL_RADIUS, 0xFFFFFFFF);

        /* copy to VRAM */
        sys_blit(buffer, bufsz);

        sys_wait_next_tick();
    }

    free(buffer);
    return 0;
}
