#pragma once

#include <sdl.h>
#include <sdl_ttf.h>

/* Main Context Structure */

typedef struct context {
        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;
        int millis_per_frame;
        unsigned int last_render_time;
} context_t;

context_t *context_create(
        void
);

void context_destroy(
        context_t *ctx
);
