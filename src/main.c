#include "button.h"
#include "context.h"
#include "grid.h"
#include "layout.h"
#include "palette.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

/* Simplify Digit Drawing by Caching */

static SDL_Texture **create_digits(
        context_t *ctx,
        SDL_Color *color
) {
        SDL_Texture **digits;
        int i;
        char glyph;

        digits = malloc(9 * sizeof(SDL_Texture *));
        if (!digits) {
                SDL_Log("Failed to allocate memory for digits\n");
                return NULL;
        }
        for (i = 0; i < 9; ++i) {
                glyph = '1' + i;
                digits[i] = context_prepare_glyph(ctx, '1' + i, digit_color);
        }
        return digits;
}

static void destroy_digits(
        SDL_Texture **digits
) {
        int i;

        for (i = 0; i < 9; ++i)
                SDL_DestroyTexture(digits[i]);
        free(digits);
}

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

int main(
        int argc,
        char *argv[]
) {
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

        digits = create_digits(ctx, digit_color);
        if (!digits) {
                SDL_Log("Failed to create digits\n");
                context_destroy(ctx);
                return -1;
        }

        grid = grid_create();
        if (!grid) {
                SDL_Log("Failed to create grid\n");
                destroy_digits(digits);
                context_destroy(ctx);
                return -1;
        }

        buttons = buttons_create(grid);
        if (!buttons) {
                SDL_Log("Failed to create buttons\n");
                destroy_digits(digits);
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
        destroy_digits(digits);
        grid_destroy(grid);
        context_destroy(ctx);
        return 0;
}
