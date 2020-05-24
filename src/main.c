#include "button.h"
#include "context.h"
#include "grid.h"
#include "layout.h"
#include "palette.h"
#include "digit.h"

#include <SDL.h>

#include <stdio.h>

static void draw_timer(
        context_t *ctx
) {
        SDL_Point pos;
        unsigned int seconds_from_start;
        unsigned int timer_seconds;
        unsigned int timer_minutes;
        char timer_str[8];

        seconds_from_start = SDL_GetTicks() / 1000;
        timer_minutes = seconds_from_start / 60;
        timer_seconds = seconds_from_start % 60;
        sprintf(timer_str, "%02u:%02u", timer_minutes, timer_seconds);
        pos.x = pos.y = 0;
        context_draw_string(ctx, timer_str, timer_color, &pos);
}

static void on_window_event(
        SDL_WindowEvent *e,
        button_t *buttons
) {
        SDL_Rect screen;

        if (e->event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                screen.x = 0;
                screen.y = 0;
                screen.w = e->data1;
                screen.h = e->data2;
                buttons_position(buttons, &screen);
        }
}

static void on_keydown_event(
        SDL_KeyboardEvent *e,
        int *quit
) {
        if (e->keysym.sym == SDLK_ESCAPE)
                *quit = 1;
}

#define UNUSED(V) (void)(V)

int main(
        int argc,
        char *argv[]
) {
        UNUSED(argc);
        UNUSED(argv);

        context_t *ctx;
        SDL_Texture **digits;
        grid_t *grid;
        button_t *buttons;
        SDL_Rect screen;
        SDL_Event event;
        SDL_Point mouse_pos;
        int quit;

        ctx = context_create();
        if (!ctx) {
                SDL_Log("Failed to create context\n");
                return -1;
        }

        digits = digits_create(ctx);
        if (!digits) {
                SDL_Log("Failed to create digits\n");
                context_destroy(ctx);
                return -1;
        }

        grid = grid_create();
        if (!grid) {
                SDL_Log("Failed to create grid\n");
                digits_destroy(digits);
                context_destroy(ctx);
                return -1;
        }

        buttons = buttons_create(grid);
        if (!buttons) {
                SDL_Log("Failed to create buttons\n");
                digits_destroy(digits);
                grid_destroy(grid);
                context_destroy(ctx);
                return -1;
        }

        screen = context_get_screen_size(ctx);
        buttons_position(buttons, &screen);

        quit = 0;
        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_WINDOWEVENT)
                                on_window_event(&event.window, buttons);
                        if (event.type == SDL_KEYDOWN)
                                on_keydown_event(&event.key, &quit);
                }
                SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
                context_clear_screen(ctx);
                buttons_draw(buttons, ctx, digits, mouse_pos);
                draw_timer(ctx);
                context_present_screen(ctx);
        }

        buttons_destroy(buttons);
        digits_destroy(digits);
        grid_destroy(grid);
        context_destroy(ctx);
        return 0;
}
