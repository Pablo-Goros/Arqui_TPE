#include "pongis_lib.h"


static void handle_input(int *running, GamePhase phase, GameState *state, int *level_complete_displayed);
static void update_physics(GameState *state, ModeInfo mode);
static void render_playing(GameState *state,int prev_px, int prev_py,int prev_bx, int prev_by, ModeInfo mode);
static void render_level_complete(ModeInfo mode);
static void render_all_levels_complete(ModeInfo mode);

void pongis_init(void)
{
    clear_screen();

    ModeInfo mode;
    if (get_mode_info(&mode) != 0) {
        putString("\nError getting mode info\n");
        return;
    }

    display_welcome_screen(mode);
    handle_menu_input(mode);
}

void pongis(ModeInfo mode)
{
    clear_screen();

    int running = 1;
    GameState state;
    GamePhase phase = GAME_PLAYING;

    load_level(&state, 0);

    // previous positions, used for “clearing” only moved objects 
    int prev_player_x = state.player_x;
    int prev_player_y = state.player_y;
    int prev_ball_x   = state.ball_x;
    int prev_ball_y   = state.ball_y;

    // Flags to ensure “Level Complete” / “All Complete” messages print once
    int level_complete_displayed = 0;
    int all_complete_displayed   = 0;

    // Draw the hole background exactly once at the start 
    draw_hole(state.holeX, state.holeY, state.holeRadius);

    while (running) {
        // Handle any keypress and (possibly) change running or phase 
        handle_input(&running, phase, &state, &level_complete_displayed);

        // If PLAYING, update physics
        if (phase == GAME_PLAYING) {
            // Save current positions before updating
            prev_player_x = state.player_x;
            prev_player_y = state.player_y;
            prev_ball_x   = state.ball_x;
            prev_ball_y   = state.ball_y;

            update_physics(&state, mode);
        }

        // Check if level has been completed 
        if (phase == GAME_PLAYING) {
            if (check_ball_in_hole(&state)) {
                clear_screen();
                sys_call(SYS_RESET_KBD_BUFFER, 0, 0, 0, 0, 0);

                if (state.currentLevel + 1 < (int)level_count) {
                    phase = GAME_LEVEL_COMPLETE;
                } else {
                    phase = GAME_ALL_COMPLETE;
                }
            }
        }

        // Render according to current phase
        switch (phase) {
            case GAME_PLAYING:
                render_playing(&state, prev_player_x, prev_player_y, prev_ball_x, prev_ball_y, mode);
                break;

            case GAME_LEVEL_COMPLETE:
                if (!level_complete_displayed) {
                    render_level_complete(mode);
                    level_complete_displayed = 1;
                }
                break;

            case GAME_ALL_COMPLETE:
                if (!all_complete_displayed) {
                    render_all_levels_complete(mode);
                    all_complete_displayed = 1;
                }
                break;
        }

        wait_next_tick();
    }

    clear_screen();
}

/*
   handle_input: read a single character if available, then:
     - In GAME_PLAYING: 'c' quits immediately.
     - In GAME_LEVEL_COMPLETE: ENTER → load next level; 'c' → quit.
     - In GAME_ALL_COMPLETE: 'c' → quit.
*/
static void handle_input(int *running, GamePhase phase, GameState *state, int *level_complete_displayed)
{
    if (!isCharReady()) {
        return;
    }

    char ch = getChar();

    switch (phase) {
        case GAME_PLAYING:
            if (ch == 'c') {
                *running = 0;
            }
            break;

        case GAME_LEVEL_COMPLETE:
            if (ch == '\n' || ch == '\r') {
                // Load next level, reset flags 
                load_level(state, state->currentLevel + 1);
                *level_complete_displayed = 0;
            }
            else if (ch == 'c') {
                *running = 0;
            }
            break;

        case GAME_ALL_COMPLETE:
            if (ch == 'c') {
                *running = 0;
            }
            break;
    }
}

/*
   update_physics: apply input→direction, then move player, ball, check collision.
*/
static void update_physics(GameState *state, ModeInfo mode)
{
    int dir_x = 0, dir_y = 0;

    /* WASD check via sys_call */
    if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'w', 0, 0, 0, 0)) dir_y -= 1;
    if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'s', 0, 0, 0, 0)) dir_y += 1;
    if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'a', 0, 0, 0, 0)) dir_x -= 1;
    if (sys_call(SYS_IS_KEY_DOWN, (uint64_t)'d', 0, 0, 0, 0)) dir_x += 1;

    /* Update player velocity and position */
    player_velocity_update(dir_x, dir_y, &state->player_vel_x, &state->player_vel_y);
    movement_update(&state->player_x, &state->player_y, &state->player_vel_x, &state->player_vel_y, &mode, IS_PLAYER);

    /* Update ball velocity and position */
    ball_velocity_update(&state->ball_vel_x, &state->ball_vel_y);

    movement_update(&state->ball_x, &state->ball_y, &state->ball_vel_x, &state->ball_vel_y, &mode, IS_BALL);

    /* Check collision between player and ball */
    check_ball_player_collision(state);
}

/*
   render_playing: 
     - Clear only objects that moved (using prev positions).
     - Draw player, ball, and possibly redraw hole if overlapping.
*/
static void render_playing(GameState *state,
                           int prev_px, int prev_py,
                           int prev_bx, int prev_by,
                           ModeInfo mode)
{
    // If player moved, erase its previous circle
    if (prev_px != state->player_x || prev_py != state->player_y) {
        clear_object(prev_px, prev_py, PLAYER_RADIUS);
    }
    // If ball moved, erase its previous circle
    if (prev_bx != state->ball_x || prev_by != state->ball_y) {
        clear_object(prev_bx, prev_by, BALL_RADIUS);
    }
    // If either player or ball overlaps with the hole, redraw the hole
    if (objects_overlap(state->ball_x, state->ball_y, BALL_RADIUS, state->holeX, state->holeY, state->holeRadius) || 
    objects_overlap(state->player_x, state->player_y, PLAYER_RADIUS, state->holeX, state->holeY, state->holeRadius)) {
            draw_hole(state->holeX, state->holeY, state->holeRadius);
    }

    // Draw player and ball at new positions
    draw_player(state->player_x, state->player_y, PLAYER_RADIUS);
    draw_ball(state->ball_x, state->ball_y, BALL_RADIUS);
}

/*
   render_level_complete: show “Level Complete!” only once per level.
*/
static void render_level_complete(ModeInfo mode)
{
    set_cursor((mode.width / 2) - 200, mode.height / 2 - 50);
    set_zoom(6);
    putString("Level Complete!\n");

    set_cursor((mode.width / 2) - 300, mode.height / 2 + 50);
    set_zoom(4);
    putString("Press ENTER for next level or 'c' to quit\n");
}

static void render_all_levels_complete(ModeInfo mode)
{
    set_cursor((mode.width / 2) - 200, mode.height / 2 - 50);
    set_zoom(6);
    putString("All levels complete!\n");

    set_cursor((mode.width / 2) - 250, mode.height / 2 + 50);
    set_zoom(4);
    putString("Press 'c' to return to shell\n");
}
