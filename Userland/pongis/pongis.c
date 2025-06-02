#include "pongis.h"

static float limit_vel(float vel, float min, float max) {
    return vel < min ? min : (vel > max ? max : vel);
}

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
    
    set_zoom(6);
    putString("PONGIS GOLF\n");
    set_zoom(4);
    putString("Press ENTER to start\n");
    putString("Press 'c' to quit\n");
    */
    
    char key = getChar();
    if (key == ' ' || key == '\n' || key == '\r') {
          pongis(mode);

    } else if (key == 'q') {
       // free(buffer);
        shell();
    }
    
}

int pongis(ModeInfo mode) {
    //! TENGO Q HACER LO DE LOS LVLS 

    float ball_x = mode.width  / 2.0f;
    float ball_y = mode.height / 2.0f;
    float player_x = mode.width  / 2.0f;
    float player_y = mode.height - 20.0f; 
    
    float vel_x  = 0.0f, vel_y = 0.0f;

    float ball_vx = 0.0f, ball_vy = 0.0f;
    int   dir_x  = 0;
    int   dir_y  = 0;
    int   running = 1;

    // Umbral al 60% para "caida en el hoyo"
    // float threshold = hole.radius - 0.6f * ball.radius;
    
    while (running) {
        dir_x = dir_y = 0;
        while (isCharReady()) {
            char k = getChar();
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
       
        float speed = _sqrt(vel_x*vel_x + vel_y*vel_y);

        // limit speed
        if (speed > MAX_SPEED) {
            float scale = MAX_SPEED / speed;
            vel_x *= scale;
            vel_y *= scale;
        }
        // coeficiente de friccion 
        vel_x *= FRICTION;
        vel_y *= FRICTION;

        // movement update 
        ball_x += vel_x;
        ball_y += vel_y;

        //limits 
        if (ball_x < BALL_RADIUS) { // izq
            ball_x = BALL_RADIUS; 
            vel_x = -vel_x;
        } else if (ball_x > (mode.width - BALL_RADIUS)) { // der
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
        



        wait_next_tick();
    }

    return 0;
}
